#include <QtDebug>
#include "baseitem.h"

BaseItem::BaseItem(){

}

void BaseItem::componentComplete(){
    QQuickItem::componentComplete();
    qDebug()<<"chamou o componentOnComplete";
}
