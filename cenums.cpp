#include <QtCore>
#include "cenums.h"
#include "cbase.h"
#include "blogger.h"

CEnums::CEnums(QObject *parent) :QObject(parent){
}


QString CEnums::getNameFromEnumValue(const QMetaObject &metaObj, const QString &enumName, qint32 value){
	QMetaEnum metaEnum=getMetaEnum(metaObj,enumName);
	QString enumNome=metaEnum.name();
	for(int i=0; i<metaEnum.keyCount(); i++) {
        if(metaEnum.value(i)!=value)continue;
		QString key(metaEnum.key(i));
		int index=key.indexOf(enumNome);
		if(index>=0)key=key.remove(index,enumNome.length());
		key=key.replace('_',' ');
		return key;
	}
	BLogger::error(0,__FUNCTION__,"unable to get enum name "+enumName+" from class "+QString(metaObj.className())+" . Value:"+ QString::number(value));
	return QString();
}

QString CEnums::getNameFromBaseEnumValue(const QString &enumName, qint32 value){
	return getNameFromEnumValue(CBase::staticMetaObject,enumName,value);
}


QList<CEnums::STREnum> CEnums::getEnumKeysAndValues(const QMetaObject &metaObj, const QString &enumName){
	QMetaEnum metaEnum=getMetaEnum(metaObj,enumName);
	QString enumNome=metaEnum.name();
	QList<STREnum> list;
	for(int i=0; i<metaEnum.keyCount(); i++) {
		QString key(metaEnum.key(i));
		int index=key.indexOf(enumNome);
		if(index>=0)key=key.remove(index,enumNome.length());
		key=key.replace('_',' ');
		STREnum str;
		str.key=key;
		str.value=metaEnum.value(i);
		list.append(str);
	}
	return list;
}


/**
 *Return a QMetaEnum with the give name
 */
QMetaEnum CEnums::getMetaEnum(const QMetaObject &metaObj, const QString &enumName) {
	QMetaEnum metaEnum;
	metaEnum= metaObj.enumerator(metaObj.indexOfEnumerator(enumName.toUtf8().constData()));
	return metaEnum;
}
