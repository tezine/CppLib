#include <QtCore>
#include "blogger.h"
#include "efirstsvcarg.h"

EFirstSVCArg::EFirstSVCArg(QObject *parent):QObject(parent){
	appType=0;
	argsCount=0;
}

QMetaObject EFirstSVCArg::getMeta(){
	if(QMetaType::type("EFirstSVCArg")==0){
		qRegisterMetaType<EFirstSVCArg>();
		qRegisterMetaType<EFirstSVCArg*>();
		qRegisterMetaType<QList<EFirstSVCArg*> >();
		qRegisterMetaType<QList<EFirstSVCArg> >();
	}
	return EFirstSVCArg::staticMetaObject;
}

