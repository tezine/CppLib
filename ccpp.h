#ifndef ECPP_H
#define ECPP_H
#include <QPointer>
#include "cpplib_global.h"

class STRVar;
class BasicType;


/*
 * Class responsible to manage C++
 */
class CPPLIB_EXPORT ECpp : public QObject {
	Q_OBJECT

	public:
		explicit ECpp(QObject* parent = 0);
		static ECpp* obj(){if(!o) o=new ECpp(); return o; }
		QString getCppEnumValueName(const QString& className, const QString& enumName, qint32 enumValue);
		QString getCppFlagsValuesNames(const QString& className, const QString& flagName, qint32 value);
		bool runStaticFunction(const QString& className, const QString& functionBaseName, const QVariantList& argsList, QVariant& retVal, QString& outError);
		static bool containsProperty(QObject* o, const QString& propertyName);
		bool runFunctionWithVariants(QObject* obj, const QString& method, const QVariantList& args, QVariant* ret, QString& outError, Qt::ConnectionType type = Qt::AutoConnection);
		bool runFunctionWithArray(QObject* obj, const QString& method, const QByteArray& arg1,const QByteArray& arg2,const QByteArray& arg3,const QByteArray& arg4,
		                          const QByteArray& arg5, const QByteArray& arg6,const QByteArray& arg7, const QByteArray& arg8,const QByteArray& arg9,
		                          const QByteArray& arg10, QVariant* ret, Qt::ConnectionType type = Qt::AutoConnection);
		QVariantList getVariantList(const QList<QByteArray>& args);
		QObject* createObjectFromVariant(QList<QMetaObject> listaDeClassesDeDados, const QString& className, const QVariantList& args);
		static QObject* createObjectFromVariant(const QString& cppClassName, const QVariantList& args, QString& outError);
		static qint32 getReturnType(const QMetaObject* obj,const QByteArray& method,  QList<QByteArray> argTypes);
		static QString getSlotName(const char* slot);
		static QString getArgumentName(const char* slot);
        QMetaObject getMeta(const QString &className);
        QList<QMetaObject> getMeta();
        QObject *getCppObj(const QString &className){return NULL;}

	signals:
		void sigException(const QString& msg);

	private:
		bool containsFunction(QObject* obj, const QString& methodBaseName, const QList<QByteArray>& argTypes, QString& outMethodName);
		static QObject* createObjectFromVariantWithObjectStar(QVariant& variant);
		QString getCorrectParameterTypeName(const QString& className, const QVariant& v);
		static const char* getCorrectParameterName(const QString& className, const QVariant& v);
		static QObject* createObjectWithQVarianttArgs(QMetaObject& obj, const QVariantList& argumentList);
		static QPointer<ECpp> o;
};
#endif
