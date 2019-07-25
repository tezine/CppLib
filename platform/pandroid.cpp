#include <QDebug>
#include "pandroid.h"
#include "cbase.h"
#include "blogger.h"
#ifdef Q_OS_ANDROID
	#include <QAndroidJniObject>
	#include <QAndroidJniEnvironment>	
#endif

QPointer<PAndroid> PAndroid::staticObj=0;
PAndroid::PAndroid(): QObject(){
}


/*
 *Shares a text with another app
 */
void PAndroid::shareText(const QString &txt){
    callStaticMethod(CBase::BasicTypeString,androidLibInterJavaPath, "shareText",txt);
}


/**
 * @brief This function must be called prior to use anything on android.
 * @param androidLibInterjavaPath: ex: "tezine/com/androidlib/JInterJava"
 * @param completePackageName //ex: "com/tezine/copagazcaminhoneiro"
 * @param cAndroidPath: //ex: com/tezine/copagazcaminhoneiro/codes/CAndroid
 */
void PAndroid::setup(const QString &androidLibInterjavaPath, const QString &completePackageName, const QString &cAndroidPath){
    this->androidLibInterJavaPath=androidLibInterjavaPath;
    this->completePackageName=completePackageName;
    this->cAndroidPath=cAndroidPath;
    registerNativeMethods();
}

/*
 *Calls a Static Object Java method
 */
QVariant PAndroid::callStaticObjectMethod(const QString &completeClassPath, const QString &methodName, const QString &returnTypeName, const QVariantList &argList){
#ifdef Q_OS_ANDROID
    QString argTypes=getArgumentTypes(argList);
    argTypes.append(returnTypeName);
    QAndroidJniObject jArg1, jArg2, jArg3, jArg4, jArg5, jArg6, jArg7, jArg8, jArg9, jArg10;
    if(argList.size()>=1)if(!getJavaValue(argList.at(0),jArg1)){BLogger::error(0,__FUNCTION__,"invalid arg1 value");}
    if(argList.size()>=2)if(!getJavaValue(argList.at(1),jArg2)){BLogger::error(0,__FUNCTION__,"invalid arg2 value");}
    if(argList.size()>=3)if(!getJavaValue(argList.at(2),jArg3)){BLogger::error(0,__FUNCTION__,"invalid arg3 value");}
    if(argList.size()>=4)if(!getJavaValue(argList.at(3),jArg4)){BLogger::error(0,__FUNCTION__,"invalid arg4 value");}
    if(argList.size()>=5)if(!getJavaValue(argList.at(4),jArg5)){BLogger::error(0,__FUNCTION__,"invalid arg5 value");}
    if(argList.size()>=6)if(!getJavaValue(argList.at(5),jArg6)){BLogger::error(0,__FUNCTION__,"invalid arg6 value");}
    if(argList.size()>=7)if(!getJavaValue(argList.at(6),jArg7)){BLogger::error(0,__FUNCTION__,"invalid arg7 value");}
    if(argList.size()>=8)if(!getJavaValue(argList.at(7),jArg8)){BLogger::error(0,__FUNCTION__,"invalid arg8 value");}
    if(argList.size()>=9)if(!getJavaValue(argList.at(8),jArg9)){BLogger::error(0,__FUNCTION__,"invalid arg9 value");}
    if(argList.size()>=10)if(!getJavaValue(argList.at(9),jArg10)){BLogger::error(0,__FUNCTION__,"invalid arg10 value");}
    QAndroidJniObject obj;
    switch(argList.size()){
        case 0:
            obj= QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData());
            break;
        case 1:
            //qDebug()<<"vai chamar:"<<completeClassName<<methodName<<argTypes;
            obj= QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object());
            break;
        case 2:
            qDebug()<<"vai chamar2 :"<<completeClassPath<<methodName<<argTypes;
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object());
            break;
        case 3:
            qDebug()<<"vai chamar3 :"<<completeClassPath<<methodName<<argTypes;
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object());
            break;
        case 4:
            qDebug()<<"vai chamar4 :"<<completeClassPath<<methodName<<argTypes;
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object());
            break;
        case 5:
            qDebug()<<"vai chamar5 :"<<completeClassPath<<methodName<<argTypes;
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object());
            break;
        case 6:
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object(),jArg6.object());
            break;
        case 7:
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object(),jArg6.object(),jArg7.object());
            break;
        case 8:
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object(),jArg6.object(),jArg7.object(),jArg8.object());
            break;
        case 9:
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object(),jArg6.object(),jArg7.object(),jArg8.object(),jArg9.object());
            break;
        case 10:
            obj=QAndroidJniObject::callStaticObjectMethod(completeClassPath.toUtf8().constData(),methodName.toUtf8().constData(), argTypes.toUtf8().constData(),jArg1.object(), jArg2.object(),jArg3.object(),jArg4.object(),jArg5.object(),jArg6.object(),jArg7.object(),jArg8.object(),jArg9.object(),jArg10.object());
        break;
    }
    //todo ver o q fazer com o obj
    //qDebug()<<"vai verificar o resultado";
    return getResult(obj);
#else
    Q_UNUSED(completeClassPath)
    Q_UNUSED(methodName)
    Q_UNUSED(returnTypeName)
    Q_UNUSED(argList)
#endif
    return QVariant();
}


/*
 * Calls the static java method
 * Se o metodo nao for encontrado por qualquer razao (nro de argumentos diferente, nome inexistente, etc), o app trava e indica unable to ...GetObjectClass
 */
QVariant PAndroid::callStaticMethod(qint32 returnBaseType, const QString &completeClassPath, const QString &methodName, const QVariant &arg1,
                                     const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6, const QVariant &arg7,
                                     const QVariant &arg8, const QVariant &arg9, const QVariant &arg10){
#ifdef Q_OS_ANDROID
    QString returnTypeName;
    if(!getReturnTypeName(returnBaseType,returnTypeName)){BLogger::error(nullptr,__FUNCTION__,"Invalid return type name");return QVariant();}
    QVariantList argList;
    if(arg1.isValid())if(!addVariantArg(1,arg1,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg1");return QVariant();}
    if(arg2.isValid())if(!addVariantArg(2,arg2,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg2");return QVariant();}
    if(arg3.isValid())if(!addVariantArg(3,arg3,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg3");return QVariant();}
    if(arg4.isValid())if(!addVariantArg(4,arg4,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg4");return QVariant();}
    if(arg5.isValid())if(!addVariantArg(5,arg5,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg5");return QVariant();}
    if(arg6.isValid())if(!addVariantArg(6,arg6,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg6");return QVariant();}
    if(arg7.isValid())if(!addVariantArg(7,arg7,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg7");return QVariant();}
    if(arg8.isValid())if(!addVariantArg(8,arg8,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg8");return QVariant();}
    if(arg9.isValid())if(!addVariantArg(9,arg9,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg9");return QVariant();}
    if(arg10.isValid())if(!addVariantArg(10,arg10,argList)){BLogger::error(nullptr,__FUNCTION__,"Invalid arg10");return QVariant();}
    QVariant result= callStaticObjectMethod(completeClassPath,methodName,returnTypeName,argList);
    return result;
#else
    Q_UNUSED(returnBaseType)
    Q_UNUSED(completeClassPath)
    Q_UNUSED(methodName)
    Q_UNUSED(arg1)
    Q_UNUSED(arg2)
    Q_UNUSED(arg3)
    Q_UNUSED(arg4)
    Q_UNUSED(arg5)
    Q_UNUSED(arg6)
    Q_UNUSED(arg7)
    Q_UNUSED(arg8)
    Q_UNUSED(arg9)
    Q_UNUSED(arg10)
    return QVariant();
#endif
}


/*
 *Returns the Return Type Name of the java method
 * Ex: String getName() -> returnTypeName= Ljava/lang/String;
 */
bool PAndroid::getReturnTypeName(qint64 fileID, QString &outReturnTypeName){
#ifdef Q_OS_ANDROID
    switch(fileID){
        case CBase::BasicTypeString:
            outReturnTypeName="Ljava/lang/String;";
            break;
        case CBase::BasicTypeInt32:
            outReturnTypeName="Ljava/lang/Integer;";
            break;
        case CBase::BasicTypeInt64:
            outReturnTypeName="Ljava/lang/Long;";
            break;
        case CBase::BasicTypeBoolean:
            outReturnTypeName="Ljava/lang/Boolean;";
            break;
        case CBase::BasicTypeFloat:
            outReturnTypeName="Ljava/lang/Float;";
            break;
        case CBase::BasicTypeDouble:
            outReturnTypeName="Ljava/lang/Double;";
            break;
        case CBase::BasicTypeChar:
            outReturnTypeName="Ljava/lang/Character;";
            break;
        case CBase::BasicTypeStringList:
            outReturnTypeName="[Ljava/lang/String;";
            break;
        default:
            BLogger::error(nullptr,__FUNCTION__,"Invalid return type");
            return false;
    }
#endif
    return true;
}


/*
 *Adds the variant into the list of arguments to be sent to Java
 * Only basic datatypes are supported (QString, int...)
 */
bool PAndroid::addVariantArg(int argPos,const QVariant &d, QVariantList &vList){
    if(d.userType()==QMetaType::QString){vList.append(d);return true;}
    else if(d.userType()==QMetaType::Int){vList.append(d);return true;}
    else if(d.userType()==QMetaType::LongLong){vList.append(d);return true;}
    else if(d.userType()==QMetaType::Bool){vList.append(d);return true;}
    else if(d.userType()==QMetaType::Float){vList.append(d);return true;}
    else if(d.userType()==QMetaType::Double){vList.append(d);return true;}
    else if(d.userType()==QMetaType::Char){vList.append(d);return true;}
    else if(d.userType()==QMetaType::QStringList){vList.append(d);return true;}
    else BLogger::error(nullptr,__FUNCTION__,"Invalid variant value type for argument "+QString::number(argPos));
    return false;
}

/*
 *Returns the result from java method as a DVariant
 */
QVariant PAndroid::getResult(const QAndroidJniObject &obj){
#ifdef Q_OS_ANDROID
    QVariant v=getQVariant(obj.object());
    return v;
#endif
    return QVariant();
}


bool PAndroid::hasException(){
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment env;
    return (bool)env->ExceptionCheck();
#endif
    return false;
}


void PAndroid::clearException(){
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment env;
     env->ExceptionClear();
#endif
}

//============================================JAVA TO C++ BELOW ==========================================================
#ifdef Q_OS_ANDROID
bool getBoolFromJObject(jobject obj){
    QAndroidJniObject x(obj);
    if(x.toString()=="true")return true;
    else return false;
}

qint32 getInt32FromJObject(jobject obj){
    QAndroidJniObject x(obj);
    return x.toString().toInt();
}

qint64 getInt64FromJObject(jobject obj){
    QAndroidJniObject x(obj);
    return x.toString().toLongLong();
}

float getFloatFromJObject(jobject obj){
    QAndroidJniObject x(obj);
    return x.toString().toFloat();
}

double getDoubleFromJObject(jobject obj){
    QAndroidJniObject x(obj);
    return x.toString().toDouble();
}

QChar getQCharFromJObject(jobject obj){
    QAndroidJniObject x(obj);
    QString str=x.toString();
    if(str.length()==0)return QChar();
    else return str.at(0);
}

QStringList getStringListFromJObject(jobject obj){
    QAndroidJniObject result=QAndroidJniObject::callStaticObjectMethod("java/util/Arrays","toString","([Ljava/lang/Object;)Ljava/lang/String;",obj);
    return result.toString().split(",");
}

/*
 *Returns the jobject's className
 */
QVariant getJObjectVariant(JNIEnv *env,jobject obj){
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "getClass", "()Ljava/lang/Class;");
    jobject clsObj = env->CallObjectMethod(obj, mid);
    cls = env->GetObjectClass(clsObj);
    mid = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    jstring strObj = (jstring)env->CallObjectMethod(clsObj, mid);
    const char* str = env->GetStringUTFChars(strObj, NULL);
    QString className(str);
    QVariant v;
    //qDebug()<<"Calling class is"<< str;
    if(className=="java.lang.String")v=QAndroidJniObject(obj).toString();
    else if(className=="java.lang.Integer")v=QVariant::fromValue(getInt32FromJObject(obj));
    else if(className=="java.lang.Long")v=QVariant::fromValue(getInt64FromJObject(obj));
    else if(className=="java.lang.Boolean")v=QVariant::fromValue(getBoolFromJObject(obj));
    else if(className=="java.lang.Float")v=QVariant::fromValue(getFloatFromJObject(obj));
    else if(className=="java.lang.Double")v=QVariant::fromValue(getDoubleFromJObject(obj));
    else if(className=="java.lang.Character")v=QVariant::fromValue(getQCharFromJObject(obj));
    else if(className=="[Ljava.lang.String;")v=QVariant::fromValue(getStringListFromJObject(obj));
    else qDebug()<<"(getJObjectVariant)unknown jobject type";
    env->ReleaseStringUTFChars(strObj, str);
    return v;
}

/*
 *Remember that this function is called from the JAVA thread.
 * Nao podemos chamar uma funcao em C++ diretamente
 */
static void fromCallWikiosStaticFunction(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    QAndroidJniObject objClassName(className);
    QAndroidJniObject objFunctionName(staticFunctionName);
    //qDebug()<<"chamou fromCallWikiosStaticFunction"<<objClassName.toString()<<objFunctionName.toString();
    QVariant v1=getJObjectVariant(env,arg1);
    if(objClassName.toString()=="PAndroid" && objFunctionName.toString()=="emitAndroidToken"){
        PAndroid::obj()->emitAndroidToken(v1.toString());
    }
}


static void fromCallWikiosStaticFunction2(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    QAndroidJniObject objClassName(className);
    QAndroidJniObject objFunctionName(staticFunctionName);
    qDebug()<<"chamou fromCallWikiosStaticFunction2"<<objClassName.toString()<<objFunctionName.toString();
}


static void fromCallWikiosStaticFunction3(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}


static void fromCallWikiosStaticFunction4(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction5(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction6(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5, jobject arg6){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction7(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5, jobject arg6, jobject arg7){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction8(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5, jobject arg6, jobject arg7, jobject arg8){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction9(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5, jobject arg6, jobject arg7, jobject arg8, jobject arg9){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromCallWikiosStaticFunction10(JNIEnv *env, jobject thiz, jstring className, jstring staticFunctionName, jobject arg1, jobject arg2, jobject arg3, jobject arg4, jobject arg5, jobject arg6, jobject arg7, jobject arg8, jobject arg9, jobject arg10){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
}

static void fromJavaOne(JNIEnv *env, jobject thiz, jint x){
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    qDebug() << x << "< 100";
}
#endif
//===========================================C++ TO JAVA BELOW ==========================================================

/**
 * @brief Registers all jni functions
 */
void registerNative() {
#ifdef Q_OS_ANDROID
    JNINativeMethod methods[] {{"callNativeOne", "(I)V", reinterpret_cast<void *>(fromJavaOne)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction)},
                               {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction2)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction3)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction4)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction5)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction6)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction7)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction8)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction9)},
                                {"callWikiosStaticFunction", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V", reinterpret_cast<void *>(fromCallWikiosStaticFunction10)}};

    if(PAndroid::obj()->getCompletePackageName().isEmpty()){BLogger::error(0,__FUNCTION__,"package name not defined");return;}
    QString cAndroidPath=PAndroid::obj()->getCAndroidPath();
    QAndroidJniObject cAndroidJavaClass(cAndroidPath.toUtf8().constData());
    QAndroidJniEnvironment env;
    jclass cAndroidObjClass = env->GetObjectClass(cAndroidJavaClass.object<jobject>());
    if(!cAndroidObjClass){BLogger::error(0,__FUNCTION__,"CAndroid not found");}
    env->RegisterNatives(cAndroidObjClass,methods,sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(cAndroidObjClass);
    //Now for AndroidLib JInterJava
    QAndroidJniObject jInterJavaClass(PAndroid::obj()->getAndroidLibInterJavaPath().toUtf8().constData());
    jclass jInterJavaObjClass = env->GetObjectClass(jInterJavaClass.object<jobject>());
    if(!jInterJavaObjClass){BLogger::error(0,__FUNCTION__,"InterJava not found");}
    env->RegisterNatives(jInterJavaObjClass,methods,sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(jInterJavaObjClass);
#endif
}

/*
 *Returns all argument types used to call java functions
 */
void PAndroid::registerNativeMethods(){
    registerNative();
}


QString PAndroid::getArgumentTypes(const QVariantList &vList){
    QString result("(");
    for(int i=0;i<vList.size();i++){
        QVariant v=vList.at(i);
        switch(v.userType()){
            case QMetaType::QString:
                result.append("Ljava/lang/String;");
                break;
            case QMetaType::Int:
                result.append("Ljava/lang/Integer;");
                break;
            case QMetaType::LongLong:
                result.append("Ljava/lang/Long;");
                break;
            case QMetaType::Bool:
                result.append("Ljava/lang/Boolean;");
                break;
            case QMetaType::Float:
                result.append("Ljava/lang/Float;");
                break;
            case QMetaType::Double:
                result.append("Ljava/lang/Double;");
                break;
            case QMetaType::QChar:
            case QMetaType::Char:
                result.append("Ljava/lang/Character;");
                break;
            case QMetaType::QStringList:
                result.append("[Ljava/lang/String;");
                break;
            default:
                BLogger::error(0,__FUNCTION__,"argument type not identified:"+QString(v.typeName()));
                break;
        }
    }
    result.append(")");
    //qDebug()<<"retornando argumentos:"<<result;
    return result;
}



/*
 *Returns the java value from QVariant
 */
 bool PAndroid::getJavaValue(const QVariant &v, QAndroidJniObject &outObj){
#ifdef Q_OS_ANDROID
    switch(v.userType()){
        case QMetaType::QString:
            outObj=QAndroidJniObject::fromString(v.toString());
            return true;
        case QMetaType::Int:
            outObj=QAndroidJniObject("java/lang/Integer","(I)V",(jint)v.toInt());
            return true;
        case QMetaType::LongLong:
             outObj=QAndroidJniObject("java/lang/Long", "(J)V", (jlong)v.toLongLong());
            return true;
        case QMetaType::Bool:
            outObj=QAndroidJniObject("java/lang/Boolean", "(Z)V", (jboolean)v.toBool());
            return true;
        case QMetaType::Float:
            outObj=QAndroidJniObject("java/lang/Float","(F)V",(jfloat)v.toFloat());
            return true;
        case QMetaType::Double:
            outObj=QAndroidJniObject("java/lang/Double","(D)V",(jdouble)v.toDouble());
            return true;
        case QMetaType::QChar:
        case QMetaType::Char:
            outObj=QAndroidJniObject("java/lang/Character","(C)V",(jchar)v.toChar().toLatin1());
            return true;
        case QMetaType::QStringList:{
            QAndroidJniObject mixedCaseString = QAndroidJniObject::fromString(v.toStringList().join(","));
            QAndroidJniObject splitter = QAndroidJniObject::fromString(",");
            outObj= mixedCaseString.callObjectMethod("split", "(Ljava/lang/String;)[Ljava/lang/String;",splitter.object<jstring>());
            return true;
        }
        default:
            BLogger::error(0,__FUNCTION__,"Unknown value");
    }
#endif
    return false;
 }


#ifdef Q_OS_ANDROID
QVariant PAndroid::getQVariant(jobject obj){
     QAndroidJniEnvironment qjniEnv;
     return getJObjectVariant(qjniEnv,obj);
}
#endif

