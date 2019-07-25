#ifndef BMETAOBJ_H
#define BMETAOBJ_H
#include <QPointer>
#include "cpplib_global.h"

/*
 *Class to handle metaobject
 */
class CPPLIB_EXPORT CMeta : public QObject{
    Q_OBJECT

public:
    static CMeta *obj(){if(!o)o=new CMeta();return o;}
	void setup(const QList<QMetaObject> &list);
    static QObject *createObject(const QString &typeName);
    static bool convertObjectListToVariant(qint32 listUserType, QList<QObject *> &objList, QVariant &outVariant, bool withPointerObjects=true);
    static bool convertVariantListToObjectList(const QVariantList &vList, QList<QObject*> &outList);
    static void convertVariantMapToObject(const QVariantMap &variant, QObject *object);
    static QVariantMap convertObjectToQVariantMap(const QObject *object, const QStringList &ignoredProperties);
    static QObject* createObjectFromVariant(QVariant& variant);
    static bool containsProperty(QObject *obj, const QString &propertyName);
    static QVariant getPropertyValue(QObject *obj, const QString &propertyName);
    static bool mapProperties(QObject* from, QObject* to);
    static bool deleteListObjects(QVariantList &vList);

private:
    explicit CMeta(QObject *parent = 0);
    static QPointer<CMeta> o;
    static QList<QMetaObject> metaList;
};
#endif
