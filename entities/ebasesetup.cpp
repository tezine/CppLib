#include <QtCore>
#include "blogger.h"
#include "ebasesetup.h"
#include "qmetaobject.h"

EBaseSetup::EBaseSetup(QObject *parent):QObject(parent){
	width=0;
	height=0;
}

