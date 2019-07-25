#include <QtCore>
#include "ctezinebaseproperties.h"
#include "cbase.h"

CTezineBaseProperties::CTezineBaseProperties(QObject *parent): QObject(parent){
    this->hotReload=false;
}

/*
 *Hack porque QApplication::desktop()->availableGeometry(); retorna errado
 */
void CTezineBaseProperties::setScreenHeight(qint32 height){
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
    qDebug()<<"setando novo height:"<<height;
    screenHeight=height;
#endif
}


/*
 *Hack porque QApplication::desktop()->availableGeometry(); retorna errado
 */
void CTezineBaseProperties::setScreenWidth(qint32 width){
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)|| defined(Q_OS_WINPHONE)
    screenWidth=width;
#endif
}


/*
 *Returns the current platform
 */
qint32 CTezineBaseProperties::getPlatform(){
#ifdef Q_OS_ANDROID
    return CBase::PlatformAndroid;
#endif

#ifdef Q_OS_WIN
    return CBase::PlatformWindows;
#endif

#ifdef Q_OS_MAC
    return CBase::PlatformMac;
#endif

#ifdef Q_OS_IOS
    return CBase::PlatformIOS;
#endif
}


/*
 *Returns the storage path.
 *It's where all files, like sqlite is kept.
 */
QString CTezineBaseProperties::getStoragePath(){
    return CBase::getWikiosStoragePath();
}
