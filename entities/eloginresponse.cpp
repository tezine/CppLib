#include <QtCore>
#include "blogger.h"
#include "eloginresponse.h"

ELoginResponse::ELoginResponse(QObject *parent):QObject(parent){
	userID=0;
	cityID=0;
	agreementVersion=0;
	respCreateUser=0;
}

QMetaObject ELoginResponse::getMeta(){
	if(QMetaType::type("ELoginResponse")==0){
		qRegisterMetaType<ELoginResponse>();
		qRegisterMetaType<ELoginResponse*>();
		qRegisterMetaType<QList<ELoginResponse*> >();
		qRegisterMetaType<QList<ELoginResponse> >();
	}
	return ELoginResponse::staticMetaObject;
}

