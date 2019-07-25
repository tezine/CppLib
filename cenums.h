#ifndef ENUM_H
#define ENUM_H
#include <QObject>
#include "cpplib_global.h"

/*
 * Class to handle enums
 */
class CPPLIB_EXPORT CEnums : public QObject {
	Q_OBJECT

	public:
		struct STREnum {
			QString key;
			qint32 value;
		};
        explicit CEnums(QObject* parent = 0);
		static QString getNameFromEnumValue(const QMetaObject& metaObj, const QString& enumName, qint32 value);
		static QString getNameFromBaseEnumValue(const QString& enumName, qint32 value);
		static QList<STREnum> getEnumKeysAndValues(const QMetaObject& metaObj,const QString& enumName);
		static QMetaEnum getMetaEnum(const QMetaObject& metaObj, const QString& enumName);
};
#endif
