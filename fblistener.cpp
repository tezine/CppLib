#include "fblistener.h"
#include "firebase.h"
#ifdef FIREBASEANDROID
    #include <QtAndroid>
    #include <QAndroidJniEnvironment>
    #include <QAndroidJniObject>
#endif
#include <QDebug>
#include "cqml.h"
#include "enhancedqmlapplicationengine.h"

#ifdef FIREBASEANDROID
FBListener::FBListener(){
#ifdef Q_OS_ANDROID
    _jniEnv = new QAndroidJniEnvironment();
    QAndroidJniObject jniObject = QtAndroid::androidActivity();
#endif
    ::firebase::App *instance = ::firebase::App::GetInstance();
    if (instance) {
        //qDebug() << "App instance already exists";
        _app = instance;
    } else {
        qDebug() << "(FBListener)Creating firebase instance...";
        #ifdef Q_OS_ANDROID
            _app = ::firebase::App::Create(*_jniEnv, jniObject.object<jobject>());
        #endif
        #ifdef Q_OS_IOS
            _app = ::firebase::App::Create();
        #endif
    }
}

static ::firebase::InitResult _InitializeMessaging(::firebase::App *app, void *) {
    qDebug() << "(FBListener)Trying to initialize Firebase Messaging...";
    FireBase *handler = FireBase::obj();
    return ::firebase::messaging::Initialize(*app, handler->GetListener());
}

void FBListener::InitializeMessaging(){
//    qDebug() << "(FBListener)InitializeMessaging";
    _initializer.Initialize(_app, nullptr,_InitializeMessaging);
    qDebug() << "(FBListener) Module initialized. Waiting on messaging initialization";
}

void FBListener::OnTokenReceived(const char *token){
    // I guess this is null terminated?
    qDebug() << "(FBListener)Token received: [" << token << "]";
    FireBase::obj()->setToken(QString(token));
}

//se a msg é enviada com notification, ao clicar no ícone do app que está rodando em background após chegar uma msg, essa funcao nao é executada.
//assim, enviamos uma msg sem notification e criamos o notification manualmente no código java.
void FBListener::OnMessage(const ::firebase::messaging::Message & message){
    qDebug() << "(FBListener)Received message";
//    qDebug()<<"from:"<< QString::fromStdString(message.from);
//    qDebug()<<"msgID:"<<QString::fromStdString(message.message_id);
//    qDebug()<<"error:"<<QString::fromStdString(message.error);
//    qDebug()<<"error description:"<<QString::fromStdString(message.error_description);
    QMap<std::string, std::string> mapData(message.data);
    EnhancedQmlApplicationEngine *engine= CQML::obj()->getQMLEngine();
    QJSValue jsObject= engine->newObject();
    QMapIterator<std::string, std::string> i(mapData);
      while (i.hasNext()) {
          i.next();
          qDebug() <<"(FBListener)OnMessage: map value:"<< QString::fromStdString(i.key()) << ": " << QString::fromStdString(i.value()) << endl;
          QString key=QString::fromStdString(i.key());
          QString value=QString::fromStdString(i.value());
          jsObject.setProperty(key, value);
          FireBase::obj()->setValue(key,value);
      }
      FireBase::obj()->emitMessageReceived(jsObject);
//      qDebug()<<"raw data:"<<QString::fromStdString(message.raw_data);//chega vazio
//      qDebug()<<"to:"<<QString::fromStdString(message.to);
      //if(message.notification==nullptr){qDebug()<<"sem notification";return;}//notification chega vazio quando em background
//      qDebug()<<"title:"<< QString::fromStdString(message.notification->title);
//      qDebug()<<"badge:"<< QString::fromStdString(message.notification->badge);
//      qDebug()<<"body:"<< QString::fromStdString(message.notification->body);
//      qDebug()<<"tag:"<< QString::fromStdString(message.notification->tag);
}

#endif
