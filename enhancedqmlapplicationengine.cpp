#include <QDebug>
#include "enhancedqmlapplicationengine.h"

EnhancedQmlApplicationEngine::EnhancedQmlApplicationEngine(QObject *parent): QQmlApplicationEngine(parent){
    connect(this,&QQmlApplicationEngine::warnings,this,&EnhancedQmlApplicationEngine::slotWarnings);
    connect(this,&QQmlApplicationEngine::objectCreated,this,&EnhancedQmlApplicationEngine::sloObjectCreated);
}

void EnhancedQmlApplicationEngine::clearCache(){
    this->trimComponentCache();
    this->clearComponentCache();
    this->trimComponentCache();
}


void EnhancedQmlApplicationEngine::slotWarnings(const QList<QQmlError> &warnings){
    foreach(QQmlError error, warnings){
        qDebug()<<"error:"<<error.url()<<".Line:"<<error.line()<<".Column:"<<error.column()<<".Description:"<<error.description()<<".toString:"<<error.toString();
    }
}

void EnhancedQmlApplicationEngine::sloObjectCreated(QObject *object, const QUrl &url){//nao serve para muita coisa, pq só é chamado ao invocar engine->load
    QString objectClassName;
    if(object)objectClassName=object->metaObject()->className();
    //qDebug()<<"(EnhancedQmlApplicationEngine)slotObjectCreated:"<<objectClassName<<url;
}
