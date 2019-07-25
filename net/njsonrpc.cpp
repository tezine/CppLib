#include <QtCore>
#include <QtNetwork>
#include <QQmlApplicationEngine>
#include "qjsonrpchttpclient.h"
#include "njsonrpc.h"
#include "ctezine.h"
#include "cdefines.h"
#include "blogger.h"
#include "json.h"
#include "cmeta.h"
#include "cenums.h"
#include "efirstsvcarg.h"
#include "eloginresponse.h"
#include "emetaresponse.h"
#include "cqml.h"

///VEIO DO wikios RUNTIME

QPointer<NJsonRpc> NJsonRpc::o=0;
NJsonRpc::NJsonRpc(QObject *parent) :QObject(parent){
	manager=new QNetworkAccessManager(this);
	client=new QJsonRpcHttpClient(manager,this);
    userTempKey=QString("").toUtf8();
}

void NJsonRpc::setServerUrl(const QString &serverUrl){
	this->serverUrl=QUrl(serverUrl);
}

QUrl NJsonRpc::getSVCUrl(){
	return serverUrl;
}


/*
 *Calls the webservice method
 */
void NJsonRpc::call(const QString & serverClassTypeName, const QString &functionName, QObject *objReceiver, const char *slotResponseName, const QVariant &arg1,
				const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6, const QVariant &arg7){
	qint32 argsCount=0;
	QVariantList argList;
	if(arg1.isValid())argsCount++;
	if(arg2.isValid())argsCount++;
	if(arg3.isValid())argsCount++;
	if(arg4.isValid())argsCount++;
	if(arg5.isValid())argsCount++;
	if(arg6.isValid())argsCount++;
	if(arg7.isValid())argsCount++;
	EFirstSVCArg firstSVCArg;
    //todo recolocar firstSVCArg.setAppType(EDefines::AppTypeEnguiaMobile);
	firstSVCArg.setUserTempKey(userTempKey);
    firstSVCArg.setClassName(serverClassTypeName);
	firstSVCArg.setFunctionName(functionName);
	firstSVCArg.setArgsCount(argsCount);//firstSVCArg is not counted
	QString firstSVCArgJson=JSON::convertObjectToJsonDoc(firstSVCArg);
	argList.append(firstSVCArgJson);
	argList.append(arg1);
	argList.append(arg2);
	argList.append(arg3);
	argList.append(arg4);
	argList.append(arg5);
	argList.append(arg6);
	argList.append(arg7);
	argList.append(QVariant());
	QJsonRpcMessage jMessage =QJsonRpcMessage::createRequest("Invoke",QJsonArray::fromVariantList(argList));
    client->setEndPoint(getSVCUrl().toString());
	addReceiver(objReceiver,jMessage.id(), slotResponseName);
	QJsonRpcServiceReply *reply=client->sendMessage(jMessage);
	connect(reply,SIGNAL(sigUploadProgress(qint64,qint64)),this,SLOT(slotUploadProgress(qint64,qint64)));
	connect(reply, SIGNAL(finished()), this, SLOT(slotReplyFinished()));	
	connect(reply,SIGNAL(sigError(QNetworkReply::NetworkError)),this,SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
}


/*
 *Calls SVC MetaInvoke function
 */
void NJsonRpc::metaInvoke(const QString & serverClassTypeName, const QString &functionName, QJSValue callBackFunction,
					  const QVariant &arg1,const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6, const QVariant &arg7){
	EFirstSVCArg firstSVCArg;
    //todo recolcar firstSVCArg.setAppType(EDefines::AppTypeEnguiaMobile);//todo remover
	firstSVCArg.setUserTempKey(userTempKey);
    firstSVCArg.setClassName(serverClassTypeName);
	firstSVCArg.setFunctionName(functionName);
	QVariantList argList;
	if(arg1.isValid()){
		if(arg1.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg1.value<QObject*>())));
		else argList.append(arg1);
	}
	if(arg2.isValid()){
		if(arg2.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg2.value<QObject*>())));
		else argList.append(arg2);
	}
	if(arg3.isValid()){
		if(arg3.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg3.value<QObject*>())));
		else argList.append(arg3);
	}
	if(arg4.isValid()){
		if(arg4.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg4.value<QObject*>())));
		else argList.append(arg4);
	}
	if(arg5.isValid()){
		if(arg5.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg5.value<QObject*>())));
		else argList.append(arg5);
	}
	if(arg6.isValid()){
		if(arg6.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg6.value<QObject*>())));
		else argList.append(arg6);
	}
	if(arg7.isValid()){
		if(arg7.type()==QMetaType::QObjectStar)argList.append(JSON::convertObjectToJsonDoc(*(arg7.value<QObject*>())));
		else argList.append(arg7);
	}
	for(int i=0;i<argList.count();i++){
		if(argList[i].type()==QMetaType::QDateTime){
			QDateTime dateTime=argList[i].value<QDateTime>();
			QDateTime dx=dateTime.toOffsetFromUtc(dateTime.offsetFromUtc());
			qDebug()<<"convertendo datetime:"<<dx;
			argList[i]=dx.toString(Qt::ISODate);
		}
	}
	firstSVCArg.setArgsCount(argList.count());
	//let's se to null until arg8 if not used. We have to send 8 real arguments
	while(argList.count()<8){
		argList.append(QVariant());
	}
	QString firstSVCArgJson=JSON::convertObjectToJsonDoc(firstSVCArg);
	argList.prepend(QVariant::fromValue(firstSVCArgJson));
	QJsonRpcMessage jMessage =QJsonRpcMessage::createRequest("MetaInvoke", QJsonArray::fromVariantList(argList));
    client->setEndPoint(getSVCUrl());
	addCallBackReceiver(callBackFunction,firstSVCArg.getClassName()+"."+functionName, jMessage.id());
	emit sigProgress(true,-1);
	QJsonRpcServiceReply *reply=client->sendMessage(jMessage);
	connect(reply,SIGNAL(sigUploadProgress(qint64,qint64)),this,SLOT(slotUploadProgress(qint64,qint64)));
	connect(reply, SIGNAL(finished()), this, SLOT(slotReplyFinished()));
	connect(reply,SIGNAL(sigError(QNetworkReply::NetworkError)),this,SLOT(slotNetworkReplyError(QNetworkReply::NetworkError)));
}


/*
 *Attempts to authenticate the user
 */
void NJsonRpc::authenticate(const QString &login, const QString &password, bool autoLogin,qint32 appType, QJSValue callBackFunction){
    jsFunctionAuthenticate=callBackFunction;
    setUserTempKey(QByteArray());
    QString realLogin(login);
    realLogin=realLogin.toLower();
    QByteArray passwordHash=QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5).toHex();
    mobileSeed= QUuid::createUuid().toString().remove('{').remove('}');
    QString macAddresses="";
    call("SUsuarios","Authenticate",this,SLOT(slotRespAuthenticate(QJsonRpcMessage)),realLogin,passwordHash,appType,"1.0.0", macAddresses, mobileSeed);
}
void NJsonRpc::slotRespAuthenticate(const QJsonRpcMessage &resp){
    QString json=resp.result().toString();
    BLogger::debug(this,__FUNCTION__,json);
    QJSValue jsValue;
    ELoginResponse *e=new ELoginResponse();
    if(!JSON::convertJsonDocToObject(json,*e))return;
    CTezine::obj()->setLoginResponse(e);
    serverSeed=e->getServerSeed();
    userTempKey=QCryptographicHash::hash(QString(serverSeed+QString("skq1sqoz#><29203h1#$^@(%")+mobileSeed).toUtf8(),QCryptographicHash::Md5).toHex();
    JSON::convertObjectToJSValue((QQmlApplicationEngine*)CQML::obj()->getQMLEngine(),e,jsValue,true);
    jsFunctionAuthenticate.call(QJSValueList()<<jsValue);
}


//======================================INTERNAL FUNCTIONS BELOW============================================
/*
 *Adds a new receiver object
 */
void NJsonRpc::addReceiver(QObject *objReceiver,quint64 id, const char *slot){
	STRReceiver *receiver=new STRReceiver();
	receiver->isCallBack=false;
	receiver->obj=objReceiver;
	QRegExp rx("\\(.*");
	rx.setMinimal(false);
	QString name(slot);
	if(name.isEmpty())receiver->slotName=QByteArray();
	else{
		name.remove(rx);
		receiver->slotName=QMetaObject::normalizedSignature(name.remove(0,1).toUtf8().constData());
	}
	receiverHash[id]=receiver;
}


/*
 * It will invoke a javascript function when receives the reply from server
 */
void NJsonRpc::addCallBackReceiver(QJSValue callBackFunction, const QString &completeServerFunction, quint64 msgID){
	STRReceiver *receiver=new STRReceiver();
	receiver->isCallBack=true;
	receiver->completServerFunction=completeServerFunction;
	receiver->callBackFunction=callBackFunction;
	receiverHash[msgID]=receiver;
}


/*
 *After received the entire reply from the json webservice
 */
void NJsonRpc::slotReplyFinished(){
	QJsonRpcServiceReply *reply = static_cast<QJsonRpcServiceReply *>(sender());
	if (!reply) {BLogger::error(this,__FUNCTION__,"invalid reply received");return;}
	QJsonRpcMessage resp= reply->response();
	if(!resp.isValid()){BLogger::error(this,__FUNCTION__,"invalid response for request");return;}
	if(!receiverHash.contains(resp.id())){BLogger::error(this,__FUNCTION__,"msg id "+QString::number(resp.id())+" not found in hash");return;}
	if(resp.result()=="userTempKeyExpired"){
		emit sigSessionExpired();
        setUserTempKey(QByteArray());
		return;
	}
	STRReceiver *strReceiver=receiverHash[resp.id()];
	if(strReceiver->isCallBack){invokeCallBack(strReceiver->callBackFunction,strReceiver->completServerFunction, resp.result());receiverHash.remove(resp.id());return;}
	if(strReceiver->obj==0 || strReceiver->slotName.isEmpty())return;
	if(!strReceiver->obj->metaObject()->invokeMethod(strReceiver->obj,strReceiver->slotName.constData(),Q_ARG(QJsonRpcMessage, resp)))BLogger::warning(0,__FUNCTION__,"Unable to invoke:"+QString(strReceiver->slotName));
	receiverHash.remove(resp.id());
	//todo fazer delete later do strreceiver;
}


/*
 *
 */
void NJsonRpc::slotNetworkReplyError(QNetworkReply::NetworkError error){
	if(error==QNetworkReply::OperationCanceledError)return;//cancelled by user
	QString errorName;
	switch(error){
		case QNetworkReply::HostNotFoundError:
            errorName=tr("Host not found");
			break;
		case QNetworkReply::ContentNotFoundError:
            errorName=tr("Content not found");
			break;
		case QNetworkReply::ConnectionRefusedError:
            errorName=tr("Connection refused");
			break;
		case QNetworkReply::TimeoutError:
            errorName=tr("Timeout");
			break;
		case QNetworkReply::ContentAccessDenied:
            errorName=tr("Content access denied");
			break;
		case QNetworkReply::SslHandshakeFailedError:
            errorName=tr("Ssl handshake failed");
			break;
		case QNetworkReply::RemoteHostClosedError:
            errorName=tr("Remote host closed");
			break;
		case QNetworkReply::UnknownServerError:
            errorName=tr("Unknown server error");
			break;
		case QNetworkReply::UnknownNetworkError:
            errorName=tr("Unknown network error");
			break;
		default:
			errorName=CEnums::getNameFromEnumValue(QNetworkReply::staticMetaObject,"NetworkError",error);
			break;
	}
	BLogger::error(this,__FUNCTION__,"An error occurred: "+errorName);
	emit sigNetworkError(errorName);
}


/*
 *We only display percentage if file is bigger than 1mb
 */
void NJsonRpc::slotUploadProgress(qint64 bytesSent, qint64 bytesTotal){
	if(bytesTotal<1000)return;
	int percentage=100*bytesSent/bytesTotal;
	emit sigProgress(true,percentage);
	//qDebug()<<"slot upload:"<<bytesSent<<bytesTotal;
}


/*
 *Invokes the javascript function from the response received
 */
void NJsonRpc::invokeCallBack(QJSValue callBackFunction, const QString &completeServerFunction, const QJsonValue &jValue){
    emit sigProgress(false);
	qDebug()<<"<--("+completeServerFunction+")" <<jValue;
    EMetaResponse eMetaResponse;
	JSON::convertJsonValueToObject(eMetaResponse,jValue);
	if(eMetaResponse.getClassName()=="Error"){
		if(eMetaResponse.getValue().toString()=="userTempKeyExpired"){emit sigSessionExpired();setUserTempKey(QByteArray()); return;}
		else {qDebug()<<"received error:"<<eMetaResponse.getValue().toString();return;}
	}
	if(jValue.isNull()){callBackFunction.call(QJSValueList()<<QJSValue());return;}//we send a null value as argument
    //qDebug()<<"ClassName:"<<eMetaResponse.getClassName()<<".ResponseType:"<<eMetaResponse.getResponseType();
	switch(eMetaResponse.getResponseType()){
        case ResponseTypeObject:{
            BLogger::debug(this,__FUNCTION__,"recebeu objeto da classe "+eMetaResponse.getClassName());
            QJsonDocument jDoc;
            jDoc.setObject(eMetaResponse.getValue().toObject());
            callBackFunction.call(QJSValueList()<<QJSValue(QString(jDoc.toJson(QJsonDocument::Indented))));
            break;
		}
        case ResponseTypeArray:{
            BLogger::debug(this,__FUNCTION__,"recebeu array:"+eMetaResponse.getClassName());
			QJsonArray jArray= eMetaResponse.getValue().toArray();
            QJsonDocument jDoc;
            jDoc.setArray(jArray);
            QString content=QString(jDoc.toJson(QJsonDocument::Indented));
            callBackFunction.call(QJSValueList()<<QJSValue(content));
			break;
		}
        case ResponseTypeValue:{
			QJsonValue jValue=eMetaResponse.getValue();
            BLogger::debug(this,__FUNCTION__,"recebeu value:"+jValue.toString());
			//qDebug()<<"recebeu value:"<<jValue;
			if(jValue.isBool())callBackFunction.call(QJSValueList()<<QJSValue(jValue.toBool()));
			else if(jValue.isDouble())callBackFunction.call(QJSValueList()<<QJSValue(jValue.toDouble()));
			else if(jValue.isString())callBackFunction.call(QJSValueList()<<QJSValue(jValue.toString()));
			else if(jValue.isNull())callBackFunction.call(QJSValueList()<<QJSValue());
			break;
		}
    }
}
