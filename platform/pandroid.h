#ifndef PANDROID_H
#define PANDROID_H
#include <QPointer>
#include <QVariant>
#include "cpplib_global.h"
#ifdef Q_OS_ANDROID
    #include "jni.h"
#endif

class QAndroidJniObject;


/**
 *@name PAndroid
 *@brief  The PAndroid class provides a bridge between C++ and Android's Java
 */
class CPPLIB_EXPORT  PAndroid: public QObject{
	Q_OBJECT

public:
	static PAndroid *obj(){if(!staticObj)staticObj=new PAndroid();return staticObj;}
    void emitAndroidToken(const QString &token){emit sigAndroidRegistrationIDReceived(token);}
    void shareText(const QString &txt);
    static QVariant callStaticObjectMethod(const QString &completeClassPath, const QString &methodName, const QString &returnTypeName, const QVariantList &argsList);
    static QVariant callStaticMethod(qint32 returnBaseType, const QString &completeClassPath, const QString &methodName, const QVariant &arg1=QVariant(), const QVariant &arg2=QVariant(), const QVariant &arg3=QVariant(),
                                           const QVariant &arg4=QVariant(), const QVariant &arg5=QVariant(), const QVariant &arg6=QVariant(), const QVariant &arg7=QVariant(), const QVariant &arg8=QVariant(),
                                           const QVariant &arg9=QVariant(), const QVariant &arg10=QVariant());
    QString getCAndroidPath(){return cAndroidPath;}
    void setup(const QString &androidLibInterjavaPath, const QString &completePackageName,  const QString &cAndroidPath);
    QString getCompletePackageName() const{return completePackageName;}
    QString getAndroidLibInterJavaPath() const {return androidLibInterJavaPath;}

signals:
    void sigAndroidRegistrationIDReceived(const QString &token);

protected:
    void registerNativeMethods();
#ifdef Q_OS_ANDROID
    static QVariant getQVariant(jobject obj);
#endif
    static QString getArgumentTypes(const QVariantList &vList);
    static bool getJavaValue(const QVariant &v, QAndroidJniObject &outObj);
    Q_INVOKABLE bool hasException();
    Q_INVOKABLE void clearException();
    static bool addVariantArg(int argPos, const QVariant &d, QVariantList &vList);
    static bool getReturnTypeName(qint64 fileID, QString &outReturnTypeName);
    static QVariant getResult(const QAndroidJniObject &obj);
    QString cAndroidPath;
    QString completePackageName;
    QString androidLibInterJavaPath;

private:
	PAndroid();
    static QPointer<PAndroid> staticObj;
};
#endif
