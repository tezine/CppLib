#ifndef EJSON_H
#define EJSON_H
#include <QObject>
#include <QStringList>
#include <QJsonValue>
#include "cpplib_global.h"

class QQmlApplicationEngine;
class QJSValue;

/*
 * All Json/QObject related functions
 * This class is used internally by Studio. It's not used by Engine. Instead use EJson
 */
class CPPLIB_EXPORT  JSON : public QObject {
    Q_OBJECT

	public:
        explicit JSON(QObject* parent = 0);
		static QString convertObjectToJsonDoc(QObject& obj, bool indentResult=false);
        static bool convertListToJsonValue(const QVariant v, QJsonValue &outValue);
        static bool convertListToJsonDoc(const QList<QObject *> &list, QJsonDocument &outJsonDoc);
        static bool convertJsonDocToObject(const QString& json, QObject& obj);
		bool convertJsonObjectToObject(const QJsonObject &jsonObj, QObject &obj);
		static bool convertJsonDocToJSValue(QQmlApplicationEngine *engine, const QString& json,QJSValue &outJSValue, const QMetaObject& metaObject);
        static bool convertJsonDocToList(const QString& json, QList<QObject*>& outList, const QMetaObject& listMetaType);
		static bool convertJsonDocToJSValueList(QQmlApplicationEngine *engine, const QString& json,QJSValue &outJSValue, const QMetaObject& listMetaType);
        static bool convertJsonArrayToList(const QJsonArray &jsonArray, QVariantList &outList, const QMetaObject& listMetaType);
        static bool convertJsonArrayToList(const QJsonArray &jsonArray, QVariantList &outList, qint32 objMetaType);
		static bool convertObjectToJSValue(QQmlApplicationEngine *engine, QObject *obj, QJSValue &outJSValue, bool setJavascriptOwnership);
        static QJsonObject convertObjectToJsonObject(const QObject& obj);
        static bool convertJsonValueToObject(QObject &obj, const QJsonValue &jsonValue);
        static QJsonValue runFunction(QObject *obj, const QString &methodName, const QJsonValue &arg1=QJsonValue(),const QJsonValue &arg2=QJsonValue(), const QJsonValue &arg3=QJsonValue(),
                                      const QJsonValue &arg4=QJsonValue(),const QJsonValue &arg5=QJsonValue(), const QJsonValue &arg6=QJsonValue(),const QJsonValue &arg7=QJsonValue(), const QJsonValue &arg8=QJsonValue(), const QJsonValue &arg9=QJsonValue(), const QJsonValue &arg10=QJsonValue());

private:
		static bool deserializeFromJSONValue(QObject& object, const QJsonValue& jsonValue, const char* keyName);
		static bool serializeQListToJsonObject(const QString &name, const QVariant &v, QJsonObject &outValue);
		static bool convertVariantToObjectList(QVariant &v, QList<QObject *> &qobjectList);
};
#endif
