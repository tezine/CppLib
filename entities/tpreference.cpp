#include <QtCore>
#include "blogger.h"
#include "tpreference.h"

TPreference::TPreference(QObject *parent):QObject(parent){
	id=0;
}

QMetaObject TPreference::getMeta(){
	if(QMetaType::type("TPreference")==0){
		qRegisterMetaType<TPreference>();
		qRegisterMetaType<TPreference*>();
		qRegisterMetaType<QList<TPreference*> >();
		qRegisterMetaType<QList<TPreference> >();
	}
	return TPreference::staticMetaObject;
}

