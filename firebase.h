#ifndef FIREBASE_H
#define FIREBASE_H
#include <QPointer>
#include <QJSValue>
#include "cpplib_global.h"

class FBListener;

class CPPLIB_EXPORT FireBase : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

public:
    static FireBase* obj(){if (!o) o = new FireBase();return o;}
    void emitMessageReceived(QJSValue jsValue);
    void initializeMessaging();
    QString token() const{return m_token;}
    void setValue(const QString &name, const QString & value);
    Q_INVOKABLE QString getValue(const QString &key);
    Q_INVOKABLE bool containsKey(const QString &key);
    Q_INVOKABLE void clearHash(){valueHash.clear();}
    Q_INVOKABLE void setQmlObject(QJSValue jsValue){this->qmlObject=jsValue;}

#if defined(Q_OS_ANDROID) //|| defined(Q_OS_IOS)
    // Ideally, this would be some base class and we can return an
    // implementation subclass. But, we really want to keep this minimal
    FBListener* GetListener() const {return fbListener;}
#endif


public slots:
    void setToken(QString token){if (m_token == token)return;m_token = token;emit tokenChanged(m_token);}

signals:
    void tokenReceived(const QString &token);
    void messageReceived(QJSValue message);//é um map<QString,QString>    
    void messageReceivedWithObject(QJSValue superObj);
    void tokenChanged(QString token);//nao está funcionando para receber esse evento no typescript. só funciona no qml

private:
    explicit FireBase(QObject *parent = 0);
    static QPointer<FireBase> o;

#if defined(Q_OS_ANDROID) //|| defined(Q_OS_IOS)
    FBListener *fbListener;
#endif
    QString m_token;
    QJSValue qmlObject;
    QHash<QString,QString> valueHash;
};
#endif
