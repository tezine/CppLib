#include <QtCore>
#include "ccpp.h"
#include "blogger.h"
#include "cbase.h"

QPointer<ECpp> ECpp::o=0;
ECpp::ECpp(QObject *parent) :QObject(parent){
}


/*
 *Returns the enum value name.
 *Ex: DFile.error() returns OpenError
 */
QString ECpp::getCppEnumValueName(const QString &className, const QString &enumName, qint32 enumValue){
	QMetaObject meta=getMeta(className);
	for(int i=0;i<meta.enumeratorCount();i++){
		QMetaEnum metaEnum=meta.enumerator(i);
		if(metaEnum.isFlag())continue;
		if(metaEnum.name()!=enumName)continue;
		QString valueName(metaEnum.valueToKey(enumValue));
		return valueName;
	}
	BLogger::error(this,__FUNCTION__,"enum value name not found for value:"+QString::number(enumValue));
	return QString();
}


/*
 *Returns the values names of a WFlags value.
 *Ex: dir.setSorting(DDir.Name | DDir.Time).
 *WFlags sorting=dir.sorting();
 *sorting.toText(); -> Name | Time
 */
QString ECpp::getCppFlagsValuesNames(const QString &className, const QString &flagName, qint32 value){
	QMetaObject meta=getMeta(className);
	for(int i=0;i<meta.enumeratorCount();i++){
		QMetaEnum metaEnum=meta.enumerator(i);
		//qDebug()<<"enum"<<metaEnum.name()<<metaEnum.isFlag();
		if(!metaEnum.isFlag())continue;
		if(metaEnum.name()!=flagName)continue;
		QString names(metaEnum.valueToKeys(value));
		//qDebug()<<"names"<<names;
		return names;
	}
	BLogger::error(this,__FUNCTION__,"flag key names not found for values "+QString::number(value));
	return QString();
}


/*
 *Executes a static function.
 *Static function cannot be executed using Q_INVOKABLE
 */
bool ECpp::runStaticFunction(const QString &className, const QString &functionBaseName,const QVariantList &argsList, QVariant &retVal, QString &outError){
	QObject *obj=getCppObj(className);
	return runFunctionWithVariants(obj,functionBaseName,argsList,&retVal,outError);
}


/*
 *Returns true if the object contains the property 'propertyName'
 */
bool ECpp::containsProperty(QObject *o, const QString &propertyName){
	const QMetaObject *meta=o->metaObject();
	for(int i=0;i<meta->propertyCount();i++){
		if(propertyName==QString(meta->property(i).name())){
			return true;
		}
	}
	return false;
}

/*
  *
  */
QVariantList ECpp::getVariantList(const QList<QByteArray> &args) {
	QVariantList vList;
	foreach(QByteArray arg, args) {
		if((!arg.isNull()) && !arg.isEmpty())vList.append(CBase::deserializeArray(arg));
	}
	return vList;
}


/*
  *First we attempt to create the object with real argument types. Ex; WInt32*, Text*, ...
  *If we fail, we attempt to create using WObject*
  */
QObject *ECpp::createObjectFromVariant(const QString &cppClassName,const QVariantList &arguments, QString &outError) {
	QList<QMetaObject> metaList= ECpp::obj()->getMeta();
	bool classFound=false;
	QMetaObject metaObject;
	for(int i=0;i<metaList.count();i++){
		metaObject=metaList[i];
		if(metaObject.className()!=cppClassName)continue;
		classFound=true;
		break;
	}
	if(!classFound){outError="C++ class not found:"+cppClassName;return NULL;}
	if(arguments.count() > 10)return NULL;
	QGenericArgument arg[10];
	for(int n = 0; n < arguments.count(); ++n){
		arg[n] = QGenericArgument(getCorrectParameterName(cppClassName, arguments[n]),arguments[n].constData());
	}
	QObject *newObj= metaObject.newInstance(arg[0], arg[1], arg[2], arg[3], arg[4],arg[5], arg[6], arg[7], arg[8], arg[9]);
	if(newObj==NULL){
		newObj=createObjectWithQVarianttArgs(metaObject, arguments);
		if(newObj)return newObj;
		outError="Unable to create object of class"+cppClassName+". Check if "+ QString(metaObject.className())+" has Q_INVOKABLE";
		BLogger::log(LogTypeError, 0, __FUNCTION__, outError+". Follow the args passed below:");
		for(int i=0;i<arguments.count();i++){
			QVariant v=arguments.at(i);
			if(v.isValid() && (!v.isNull()))BLogger::error(0,__FUNCTION__,"argument "+QString::number(i)+QString(". Passed:")+v.typeName()+QString(". Modified to:")+ ECpp::obj()->getCorrectParameterTypeName(cppClassName,v));
		}
		return newObj;
	}
	//In case the object was created, but we set an exception in the constructor. Ex: DFile contructor
	return newObj;
}


/*
 *Called from createObjectFromVariant only
 */
QObject *ECpp::createObjectWithQVarianttArgs(QMetaObject &metaObj, const QVariantList &argumentList){
	QGenericArgument arg[10];
	for(int n = 0; n < argumentList.count(); ++n){
		arg[n]=Q_ARG(QVariant,argumentList[n]);
	}
	QObject *newObj= metaObj.newInstance(arg[0], arg[1], arg[2], arg[3], arg[4],arg[5], arg[6], arg[7], arg[8], arg[9]);
	return newObj;
}

/*
  *
  */
QObject *ECpp::createObjectFromVariant(QList<QMetaObject> listaDeClassesDeDados, const QString &className, const QVariantList &args) {
	//QMetaObject::invokeMethod() has a 10 argument maximum
	if(args.count() > 10)return NULL;
	QList<QByteArray> argTypes;
	for(int n = 0; n < args.count(); ++n)argTypes += args[n].typeName();
	QGenericArgument arg[10];
	for(int n = 0; n < args.count(); ++n)
		arg[n] = QGenericArgument(args[n].typeName(),args[n].constData());
	foreach(QMetaObject obj, listaDeClassesDeDados) {
		if(QString(obj.className())!=className)continue;
		QObject *newObj=obj.newInstance(arg[0], arg[1], arg[2], arg[3], arg[4],arg[5], arg[6], arg[7], arg[8], arg[9]);
		if(newObj==NULL) {
			BLogger::error(this, __FUNCTION__, "Classe de dados "+QString(obj.className())+ " deve ter um construtor sem argumento com Q_INVOKABLE");
			return newObj;
		}
		//qDebug()<<"criou objeto: "<<nome;
		return newObj;
	}
	qDebug()<<"(BI)createObjectFromVariant.nao criou objeto";
	return NULL;
}



/*
  *
  */
bool ECpp::runFunctionWithArray(QObject *obj, const QString &method1, const QByteArray &arg1,const QByteArray &arg2,const QByteArray &arg3,const QByteArray &arg4,
				   const QByteArray &arg5, const QByteArray &arg6, const QByteArray &arg7, const QByteArray &arg8,
				   const QByteArray &arg9, const QByteArray &arg10, QVariant *ret, Qt::ConnectionType type) {
	QList<QByteArray> argsList;
	argsList<<arg1<<arg2<<arg3<<arg4<<arg5<<arg6<<arg7<<arg8<<arg9<<arg10;
	QVariantList vList=getVariantList(argsList);
	QString exception;
	return runFunctionWithVariants(obj,method1,vList,ret,exception,type);
}


/*
  *If the function could not be invoked, we fill the 'outError'
  *If the function set an error using WException::setException, we also fill the 'outError'
  */
bool ECpp::runFunctionWithVariants(QObject *obj, const QString &method1, const QVariantList &args, QVariant *ret, QString &outError, Qt::ConnectionType type) {
	//qDebug()<<"invoking method "<<obj->metaObject()->className()<<"."<< method1<<" with arguments "<<args;
	if(args.count() > 10)return false;
	//QTime startTime=QTime::currentTime();
	QString outMethodName;
	QByteArray method=QMetaObject::normalizedSignature(method1.toUtf8().constData());
	QString className(obj->metaObject()->className());
	QList<QByteArray> argTypes;
	for(int n = 0; n < args.count(); ++n)
		argTypes += getCorrectParameterTypeName(className, args[n]).toUtf8();
	if(!containsFunction(obj,method1,argTypes,outMethodName)){outError="cpp function not found:"+QString(className)+"."+ outMethodName; return false;}
	// QMetaObject::invokeMethod() has a 10 argument maximum
	qint32 returnType = getReturnType(obj->metaObject(),method, argTypes);
	QGenericArgument arg[10];
	QStringList argumentTypeNames;
	//todo remover isso do dlist
	bool isDList=false;
	if(obj->metaObject()->className()==QString("DList"))isDList=true;
	for(int n = 0; n < args.count(); ++n) {
		if(isDList)arg[n] = QGenericArgument("WObject*",args[n].constData());
		else arg[n] = QGenericArgument(getCorrectParameterName(className, args[n]),args[n].constData());
	}
	QGenericReturnArgument retarg;
	QVariant retval;
	QString returnTypeName;
	if(returnType != QMetaType::Void) {
		retval = QVariant(returnType, (const void *)0);
		QString barbosa=QString(retval.typeName());
		if(!barbosa.isEmpty())returnTypeName=barbosa;
		else returnTypeName="retTypeNotIdentified";
		//qDebug()<<method1<<returnType<<returnTypeName<<obj->metaObject()->className();
		QByteArray text = barbosa.toLocal8Bit();
		char *data = new char[text.size() + 1];//todo apagar esses dados depois
		strcpy(data, text.data());
		retarg = QGenericReturnArgument( data,retval.data());
	}
	if(!QMetaObject::invokeMethod(obj, method.data(), type, retarg, arg[0], arg[1], arg[2], arg[3], arg[4],arg[5], arg[6], arg[7], arg[8], arg[9])) {
		//emit sigException("unable to invoke cpp method "+method1);
		outError="Unable to invoke cpp function "+returnTypeName+" " +className+"." +method1+"("+argumentTypeNames.join(",")+")";
		BLogger::error(this,__FUNCTION__,outError+". Call getMeta() for the class, its parameters and return type. Parameters and return type must be registered as variants.");
		return false;
	}
	if(retval.isValid() && ret)*ret = retval;	
	//else BLogger::log(LogTypeDebug,this, "invokeMethodWithVariants","void or invalid ret type");
	//QTime endTime=QTime::currentTime();
	//BLogger::log(LogTypeDebug,this,"invokeMethodWithVariants","executed "+QString(obj->metaObject()->className())+"." +outMethodName +"  "+QString::number(startTime.msecsTo(endTime))+"msecs");
	return true;
}


/*
  *
  */
bool ECpp::containsFunction(QObject *obj, const QString &methodBaseName, const QList<QByteArray> &argTypes,QString &outMethodName) {
	if(obj==NULL || qobject_cast<QObject *>(obj)==NULL ) {emit sigException("Null object for method "+methodBaseName); BLogger::log(LogTypeError,this, "methodExists", "null object for method "+methodBaseName);return false;}
	//todo arrumar
	if(obj->metaObject()->className()==QString("DList"))return true;
	outMethodName=methodBaseName+"(";
	for(int n = 0; n < argTypes.count(); ++n) {
		if(n!=(argTypes.count()-1))outMethodName.append(QString(argTypes.at(n))+",");
		else outMethodName.append(QString(argTypes.at(n)));
	}
	outMethodName.append(")");
	//qDebug()<<"method name:"<<completeMethodName;
	if(obj->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(outMethodName.toUtf8().constData()))==-1) {
		if(!methodBaseName.contains(QRegExp("^respProgress")))BLogger::error(this,__FUNCTION__,"method not found:"+QString(obj->metaObject()->className())+"."+ outMethodName);
		return false;
	}
	return true;
}


/*
  *
  */
qint32 ECpp::getReturnType(const QMetaObject *metaObject,const QByteArray &method, QList<QByteArray> argTypes) {
	QString metodo(method);
	metodo=metodo.append("(");
	QStringList args;
	//todo remover do dlist
	bool isDList=false;
	if(metaObject->className()==QString("DList"))isDList=true;
	foreach(QByteArray array,argTypes){
		if(isDList)args.append("WObject*");
		else args.append(QMetaObject::normalizedType(array.constData()));
	}
	metodo=metodo.append(args.join(","));
	metodo=metodo.append(")");
	//qDebug()<<"verificando return:"<<metodo;
	QByteArray array=QMetaObject::normalizedSignature(metodo.toUtf8().constData());
	int index=metaObject->indexOfMethod(array.constData());
	if(index==-1){BLogger::error(0,__FUNCTION__,"Unable to find return type for method "+QString(array));return -1;}
	QMetaMethod metaMethod=metaObject->method(index);
	qint32 returnType=metaMethod.returnType();
	return returnType;
}

/*
  *
  */
QString ECpp::getCorrectParameterTypeName(const QString &className, const QVariant &v) {
	QString typeName(v.typeName());
	if(typeName=="qlonglong")return QString("qint64");
	else if(typeName=="qulonglong")return QString("quint64");
	else if(typeName=="int")return QString("qint32");
	else if(typeName=="uint")return QString("quint32");
	else if(typeName=="QObject*")return QString(v.value<QObject*>()->metaObject()->className())+"*";
	else if(typeName=="EWS*")return QString("QObject*");
	return typeName;
}


/*
 *I don't know why, but we have to send the arguments like this to QGenericArgument
 */
const char *ECpp::getCorrectParameterName(const QString &className, const QVariant &v){
	QString barbosa=QString(ECpp::obj()->getCorrectParameterTypeName(className,v));
	QByteArray text = barbosa.toLocal8Bit();
	char *data = new char[text.size() + 1];//todo apagar esses dados depois
	strcpy(data, text.data());
	return data;
}


QString ECpp::getSlotName(const char *slot) {
	QString temp(slot);
	temp=temp.remove(QRegExp("^\\d*"));
	temp=temp.remove(QRegExp("\\(.*"));
	qDebug()<<"slot:"<<temp;
	return temp;
}


QString ECpp::getArgumentName(const char *slot) {
	QString temp(slot);
	temp=temp.remove(QRegExp(".*\\("));
	temp=temp.remove(QRegExp("\\).*"));
	qDebug()<<"arg:"<<temp;
    return temp.trimmed();
}

QMetaObject ECpp::getMeta(const QString &className){
    return QMetaObject();
}

QList<QMetaObject> ECpp::getMeta(){
    return QList<QMetaObject>();
}

QObject *ECpp::createObjectFromVariantWithObjectStar(QVariant &variant){
	return variant.value<QObject*>();
}
