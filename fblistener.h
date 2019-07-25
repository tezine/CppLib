#ifndef FBLISTENER_H
#define FBLISTENER_H
#include "cpplib_global.h"


#ifdef FIREBASEANDROID
#include <firebase/messaging.h>
#include "firebase/app.h"
#include "firebase/util.h"

class QAndroidJniEnvironment;

class CPPLIB_EXPORT FBListener : public ::firebase::messaging::Listener{

public:
    FBListener();
    void InitializeMessaging();
    virtual void OnTokenReceived(const char *token);
    virtual void OnMessage(const ::firebase::messaging::Message & message);

private:
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment *_jniEnv;
#endif
    ::firebase::App* _app;
    ::firebase::ModuleInitializer _initializer;

};
#endif
#endif
