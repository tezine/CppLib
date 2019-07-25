#include <QtCore>
#include "csingletonperthread.h"

QThreadStorage<QCache<QString, QObject> > caches;

CSingletonPerThread::CSingletonPerThread(){
}

void CSingletonPerThread::addObject(QObject *object){
	//qDebug()<<"fazendo cache:"<<object->metaObject()->className()<<".thread:"<<object->thread();
	caches.localData().insert(QString(object->metaObject()->className()), object);
}

bool CSingletonPerThread::containsObject(const QString &className){
	if(caches.localData().contains(className))return true;
	else return false;
}

QObject* CSingletonPerThread::getObject(const QString &className){
	QObject *obj= caches.localData().object(className);
	//qDebug()<<"retornando object:"<<className<<".thread:"<<obj->thread();
	return obj;
}
