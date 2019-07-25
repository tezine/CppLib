#include <QGuiApplication>
#include <QtCore>
#include <QtQuick>
#include "ctezinebase.h"
#include "cbase.h"
#include "tpreference.h"
#include <QScreen>
#include "Sql"
#include "blogger.h"
#include "ebasesetup.h"
#include "cmeta.h"
#include "dstore.h"
#include "cenums.h"
#include "cdefines.h"
#include "cqml.h"
#include "njsonrpc.h"
#include "ctezine.h"
#include "qdrawline.h"
#include "pandroid.h"
#include "rest.h"
#include "baseitem.h"
#include "csqlimageprovider.h"
#include "tquickitem.h"
#include "enhancedqmlapplicationengine.h"
#include "firebase.h"
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include "jni.h"
#endif


static QObject *getRestSingleton(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new Rest();
}

static QObject* getTezineSingleton(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return CTezine::obj();
}

CTezineBase::CTezineBase(QObject *parent) :CTezineBaseProperties(parent){
}


/*
 *Setups the app initialization
 *Set all font's sizes and margins according to the user's screen
 */
void CTezineBase::setup(EBaseSetup &setup){
    QGuiApplication::setOrganizationName(setup.getAppOrganizationName());
    QGuiApplication::setOrganizationDomain(setup.getAppOrganizationDomain());
    QGuiApplication::setApplicationName(setup.getAppName());//no deskto fica em C:/Users/<USER>/AppData/Roaming/<APPNAME>
    CMeta::obj()->setup(setup.getEntities());
    DStore::obj()->saveValue("svcBaseAddress", QJSValue(setup.getSvcBaseAddress()));
    DStore::obj()->saveValue("svcAddress",QJSValue(setup.getSvcCompleteAddress()));
    NJsonRpc::obj()->setServerUrl(setup.getSvcCompleteAddress());
    qmlRegisterType<TQuickItem>("com.tezine.base", 1, 0, "TQuickItem");
    qmlRegisterType<QDrawLine>("com.tezine.base", 1, 0, "QDrawLine");
    qmlRegisterType<Rest>("com.tezine.base", 1, 0, "Rest");
    qmlRegisterType<BaseItem>("com.tezine.base", 1, 0, "BaseItem");
    EnhancedQmlApplicationEngine *qmlEngine=setup.getQmlEngine();
    if(qmlEngine==nullptr){BLogger::error(this,__FUNCTION__,"QML Engine not set");return;}
    CQML::obj()->setQMLEngine(qmlEngine);
    qmlEngine->rootContext()->setContextProperty("base", CTezine::obj());
    qmlEngine->rootContext()->setContextProperty("dstore", DStore::obj());
    qmlEngine->rootContext()->setContextProperty("njsonrpc", NJsonRpc::obj());
    qmlEngine->rootContext()->setContextProperty("rest", new Rest());
    qmlEngine->rootContext()->setContextProperty("firebase", FireBase::obj());
    setupScreen(setup.getWidth(),setup.getHeight());
    previewMode=setup.getIsPreview();
    qmlRegisterSingletonType<Rest>("com.tezine.basesingletons", 1, 0, "Rest", getRestSingleton);//to allow access from jscripts
    qmlRegisterSingletonType<CTezine>("com.tezine.basesingletons", 1, 0, "Base", getTezineSingleton);//to allow access from jscript

    if(setup.getIsPreview())return;
    if(!setup.getResourceDBPath().isEmpty()){
        qDebug()<<"vai ler "<<setup.getResourceDBPath();
        if(!QFile::exists(setup.getResourceDBPath())){BLogger::error(this,__FUNCTION__,setup.getResourceDBPath()+" does not exist");exit(1);}
        QFileInfo dbFileInfo(setup.getResourceDBPath());
        QString sqliteFolder=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(sqliteFolder);
        if(!dir.mkpath(sqliteFolder)){BLogger::error(this,__FUNCTION__,"unable to create path for sqlite");exit(1);}
        dbFilePath=sqliteFolder+"/"+dbFileInfo.fileName();
        QFile configFile(sqliteFolder+"/app.config");
        //qDebug()<<"app.config path:"<<sqliteFolder+"/app.config"<<". DbPath:"<<dbFilePath;
        if(!configFile.open(QIODevice::ReadWrite | QIODevice::Text)){BLogger::error(this,__FUNCTION__,"unable to open app.config");exit(1);}
        if(!QFile::exists(dbFilePath)){//let's create the sqlite and app.config files
            BLogger::debug(this,__FUNCTION__,"creating db " +dbFilePath);
            if(!QFile::copy(setup.getResourceDBPath(), dbFilePath)){BLogger::error(this,__FUNCTION__,"unable to copy "+setup.getResourceDBPath());exit(1);}
            QFile::setPermissions(dbFilePath,QFile::WriteOwner | QFile::ReadOwner);
        }else if(setup.getRecreateDBIfNewVersion()){//sqlite file exists
            QString configFileVersion=configFile.readLine().trimmed();
            if(configFileVersion!=setup.getAppVersion()){
                QFile::copy(setup.getResourceDBPath(), dbFilePath);
                QFile::setPermissions(dbFilePath,QFile::WriteOwner | QFile::ReadOwner);
            }
        }
        configFile.resize(0);
        QTextStream out(&configFile);
        out<<setup.getAppVersion()<<endl;
        configFile.close();
    }
}


/*
 *Setups the screen height/width
 */
void CTezineBase::setupScreen(qint32 desktopWidth, qint32 desktopHeight){
    qreal physicalDotsPerInch=getPixelDensity();// screen->physicalDotsPerInchX();//TODO alterar para physicalDotsPerInch. é que physicalDotsPerInch retornava infinito devido ao bug do qt 5.3.1
#if defined(Q_OS_WIN) || defined(Q_OS_MACX)
    screenHeight=desktopHeight;//screen->geometry().height();
    screenWidth=desktopWidth;//screen->geometry().width();
    qDebug()<<"(CTezineBase)setupScreen. setando height:"<<screenHeight<<".width:"<<screenWidth;
    screenHeight=desktopHeight;
    screenWidth=desktopWidth;
    //fonts
    tinyFontPointSize=8;
    smallFontPointSize=9;
    mediumFontPointSize=10;
    largeFontPointSize=11;
    hugeFontPointSize=12;
    imenseFontPointSize=13;
    pointSize1=8;
    pointSize2=9;
    pointSize2=10;
    pointSize3=11;
    pointSize4=12;
    pointSize5=13;
    pointSize6=14;
    pointSize7=15;
    pointSize8=16;
    pointSize9=17;
    pointSize10=18;
    pointSize11=19;
    pointSize12=20;
    pointSize13=21;
    pointSize14=22;
    //layout
    layoutSpacing=10;
#endif
#ifdef Q_OS_ANDROID
    QScreen *screen=qApp->primaryScreen();
    screenHeight=screen->geometry().height();
    qDebug()<<"(CWikiosBase)setup. setando height:"<<screenHeight<<"physicalDotsPerInch:"<<screen->physicalDotsPerInchX()<<screen->physicalDotsPerInchY();
    screenWidth=screen->geometry().width();
    layoutSpacing=physicalDotsPerInch*10/84;
    //fonts
    tinyFontPointSize=8;
    smallFontPointSize=10;
    mediumFontPointSize=14;
    largeFontPointSize=17;
    hugeFontPointSize=20;
    imenseFontPointSize=25;
    pointSize1=8;
    pointSize2=11;
    pointSize3=14;
    pointSize4=17;
    pointSize5=20;
    pointSize6=23;
    pointSize7=26;
    pointSize8=29;
    pointSize9=32;
    pointSize10=35;
    pointSize11=38;
    pointSize12=41;
    pointSize13=44;
    pointSize14=17;
#endif
#ifdef Q_OS_IOS
    QScreen *screen=qApp->primaryScreen();
    screenHeight=screen->geometry().height();
    qDebug()<<"(CWikiosBase)setup. setando height:"<<screenHeight<<"physicalDotsPerInch:"<<screen->physicalDotsPerInchX()<<screen->physicalDotsPerInchY();
    screenWidth=screen->geometry().width();
    layoutSpacing=physicalDotsPerInch*10/84;
    //fonts
    tinyFontPointSize=8;
    smallFontPointSize=10;
    mediumFontPointSize=14;
    largeFontPointSize=17;
    hugeFontPointSize=20;
    imenseFontPointSize=25;
    pointSize1=8;
    pointSize2=11;
    pointSize3=14;
    pointSize4=17;
    pointSize5=20;
    pointSize6=23;
    pointSize7=26;
    pointSize8=29;
    pointSize9=32;
    pointSize10=35;
    pointSize11=38;
    pointSize12=41;
    pointSize13=44;
    pointSize14=17;
#endif
    //margins
    horizontalMargin1=screenWidth*0.02;
    horizontalMargin2=screenWidth*0.04;
    horizontalMargin3=screenWidth*0.06;
    horizontalMargin4=screenWidth*0.08;
    horizontalMargin5=screenWidth*0.1;
    verticalMargin1=screenHeight*0.02;
    verticalMargin2=screenHeight*0.04;
    verticalMargin3=screenHeight*0.06;
    verticalMargin4=screenHeight*0.08;
    verticalMargin5=screenHeight*0.1;
    tinyMargin=physicalDotsPerInch*3/84;// o 84 veio do physicalDotsPerInch no windows
    smallMargin=physicalDotsPerInch*5/84;
    mediumMargin=physicalDotsPerInch*10/84;
    largeMargin=physicalDotsPerInch*15/84;
    hugeMargin=physicalDotsPerInch*20/84;
    imenseMargin=physicalDotsPerInch*25/84;
    scrollWidth=screenWidth*0.014;
}



/*
 *Returns true if app is executing in a HDPI Monitor
 */
bool CTezineBase::isHDPI(){
    QScreen *screen=qApp->primaryScreen();
    qint32 screenWidth=screen->geometry().width();
    if(screenWidth>3000)return true;
    else return false;
}

/*
 *Sets the light theme
 */
void CTezineBase::setLightTheme(){
    tabUnselectedColor="#e0e0e0";
    tabSelectedColor="#78c6e3";
    toolbarColor="#dddddd";
    buttonNormalColor="#cecece";
    buttonFocusedColor="#cecece";
    buttonPressedColor="#ababab";
    buttonFocusedDisabledColor="#e7e7e7";
    buttonDisabledColor="#e7e7e7";
    switchBackColor="#d7d7d7";
    switchButtonOnColor="#2ba5ce";
    switchButtonOffColor="#bebebe";
    sectionNormalColor="#f2f2f2";
    sectionPressedColor="#e0e0e0";
    sectionActivatedColor="#59c1e8";
    sectionFocusedColor="#b8e0ee";
    sectionDisabledFocusedColor="#b8e0ee";
    sectionDisabledColor="#f2f2f2";
    backColor="#f2f2f2";
    fontColor="#222222";
    secondLineFontColor="#777777";
    tabFontColor="#3d3d3d";
    buttonFontColor="#333333";
    dlgTitleBarColor="#cecece";
    textFieldBackColor="#f2f2f2";
    menuItemBackColor="#f5f5f5";
    comboNormalColor="#f2f2f2";
    comboFocusedColor="#f2f2f2";
    comboPressedColor="#7fcdea";
    comboDisabledFocusedColor="#b8e0ee";
    comboDisabledColor="#f2f2f2";
    progressBackColor="#a9a9a9";
    progressForeColor="#33b5e5";
    sectionSeparatorColor="#d9d9d9";//se for dark é 292929
    scrollBarIcon="qrc:///Images/scrollbar.png";
}


qint32 CTezineBase::getPixelDensity(){
    int density;
#ifdef Q_OS_ANDROID
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    density = displayMetrics.getField<int>("densityDpi");
#else
    QScreen *screen=qApp->primaryScreen();
    density= screen->physicalDotsPerInch();
#endif
    //qDebug()<<"retornando densidade:"<<density;
    return density;
}


void CTezineBase::setDarkTheme(){
    tabUnselectedColor="#333333";
    tabSelectedColor="#33819e";
    toolbarColor="#222222";
    buttonNormalColor="#474747";
    buttonFocusedColor="#474747";
    buttonPressedColor="#7e7e7e";
    buttonFocusedDisabledColor="#212121";
    buttonDisabledColor="#212121";
    switchBackColor="#3a3a3a";
    switchButtonOnColor="#0c86af";
    switchButtonOffColor="#474747";
    sectionNormalColor="#111111";
    sectionPressedColor="#404040";
    sectionActivatedColor="#2c94bb";
    sectionFocusedColor="#1b4351";
    sectionDisabledFocusedColor="#1b4351";
    sectionDisabledColor="#111111";
    backColor="#111111";
    fontColor="#ffffff";
    secondLineFontColor="#bbbbbb";
    tabFontColor="#ffffff";
    buttonFontColor="#ffffff";
    dlgTitleBarColor="#474747";
    textFieldBackColor="#111111";
    menuItemBackColor="#282828";
    comboNormalColor="#111111";
    comboFocusedColor="#111111";
    comboPressedColor="#257390";
    comboDisabledFocusedColor="#1b4351";
    comboDisabledColor="#111111";
    progressBackColor="#292929";
    progressForeColor="#33b5e5";
    sectionSeparatorColor="#292929";
    scrollBarIcon="qrc:///Images/scrollbardark.png";
}
