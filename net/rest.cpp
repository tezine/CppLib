#include <QtNetwork>
#include <QJSValueIterator>
#include <QQuickItem>
#include "rest.h"
#include "dstore.h"
#include "json.h"
#include "restreply.h"
#include "ctezine.h"

QPointer<QNetworkAccessManager> Rest::staticAccessManager=0;
Rest::Rest( QObject *parent) : QObject(parent){
    messageID=0;
    loadingObject=NULL;
    isNewRest=false;
    if(staticAccessManager)manager=staticAccessManager;
    else manager=new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(slotReplyFinished(QNetworkReply *)));
    connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(slotSslErrors(QNetworkReply*,QList<QSslError>)));
}

/*
 *Ex: setParameters({'email':email, 'appVersion':mCopagazUsuario.getAppVersion()})
 */
Rest* Rest::setParameters(QVariantMap parameters){
    if (this->parameters == parameters)return this;
    this->parameters = parameters;
    emit parametersChanged(parameters);
    return this;
}

/*
 *
 */
Rest *Rest::appendParameter(const QString &parameterName, const QString &value){
    this->parameters[parameterName]=value;
    emit parametersChanged(this->parameters);
    return this;
}

/*
 *Ex:
   var parameters=new Object;
    parameters.email=email;
    parameters.appVersion=mCopagazUsuario.getAppVersion();
    rest.setParametersObject(parameters);
 */
Rest * Rest::setParametersObject(QJSValue parameters){
    if(!parameters.isObject()){qDebug()<<"invalid parameter";return this;}
    QJSValueIterator it(parameters);
    while (it.hasNext()) {
        it.next();
        qDebug() <<"(setParametersObject)"<< it.name() << ": " << it.value().toString();
        this->parameters[it.name()]=it.value().toString();
    }
    emit parametersChanged(this->parameters);
    return this;
}

/*
 *
 */
Rest* Rest::setBaseUrl(const QString &baseURL){
    if (this->baseUrl == baseURL)return this;
    this->baseUrl = baseURL;
    emit baseURLChanged(baseURL);
    return this;
}

/*
 *Ex: setRoute("/Users/Authenticate")
 */
Rest* Rest::setRoute(const QString &route){
    if (this->route == route)return this;
    this->route = route;
    emit routeChanged(route);
    return this;
}

Rest *Rest::setToken(const QString &token){
    this->token=token;
    return this;
}

/*
 *Ex: setUrlSegments([login, passwordHash])
 */
Rest* Rest::setUrlSegments(const QStringList &urlSegments) {
    if (this->urlSegments == urlSegments)return this;
    this->urlSegments = urlSegments;
    emit urlSegmentsChanged(urlSegments);
    return this;
}

/*
 *Ex: appendUrlSegment(password);
 *PS: DO NOT use encodeURIComponent(txt) to encode the segment
 */
Rest *Rest::appendUrlSegment(const QString &urlSegment){
    QByteArray encoded=QUrl::toPercentEncoding(urlSegment);
    urlSegments.append(QString(encoded));
    emit urlSegmentsChanged(urlSegments);
    return this;
}


/*
 *
 */
Rest *Rest::setBody(const QString &body){
    if (this->body == body)return this;
    this->body = body;
    emit bodyChanged(body);
    return this;
}

/*
 *
 */
void Rest::setLoadingObject(QQuickItem *loadingObject){
    if (this->loadingObject == loadingObject)return;
    this->loadingObject = loadingObject;
    emit loadingObjectChanged(loadingObject);
}

/*
 *
 */
Rest* Rest::clear(){
    route.clear();
    urlSegments.clear();
    loadingObject=NULL;
    token.clear();
    parameters.clear();
    emit sigCancelAllRequests();
    return this;
}

/*
 *POST operation
 */
void Rest::post(){

}

/*
 *POST operation
 */
void Rest::post(QJSValue callBackFunction){
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)POST:"<<url.toString();
    configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    configureReply(manager->post(request, configureBody(QJSValue(body))));
}

/*
 *POST operation from C++
 */
void Rest::post(QObject *objReceiver, const char *slotResponseName){
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)POST:"<<url.toString();
    configureSLOTReceiver(objReceiver,url.toString(),++messageID,slotResponseName);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    configureReply(manager->post(request, configureBody(QJSValue(body))));
}

/*
 *GET operation
 */
QNetworkReply* Rest::get(){
    if(loadingObject)loadingObject->setVisible(true);
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)GET:"<<url.toString();
    QNetworkRequest request(url);
    configureRequest(request);
    QNetworkReply *reply=manager->get(request);
    configureReply(reply);
    return reply;
}

/*
 *GET operation
 * Ex: rest.get(function(response){

        });
 */
void Rest::get(QJSValue callBackFunction){
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)GET:"<<url.toString();
    configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    configureReply(manager->get(request));
}

/*
 *PUT operation
 */
void Rest::put(QJSValue callBackFunction){
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)PUT:"<<url.toString();
    configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    configureReply(manager->put(request, configureBody(QJSValue(body))));
}


//GET for typescript
QObject *Rest::getForTypescript(QJSValue progressBarValue){
    isNewRest=true;
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"==========================================================================";
    qDebug()<<"-->(GET) "+url.toString();
    messageID++;
    //configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    RestReply *restReply=new RestReply(this, manager->get(request), messageID, url, progressBarValue);
    return (QObject*)restReply;
}

//PUT for typescript
QObject *Rest::putForTypescript(QJSValue progressBarValue){
    isNewRest=true;
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"==========================================================================";
    qDebug()<<"-->(PUT) "+url.toString();
    messageID++;
    //configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    RestReply *restReply=new RestReply(this, manager->put(request,configureBody(QJSValue(body))), messageID, url, progressBarValue);
    return (QObject*)restReply;
}

QObject *Rest::postForTypescript(QJSValue progressBarValue){
    isNewRest=true;
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"==========================================================================";
    qDebug()<<"-->(POST) "+url.toString();
    messageID++;
    //configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    RestReply *restReply=new RestReply(this, manager->post(request,configureBody(QJSValue(body))), messageID, url, progressBarValue);
    return (QObject*)restReply;
}

QObject *Rest::delForTypescript(QJSValue progressBarValue){
    isNewRest=true;
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"==========================================================================";
    qDebug()<<"-->(DELETE) "+url.toString();
    messageID++;
    //configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    RestReply *restReply=new RestReply(this, manager->deleteResource(request), messageID, url, progressBarValue);
    return (QObject*)restReply;
}

/*
 *PUT operation
 */
void Rest::put(){

}



/*
 *DELETE operation
 */
void Rest::del(){

}

/*
 *DELETE operation
 */
void Rest::del(QJSValue callBackFunction){
    QUrl url=configureUrl(baseUrl,route,urlSegments,parameters);
    qDebug()<<"-->(Rest)DELETE:"<<url.toString();
    configureCallbackReceiver(callBackFunction,url.toString(), ++messageID);
    QNetworkRequest request(url);
    configureRequest(request,messageID);
    configureReply(manager->deleteResource(request));
}

/*
 *
 */
void Rest::slotReplyFinished(QNetworkReply *reply){
    if(isNewRest)return;
    if(loadingObject)loadingObject->setVisible(false);
    qDebug()<<"<--(Rest)Reply from:"<<reply->request().url().toString();//<<reply->request().hasRawHeader("MsgID") ;//<<reply->request().rawHeader("MsgID");
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->error() << reply->errorString();
        //emit replyError(reply, reply->error(), reply->errorString());
        return;
    }
    if(!reply->request().hasRawHeader("MsgID"))emit success(reply->readAll());
    else {
        qint64 msgID=reply->request().rawHeader("MsgID").toLongLong();
        if(!receiverHash.contains(msgID))return;
        STRReceiver *strReceiver=receiverHash[msgID];
        if(strReceiver->isCallBack)invokeCallBack(strReceiver->callBackFunction,strReceiver->completeUrl, reply->readAll());
        else invokeSLOT(strReceiver->completeUrl, strReceiver->obj,strReceiver->slotName,reply->readAll());//C++ slot
        receiverHash.remove(msgID);
        return;
    }
}


/*
 *Invokes the javascript function from the response received
 */
void Rest::invokeCallBack(QJSValue callBackFunction, const QString &completeServerFunction, const QByteArray &array){
    emit sigProgress(false);
    qDebug()<<"<--("+completeServerFunction+")" <<array;
    //qDebug()<<array;
    callBackFunction.call(QJSValueList()<<QJSValue(QString(array)));
}

/*
 *
 */
void Rest::invokeSLOT(const QString &requestURL, QObject *obj, const QByteArray &slot, const QByteArray &response){
    if(obj==0 || slot.isEmpty())return;
    qDebug()<<"<--("+requestURL+")"<<response;
    if(!obj->metaObject()->invokeMethod(obj,slot.constData(),Q_ARG(QByteArray, response))){
        lastError="Unable to invoke:"+QString(slot);
        emit error(lastError);
    }
}


/*
 *
 */
QUrl Rest::configureUrl(const QString &baseURL, const QString &route, const QStringList &urlSegments, const QVariantMap &parameters){
    QUrl url = QUrl();
    QUrlQuery query;
    QString internalRoute(route);
    if(!route.startsWith("/"))internalRoute.prepend("/");
    QString path=baseURL+internalRoute;
    foreach(QString segment, urlSegments){
        path+="/"+segment;
    }
    url.setUrl(path);
    QMapIterator<QString, QVariant> i(parameters);
    while (i.hasNext()) {
        i.next();
        //qDebug() << i.key() << ": " << i.value();
        query.addQueryItem(i.key(),i.value().toString());
    }
    url.setQuery(query.query());
    return url;
}

/*
 *
 */
void Rest::configureRequest(QNetworkRequest &request, qint64 msgID){
    if(msgID!=-1) request.setRawHeader("MsgID",QString::number(messageID).toUtf8().constData());
    request.setRawHeader("Content-Type", "application/json");
    if(token.length()>0)request.setRawHeader("Authorization", QString(QString("Bearer ") +token).toUtf8().constData());
    //request->setRawHeader(acceptHeader(), accept());
    //request->setRawHeader(authTokenHeader(), authToken());
}

QByteArray Rest::configureBody(QJSValue body){
    QString bodyContent=body.toString();
    if(bodyContent.isEmpty())return QByteArray();
    QJsonParseError error;
    QJsonDocument doc= QJsonDocument::fromJson(bodyContent.toUtf8(),&error);
    if(doc.isNull()){
        //qDebug()<<"enviando string como body";
        return QString("'"+bodyContent+"'").toUtf8();
    }else{
        //qDebug()<<"enviando json:"<<bodyContent;
        return bodyContent.toUtf8();
    }
}

/*
 * It will invoke a javascript function when receives the reply from server
 */
void Rest::configureCallbackReceiver(QJSValue callBackFunction, const QString &completeServerUrl, quint64 msgID){
    STRReceiver *receiver=new STRReceiver();
    receiver->completeUrl=completeServerUrl;
    receiver->callBackFunction=callBackFunction;
    receiver->isCallBack=true;
    receiverHash[msgID]=receiver;
}

/*
 *
 */
void Rest::configureSLOTReceiver(QObject *objReceiver, const QString &completeServerUrl, qint64 msgID, const char *slot){
    STRReceiver *receiver=new STRReceiver();
    receiver->isCallBack=false;
    receiver->obj=objReceiver;
    receiver->completeUrl=completeServerUrl;
    QRegExp rx("\\(.*");
    rx.setMinimal(false);
    QString name(slot);
    if(name.isEmpty())receiver->slotName=QByteArray();
    else{
        name.remove(rx);
        receiver->slotName=QMetaObject::normalizedSignature(name.remove(0,1).toUtf8().constData());
    }
    receiverHash[msgID]=receiver;
}

/*
 *
 */
void Rest::configureReply(QNetworkReply *reply){
    if(CTezine::obj()->getAutoCancelPreviousRequest()) {//o  padrao é true. só o sysadmin que nao é
        connect(this,SIGNAL(sigCancelAllRequests()),reply,SLOT(abort()));
    }
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotReplyError(QNetworkReply::NetworkError)));
    //connect(reply, SIGNAL(sslErrors(QList<QSslError>)),this, SLOT(slotSslErrors(QList<QSslError>)));
}


/*
 *
 */
void Rest::slotReplyError(QNetworkReply::NetworkError error){
    qDebug() << "(Rest)Error" << error;

    CTezine::obj()->emitSigRestError("Network Error:"+QString::number(error));
}

/*
 *
 */
void Rest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    qDebug() << errors;
}








