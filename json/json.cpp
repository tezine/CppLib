#include <QtCore>
#include <QQmlApplicationEngine>
#include "json.h"
#include "blogger.h"
#include "cbase.h"
#include "cmeta.h"

JSON::JSON(QObject *parent) :QObject(parent){
}

/**
 * Só o Q_DECLARE_METATYPE nao é suficiente. Precisa chamar o getMeta do objeto senao nao o usertype nao é reconhecido
 */
QString JSON::convertObjectToJsonDoc(QObject &obj,bool indentResult){
    QJsonObject jsonObj=convertObjectToJsonObject(obj);
	QJsonDocument jsonDoc(jsonObj);
	if(indentResult)return jsonDoc.toJson(QJsonDocument::Indented);
	else return jsonDoc.toJson(QJsonDocument::Compact);
}


/*
 *Returns a QJsonObject from QObject 'obj' with all properties filled
 */
QJsonObject JSON::convertObjectToJsonObject(const QObject &obj){
	QJsonObject jsonObj;
	QVariantMap variantMap;
	const QMetaObject *metaobject = obj.metaObject();
	int count = metaobject->propertyCount();
	for (int p=0; p<count; ++p) {
		QMetaProperty metaproperty = metaobject->property(p);
		QString name(metaproperty.name());
		QVariant propertyVariant = obj.property(name.toUtf8().constData());
        if(name=="objectName" &&propertyVariant.isNull())continue;
		variantMap[name] = propertyVariant;
		QMetaType metatype(propertyVariant.userType());
		if(!metatype.isValid() || (!metatype.isRegistered())){BLogger::error(0,__FUNCTION__,"MetaType not valid for property: "+QString(metaproperty.typeName())+" or not registered. Did you call getMeta()?");return QJsonObject();}
		if(propertyVariant.type()!=QVariant::UserType)jsonObj.insert(name, QJsonValue::fromVariant(propertyVariant));
		else{//usertype
			QString typeName(metaproperty.typeName());
			if(typeName.contains("QList<"))serializeQListToJsonObject(name,propertyVariant,jsonObj);
			else{//object
                QObject *childObj= CMeta::createObjectFromVariant(propertyVariant);
				if(childObj==NULL){BLogger::error(0,__FUNCTION__,"Unable to create Object");return QJsonObject();}
                jsonObj.insert(name,convertObjectToJsonObject(*childObj));
				childObj->deleteLater();
			}
		}
	}
	return jsonObj;
}


/*
 *Serializes a QList into a QJsonValue
 */
bool JSON::convertListToJsonValue(const QVariant v, QJsonValue &outValue){
    QJsonArray jsonArray;
    QList<QObject*> *objectList=(QList<QObject*> *)QMetaType::create(v.userType(),v.data());
    foreach(QObject *o, *objectList){
        QJsonObject childJsonObject=convertObjectToJsonObject(*o);
        jsonArray.append(childJsonObject);
    }
    outValue=QJsonValue(jsonArray);
    return true;
}


/*
 *vList must be QList<SomeQObject*>
 */
bool JSON::convertListToJsonDoc(const QList<QObject*> &objectList, QJsonDocument &outJsonDoc){
    QJsonArray jsonArray;
    foreach(QObject *o, objectList){
        QJsonObject childJsonObject=convertObjectToJsonObject(*o);
        jsonArray.append(childJsonObject);
    }
    outJsonDoc.setArray(jsonArray);
    return true;
}


/*
 *Called from getJsonObject
 *Serializes a QList<> into QJsonObject
 */
bool JSON::serializeQListToJsonObject(const QString &name, const QVariant &v, QJsonObject &outValue){
	QJsonArray jsonArray;
	QList<QObject*> *objectList=(QList<QObject*> *)QMetaType::create(v.userType(),v.data());
	foreach(QObject *o, *objectList){
        QJsonObject childJsonObject=convertObjectToJsonObject(*o);
		jsonArray.append(childJsonObject);
	}
	delete objectList;//qDeleteAll trava nao sei pq.
	outValue.insert(name,jsonArray);
	return true;
}


/**
 *Provavelmente nao funcionan para objeto com propriedade com ponteiro, ou seja nao pode ter QObject*
 */
bool JSON::convertJsonDocToObject(const QString &json, QObject &obj){
	QJsonParseError jsonError;
	QJsonDocument jsonDocument=QJsonDocument::fromJson(json.toUtf8(),&jsonError);
	if(jsonError.error!=QJsonParseError::NoError){BLogger::error(0,__FUNCTION__, "Unable to create QJsonDocument from string:"+jsonError.errorString());return false;}
	QJsonObject jsonObj=jsonDocument.object();
	QStringList jsonObjKeys=jsonObj.keys();
	foreach(QString jsonKey, jsonObjKeys){
		QJsonValue jsonValue= jsonObj.value(jsonKey);
        if(!deserializeFromJSONValue(obj,jsonValue,jsonKey.toUtf8().constData()))return false;
	}
	return true;
}


/*
 *Converts a json object to object
 */
bool JSON::convertJsonObjectToObject(const QJsonObject &jsonObj, QObject &obj){
	QStringList jsonObjKeys=jsonObj.keys();
	foreach(QString jsonKey, jsonObjKeys){
		QJsonValue jsonValue= jsonObj.value(jsonKey);
		if(!deserializeFromJSONValue(obj,jsonValue,jsonKey.toUtf8().constData()))return false;
	}
	return true;
}


/*
 *Converts the json to a QJSValue
 */
bool JSON::convertJsonDocToJSValue(QQmlApplicationEngine *engine, const QString &json, QJSValue &outJSValue, const QMetaObject &metaObject){
	QObject *o=metaObject.newInstance();
	if(!convertJsonDocToObject(json,*o))return false;
	outJSValue = engine->newQObject(o);
	o->deleteLater();
	return true;
}


/*
 *Converts the object to a QJSValue
 */
bool JSON::convertObjectToJSValue(QQmlApplicationEngine *engine, QObject *obj, QJSValue &outJSValue, bool setJavascriptOwnership){
	engine->setObjectOwnership(obj,QQmlEngine::JavaScriptOwnership);
	outJSValue = engine->newQObject(obj);
	return true;
}


bool JSON::convertVariantToObjectList(QVariant &v, QList<QObject*> &qobjectList){
	if (v.canConvert(QMetaType::QVariantList)) {
		QSequentialIterable iterable = v.value<QSequentialIterable>();
		foreach (const QVariant& item, iterable) {
			QObject* object = item.value<QObject*>();
			if (object) { qobjectList.append(object); }
		}
	}
	return true;
}


/*
 *Deserialize a json text to a list of QObjects
 */
bool JSON::convertJsonDocToList(const QString &json, QList<QObject *> &outList, const QMetaObject &listMetaType){
	QJsonParseError jsonError;
	QJsonDocument jsonDocument=QJsonDocument::fromJson(json.toUtf8(),&jsonError);
	if(jsonError.error!=QJsonParseError::NoError){BLogger::error(0,__FUNCTION__ ,"Unable to create QJSonDocument from string:"+jsonError.errorString());return false;}
	if(!jsonDocument.isArray()){BLogger::error(0,__FUNCTION__,"String is not an array");return false;return false;}
	QJsonArray jsonArray=jsonDocument.array();
	for(int i=0;i<jsonArray.count();i++){
		QJsonObject jsonObject=jsonArray.at(i).toObject();
		QObject *o=listMetaType.newInstance();
        CMeta::convertVariantMapToObject(jsonObject.toVariantMap(),o);
        outList.append(o);
	}
	return true;
}



/*
 *Converts a json doc to a QJSValueList
 */
bool JSON::convertJsonDocToJSValueList(QQmlApplicationEngine *engine, const QString &json, QJSValue &outJSValue, const QMetaObject &listMetaType){
	QList<QObject*> objList;
	if(!convertJsonDocToList(json,objList,listMetaType))return false;
	outJSValue = engine->newArray(objList.size());
	for (int i = 0; i < objList.size();i++) {
		QJSValue v = engine->newQObject(objList[i]);
		outJSValue.setProperty(i, v);
	}
	return true;
}



/*
 *Deserializes a jsonarray, like one created using serializeQListToJsonValue
 */
bool JSON::convertJsonArrayToList(const QJsonArray &jsonArray, QVariantList &outList, const QMetaObject &listMetaType){
	for(int i=0;i<jsonArray.count();i++){
        if(!jsonArray.at(i).isObject())outList.append(jsonArray.at(i).toVariant());//not a qobject. ex: int, qstring, double, bool
		QJsonObject jsonObject=jsonArray.at(i).toObject();
		QObject *o=listMetaType.newInstance();
        CMeta::convertVariantMapToObject(jsonObject.toVariantMap(),o);
        outList.append(QVariant::fromValue(o));
	}
    return true;
}


/*
 * Same as above. objMetaType must be from a pointer qobject
 */
bool JSON::convertJsonArrayToList(const QJsonArray &jsonArray, QVariantList &outList, qint32 objMetaType){
    if(!QMetaType::isRegistered(objMetaType)){BLogger::error(0,__FUNCTION__,"listUserType is not registered for metaTypeID: "+QString::number(objMetaType)+QString(".Name:")+QMetaType::typeName(objMetaType));return false;}
    const QMetaObject *meta=QMetaType::metaObjectForType(objMetaType);
    if(!meta){BLogger::error(0,__FUNCTION__,"Null meta");return false;}
	return convertJsonArrayToList(jsonArray,outList,*meta);
}


/*
 * Arrumar
 */
bool JSON::convertJsonValueToObject(QObject &obj, const QJsonValue &jsonValue){
	QJsonObject childJsonObject=jsonValue.toObject();
	QStringList jsonObjChildKeys=childJsonObject.keys();
	foreach(QString childJsonKey, jsonObjChildKeys){
		QJsonValue childJsonValue= childJsonObject.value(childJsonKey);
		//qDebug()<<"value:"<<childJsonValue<<"key:"<<childJsonKey;
        if(!deserializeFromJSONValue(obj,childJsonValue,childJsonKey.toUtf8().constData()))return false;
	}
    return true;
}


/*
 *Attempts to execute the function 'methodName'
 */
QJsonValue JSON::runFunction(QObject *obj, const QString &methodName, const QJsonValue &arg1, const QJsonValue &arg2, const QJsonValue &arg3,
                             const QJsonValue &arg4,const QJsonValue &arg5, const QJsonValue &arg6,const QJsonValue &arg7, const QJsonValue &arg8, const QJsonValue &arg9, const QJsonValue &arg10){
    QString completeMethodName=QString(obj->metaObject()->className())+"."+methodName;
    //qDebug()<<"(JSON)runFunction."+completeMethodName;
    QJsonValue retValue;
    if(!arg10.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5),Q_ARG(QJsonValue,arg6),Q_ARG(QJsonValue,arg7),Q_ARG(QJsonValue,arg8),Q_ARG(QJsonValue,arg9),Q_ARG(QJsonValue,arg10))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 10 arguments");return QJsonValue();}}
    else if(!arg9.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5),Q_ARG(QJsonValue,arg6),Q_ARG(QJsonValue,arg7),Q_ARG(QJsonValue,arg8),Q_ARG(QJsonValue,arg9))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 9 arguments");return QJsonValue();}}
    else if(!arg8.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5),Q_ARG(QJsonValue,arg6),Q_ARG(QJsonValue,arg7),Q_ARG(QJsonValue,arg8))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 8 arguments");return QJsonValue();}}
    else if(!arg7.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5),Q_ARG(QJsonValue,arg6),Q_ARG(QJsonValue,arg7))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 7 arguments");return QJsonValue();}}
    else if(!arg6.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5),Q_ARG(QJsonValue,arg6))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 6 arguments");return QJsonValue();}}
    else if(!arg5.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4),Q_ARG(QJsonValue,arg5))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 5 arguments");return QJsonValue();}}
    else if(!arg4.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3),Q_ARG(QJsonValue,arg4))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 4 arguments");return QJsonValue();}}
    else if(!arg3.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2),Q_ARG(QJsonValue,arg3))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 3 arguments");return QJsonValue();}}
    else if(!arg2.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1),Q_ARG(QJsonValue,arg2))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 2 arguments");return QJsonValue();}}
    else if(!arg1.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue),Q_ARG(QJsonValue,arg1))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with 1 argument");return QJsonValue();}}
    if(arg1.isNull()){if(!QMetaObject::invokeMethod(obj, methodName.toUtf8().constData(), Q_RETURN_ARG(QJsonValue, retValue))){BLogger::error(0,__FUNCTION__,"Unable to invoke "+completeMethodName+" with no arguments");return QJsonValue();}}
    return retValue;
}


/**
 *obj receives the content of jsonValue
 */
bool JSON::deserializeFromJSONValue(QObject &obj, const QJsonValue &jsonValue, const char *keyName){
	//qDebug()<<"deserializando obj"<<obj.metaObject()->className()<<".IsObj:"<<jsonValue.isObject()<<".isArray:"<<jsonValue.isArray()<<".KeyName:"<<keyName;
	//if(!(&obj)){BLogger::error(this,__FUNCTION__,"null obj");return false;}
	qint32 propertyIndex=obj.metaObject()->indexOfProperty(keyName);
	if(propertyIndex==-1){/*BLogger::debug(0,__FUNCTION__,"obj "+QString(obj.metaObject()->className())+" does not have a property named "+QString(keyName));*/return true;}
	int propertyType=obj.metaObject()->property(propertyIndex).type();
	if(propertyType==QMetaType::QJsonValue){
		obj.setProperty(keyName,jsonValue);return true;
	}
	if((!jsonValue.isObject()) && (!jsonValue.isArray())){//se der pau aqui, apague os entities, o build e recompile o projeto
		obj.setProperty(keyName,jsonValue.toVariant());return true;//primitive value
	}
	if(jsonValue.isObject()){
		QVariant objectVariant=obj.property(keyName);
        QObject *childObj=CMeta::createObjectFromVariant(objectVariant);
		QJsonObject childJsonObject=jsonValue.toObject();
		QStringList jsonObjChildKeys=childJsonObject.keys();
		foreach(QString childJsonKey, jsonObjChildKeys){
			QJsonValue childJsonValue= childJsonObject.value(childJsonKey);
			deserializeFromJSONValue(*childObj,childJsonValue,childJsonKey.toUtf8().constData());
		}
        if(!obj.setProperty(keyName,QVariant(objectVariant.userType(),(void*)childObj))){BLogger::error(0,__FUNCTION__,"Unable to set property:"+QString(keyName));return false;}
		childObj->deleteLater();
	}
	if(jsonValue.isArray()){
		QJsonArray jsonArray=jsonValue.toArray();
        QVariantList vList;
        QString typeName=obj.property(keyName).typeName();
        typeName=typeName.remove("QList<").remove(">");
        if(jsonArray.isEmpty())return true;
        if(!jsonArray.at(0).isObject()){//not object array
            if(!obj.setProperty(keyName,jsonValue.toVariant())){BLogger::error(0,__FUNCTION__,"Unable to set property:"+QString(keyName));return false;}
            return true;
        }
        //object array below
        if(!typeName.endsWith("*"))typeName.append("*");
        int tipo=QMetaType::type(typeName.toUtf8().constData());
        //qDebug()<<"deserializando array:"<<keyName<<tipo<<typeName<< jsonValue;
        QList<QObject*> objList;
        if(tipo==QMetaType::UnknownType){BLogger::error(0,__FUNCTION__,"Invalid type for typeName:"+typeName);return false;}
        if(!convertJsonArrayToList(jsonArray,vList,tipo))return false;
        CMeta::convertVariantListToObjectList(vList,objList);
        if(!obj.setProperty(keyName,QVariant::fromValue(objList))){BLogger::error(0,__FUNCTION__,"Unable to set property:"+QString(keyName));return false;}
        //se usar deleteLater nos objs trava
	}
	return true;
}
