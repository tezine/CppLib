#include "dstore.h"
#include "blogger.h"

QPointer<DStore> DStore::staticObj=0;
QMetaObject DStore::getMeta(){
    if(QMetaType::type("DStore*")==0){
        qRegisterMetaType<DStore*>();
        qRegisterMetaType<QList<DStore*> >();
    }
    return DStore::staticMetaObject;
}


/**
 *@brief Constructs an empty DStore object.
 *@completename DStore()
 *@level model
 *@type constructor
 */
DStore::DStore(): QObject(){
    currentUserID=0;
    currentPlaceID=0;
    revendaID=0;
    membroConselho=false;
    caminhaoID=0;
    otherUserID=0;
    currentUserNumber="";
    isProfileComplete=false;
}



void DStore::saveValue(const QString &key, QJSValue jsValue){
    jsValueHash[key]=jsValue;
}

QJSValue DStore::getValue(const QString &key){
    if(!jsValueHash.contains(key)){BLogger::debug(this,__FUNCTION__,"key not found in hash:"+key);return QJSValue();}
    return jsValueHash[key];
}

void DStore::setAdicionaisHabilitados(bool adicionaisHabilitados){
    if (m_adicionaisHabilitados == adicionaisHabilitados)return;
    m_adicionaisHabilitados = adicionaisHabilitados;
    emit sigAdicionaisHabilitadosChanged(adicionaisHabilitados);
}
