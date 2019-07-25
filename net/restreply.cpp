#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QQmlApplicationEngine>
#include "cqml.h"
#include "restreply.h"
#include "rest.h"

RestReply::RestReply(Rest *rest, QNetworkReply *reply, qint64 messageID, const QUrl &completeServerURL, QJSValue progressBarValue) : QObject(reply){
    this->reply=reply;
    this->completeServerURL=completeServerURL;
    this->progressBarValue=progressBarValue;
    //todo adicionar QObject::connect(this,SIGNAL(sigCancelAllRequests()),reply,SLOT(abort()));
    //todo adicionar QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotReplyError(QNetworkReply::NetworkError)));
    //connect(reply, SIGNAL(sslErrors(QList<QSslError>)),this, SLOT(slotSslErrors(QList<QSslError>)));
    QObject::connect(reply,SIGNAL(finished()),this,SLOT(slotReplyFinished()));
}

RestReply::~RestReply(){
    //qDebug()<<"(RestReply)destructor"<<reply;
    if(reply!=NULL){
        reply->abort();
        reply->deleteLater();
    }
}

void RestReply::connect(QJSValue callBackFunction){
    //qDebug()<<"(RestReply)connect "<<callBackFunction.isCallable();
    isNewConnect=false;
    this->callBackFunction=callBackFunction;
}

void RestReply::connectWithThis(QJSValue qmlObject, QJSValue callBackFunction){
    isNewConnect=true;
    this->qmlObject=qmlObject;
    this->callBackFunction=callBackFunction;
}

void RestReply::slotReplyFinished(){
    //qDebug()<<"(RestReply)slotReplyFinished"<<reply->request().url().toString();//<<reply->request().hasRawHeader("MsgID") ;//<<reply->request().rawHeader("MsgID");
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->error() << reply->errorString();
        //emit replyError(reply, reply->error(), reply->errorString());
        return;
    }
    emit sigReplyFinished();//para listeners em c++
    if(!callBackFunction.isCallable())return;
    //todo recolocar emit sigProgress(false);
    if(progressBarValue.isObject())progressBarValue.toQObject()->setProperty("visible",false);
    QString responseString= QString::fromUtf8(reply->readAll());
    //responseString= responseString.remove('\r');
    //responseString=responseString.remove('\n');
    qDebug()<<"<--(REPLY)" +responseString;// from "+completeServerURL.toString();
    //qDebug()<<responseString;
    qDebug()<<"==========================================================================";
    QJSValue jsValue(responseString);
    //JSON.parse pelo C++ nao funciona muito bem quando a string é muito longa. Ver pending deliveries
   //QJSValue jsValue= CQML::obj()->getQMLEngine()->evaluate("JSON.parse('"+responseString+"');");
    if (jsValue.isError()){
          qDebug()<< "Uncaught exception at line"<< jsValue.property("lineNumber").toInt()<< ":" << jsValue.toString();
    }
    //qDebug()<<"jsValue:"<<jsValue.isObject()<<jsValue.toString()<<jsValue.isError()<<jsValue.isNull();
    if(isNewConnect)callBackFunction.call(QJSValueList()<<this->qmlObject<<jsValue);
    else callBackFunction.call(QJSValueList()<<jsValue);

    this->deleteLater();
}
