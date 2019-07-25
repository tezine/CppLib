#include "cqml.h"
#include "firebase.h"
#include "enhancedqmlapplicationengine.h"
#include <QDebug>
#include "blogger.h"
#if defined(FIREBASEANDROID) //|| defined(Q_OS_IOS)
    #include "fblistener.h"
#endif

QPointer<FireBase> FireBase::o = nullptr;
FireBase::FireBase(QObject *parent) : QObject(parent){
#if defined(FIREBASEANDROID) //|| defined(Q_OS_IOS)//nao usamos mais no ios
    qDebug() << "(FireBase)Constructor";
    fbListener = new FBListener();
#endif
}

void FireBase::emitMessageReceived(QJSValue jsValue){
    emit messageReceived(jsValue);

//     QJSValue myObject = CQML::obj()->getQMLEngine()->newObject();
//     myObject.setProperty("superObj",qmlObject);
    emit messageReceivedWithObject(qmlObject);


}

void FireBase::initializeMessaging(){
#if defined(FIREBASEANDROID)// || defined(Q_OS_IOS)//nao usamos mais no ios
    qDebug() << "(FireBase)initializeMessaging";
    fbListener->InitializeMessaging();
#endif
}


void FireBase::setValue(const QString &key, const QString &value){
    valueHash[key]=value;
}

QString FireBase::getValue(const QString &key){
    if(!valueHash.contains(key)){BLogger::debug(this,__FUNCTION__,"key not found in hash:"+key);return QString();}
    return valueHash[key];
}

bool FireBase::containsKey(const QString &key){
    if(!valueHash.contains(key))return false;
    return true;
}
