#include <QtCore>
#include <QtQml>
#include <QJSValueList>
#include <QQmlEngine>
#include <QQmlContext>
#include "tquickitem.h"
#include "ctezine.h"
#include "enhancedqmlapplicationengine.h"
#include "cqml.h"
#include "cmeta.h"
#include "inject.h"

TQuickItem::TQuickItem(QQuickItem *parent): QQuickItem (parent){    
}


void TQuickItem::componentComplete(){
    QQuickItem::componentComplete();
    EnhancedQmlApplicationEngine *engine=CQML::obj()->getQMLEngine();
    //hack para verificar a classe do QML caso nao esteja especificado o objectName.
    QString qmlComponentName;
    if(!objectName().isEmpty())qmlComponentName=objectName();
    else {
        auto name=QString(this->metaObject()->className());// ex: VLogin_QMLTYPE_27
        qmlComponentName= name.left(name.indexOf("_QMLTYPE"));
        //.remove("_QMLTYPE_14");
    }

    //qDebug()<<"(TQuickItem)componentComplete:"<<objectName()<<engine->rootContext()->nameForObject(this)<< this->metaObject()->className();//deveria retornar pelo id
    //parece que nao é possivel acessar o id. Veja em \http://doc.qt.io/qt-5/qtqml-syntax-objectattributes.html#the-id-attribute
    //bool containsID= CMeta::obj()->containsProperty(this,"id");
    auto base=CTezine::obj();
    QString completeModulePath(base->getQmlPagesFolder()+ "/"+qmlComponentName+".mjs");
    qDebug()<<"(TQuickItem)componentOnComplete: Instantiating "+completeModulePath;
    QJSValue jsModule= engine->importModule(completeModulePath);
    QJSValue vloginJS=jsModule.property(qmlComponentName);//ex: VLogin

    /*
         //QHash<QString, QJSValue> hashInjects=base->getTypescriptInjects();*
    QJSValue baseJS= engine->newQObject(base);
    engine->setObjectOwnership(baseJS.toQObject(),QQmlEngine::CppOwnership);//para nao ser destruido
    Inject *injects=Inject::obj();
//    QVariant baseVariant=QVariant::fromValue(base); assim nao funciona
//    injects->setProperty("base",baseVariant);
    QJSValue injectsJS= engine->newQObject(injects);
    engine->setObjectOwnership(injectsJS.toQObject(),QQmlEngine::CppOwnership);//para nao ser destruido
    injectsJS.setProperty("base",baseJS);

    //deixar de usar isso quando enviar somente o base para as funcoes rest
    QJSValue eCppObjectsJS= engine->newObject();
    eCppObjectsJS.setProperty("base",baseJS);
    injectsJS.setProperty("eCppObjects",eCppObjectsJS);
    QHashIterator<QString, QJSValue> i(hashInjects);
      while (i.hasNext()) {
          i.next();
          qDebug()<<"injected property:"<< i.key();
           i.value();
           injectsJS.setProperty(i.key(),i.value());
      }
*/

    //vamos enviar esse objeto aqui como sendo o form
    QJSValue formJS= engine->newQObject(this);
    engine->setObjectOwnership(formJS.toQObject(),QQmlEngine::CppOwnership);//para nao ser destruido
    QJSValue vLoginInstance= vloginJS.callAsConstructor(QJSValueList()<<formJS<<CTezine::obj()->getInjectJS());


}
