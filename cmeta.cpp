#include <QtCore>
#include "cmeta.h"
#include "blogger.h"

QPointer<CMeta> CMeta::o=0;
QList<QMetaObject> CMeta::metaList=QList<QMetaObject>();
CMeta::CMeta(QObject *parent) :QObject(parent){
}



void CMeta::setup(const QList<QMetaObject> &list){
    metaList=list;
}

/*
 *Creates an object of typeName
 *Typename must be registered into QMetaObject
 */
QObject *CMeta::createObject(const QString &tName){
    foreach(QMetaObject metaObj, metaList){
        if(QString(metaObj.className())!=tName)continue;
        return metaObj.newInstance();
    }
    BLogger::error(0,__FUNCTION__,"Unable to create obj of class "+tName);
    return NULL;
}


/*
 *Converts the objList into a QVariant
 *Ex:
 *To convert to QVariant(QList<STRItem*>): convertObjectListToVariant(QList<STRItem*> usertype, objList, outVariant, true)
 *To convert to QVariant(QList<STRItem>):   convertObjectListToVariant(QList<STRItem> usertype, objList, outVariant, false)
 */
bool CMeta::convertObjectListToVariant(qint32 listUserType,  QList<QObject *> &objList, QVariant &outVariant, bool withPointerObjects){
    if(!QMetaType::isRegistered(listUserType)){BLogger::error(0,__FUNCTION__,"listUserType is not registered");return false;}
    qint32 objType;
    if(!objList.isEmpty()){
        objType=QMetaType::type(objList.first()->metaObject()->className());
        if(objType==QMetaType::UnknownType){
            BLogger::error(0,__FUNCTION__,"Unknown object type:"+QString(objList.first()->metaObject()->className()));
            return false;
        }
    }
    QVariant v(listUserType, &objList);
    outVariant=v;
    if(!withPointerObjects){//without pointer objects
        //se habilita, trava
        //qDebug()<<"sem pointer";
        //foreach(QObject *o, objList)o->deleteLater();
    }
    return true;
}



/*
 *Converts a QVariantList to a ObjectList.Ex:
 * From QVariantList(QVariant(STRItem*), QVariant(STRItem*)) to QList<STRItem*>
 */
bool CMeta::convertVariantListToObjectList(const QVariantList &vList, QList<QObject *> &outList){
    foreach(QVariant v, vList){
        outList.append(v.value<QObject*>());
    }
    return true;
}


/*
 *Converts a QObject into a QVariantMap
 */
QVariantMap CMeta::convertObjectToQVariantMap( const QObject* object,const QStringList& ignoredProperties){
    QVariantMap result;
    const QMetaObject *metaobject = object->metaObject();
    int count = metaobject->propertyCount();
    for (int i=0; i<count; ++i) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        if (ignoredProperties.contains(QLatin1String(name)) || (!metaproperty.isReadable()))continue;
        QVariant value = object->property(name);
        result[QLatin1String(name)] = value;
    }
    return result;
}


/*
 *Converts a variant map into properties of an object
 */
void CMeta::convertVariantMapToObject(const QVariantMap& variant, QObject* object){
    //qDebug()<<"convertQVariantMapToQObject"<<object->metaObject()->className();
    const QMetaObject *metaobject = object->metaObject();
    QVariantMap::const_iterator it;
    for (it = variant.constBegin(); it != variant.constEnd(); ++it) {
        int index = metaobject->indexOfProperty( it.key().toUtf8() );
        if ( index < 0 ) continue;
        QMetaProperty metaproperty = metaobject->property( index );
        qint32 userType = metaproperty.userType();
        QVariant v( it.value() );
        //qDebug()<<"propertyName:"<<metaproperty.name()<<"propertyType:"<<metaproperty.typeName()<< ".it value:"<<it.value()<<".Can convert:"<<v.canConvert(userType);
        if ( v.canConvert( userType ) ) {
            v.convert( userType );
            metaproperty.write( object, v );
        } else if(QString(metaproperty.typeName()).startsWith("QList")){//qlist property
            if(!v.canConvert<QVariantList>()){BLogger::error(0,__FUNCTION__,"Unable to convert to variantlist to write into "+QString(metaproperty.name()));return;}
            QVariantList vList=v.value<QVariantList>();
            QList<QObject*> objList;
            //qDebug()<<"gravando qlist property:"<<vList.size();
            foreach(QVariant v, vList){
                if(!v.canConvert<QVariantMap>()){BLogger::error(0,__FUNCTION__,"Unable to convert to variantmap to write into"+QString(metaproperty.name()));return;}
                QMap<QString,QVariant> vMap=v.toMap();
                QString typeName=QString(metaproperty.typeName()).remove("QList<").remove(">");
                QObject *o=createObject(typeName);
                if(!o)return;
                convertVariantMapToObject(vMap,o);
                objList.append(o);
            }
            QVariant v;
            if(!CMeta::convertObjectListToVariant(userType,objList,v,false))return;
            if(!metaproperty.write(object, v)){BLogger::error(0,__FUNCTION__,"Unable to write into property: "+QString(metaproperty.name()));return;}
        }else if (QString(QLatin1String("QVariant")).compare(QLatin1String(metaproperty.typeName())) == 0) {
            metaproperty.write( object, v );
        }
    }
}


/**
 *Se fosse para criar o QObject a partir de um QVariant contendo um QObject*, seria so fazer variant.value<QObject*>()
 *O problema é que nao da pra fazer variant.value<QObject>() e
 *QMetaType::metaObjectForType(int type) nao ta funcionando sei la eu pq.
 */
QObject *CMeta::createObjectFromVariant(QVariant &variant){
    QObject *obj=(QObject*)QMetaType::create(variant.userType(),variant.data());
    return obj;
}

bool CMeta::containsProperty(QObject *obj, const QString &propertyName){
    const QMetaObject *metaobject = obj->metaObject();
      int count = metaobject->propertyCount();
      for (int i=0; i<count; ++i) {
          QMetaProperty metaproperty = metaobject->property(i);
          if(QString(metaproperty.name())==propertyName)return true;
      }
      return false;
}


/*
 *
 */
bool CMeta::mapProperties(QObject *from, QObject *to){
    BLogger::assertError(from,0,__FUNCTION__,__LINE__);
    BLogger::assertError(to,0,__FUNCTION__,__LINE__);
    const QMetaObject *fromMeta=from->metaObject();
    const QMetaObject *toMeta=to->metaObject();
    for(int pFrom=0;pFrom<fromMeta->propertyCount();pFrom++){
        const char * propertyFromName=fromMeta->property(pFrom).name();
         int indexTo=toMeta->indexOfProperty(propertyFromName);
         if(indexTo!=-1){
             //qDebug()<<"setando prop"<<propertyFromName;
             to->setProperty(propertyFromName,from->property(propertyFromName));
         }
    }
	return true;
}


/*
 *vList must be of type QList<QObject*> or derivative object
 */
bool CMeta::deleteListObjects(QVariantList &vList){
    //QList<QObject*> objectList = *reinterpret_cast<QList<QObject*>*>(&vList);
    //qDeleteAll(objectList); assim trava
    foreach(QVariant v, vList){
        QObject *o=v.value<QObject*>();
        delete o;
    }
    return true;
}

QVariant CMeta::getPropertyValue(QObject *obj, const QString &propertyName){
  return obj->property(propertyName.toLatin1());
}
