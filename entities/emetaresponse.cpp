#include <QtCore>
#include "blogger.h"
#include "emetaresponse.h"

EMetaResponse::EMetaResponse(QObject *parent):QObject(parent){
	responseType=0;
}

QMetaObject EMetaResponse::getMeta(){
	if(QMetaType::type("EMetaResponse")==0){
		qRegisterMetaType<EMetaResponse>();
		qRegisterMetaType<EMetaResponse*>();
		qRegisterMetaType<QList<EMetaResponse*> >();
		qRegisterMetaType<QList<EMetaResponse> >();
	}
	return EMetaResponse::staticMetaObject;
}

