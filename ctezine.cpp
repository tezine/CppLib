#include <QGuiApplication>
#include <QtQuick>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include "cqml.h"
#include "ctezine.h"
#include "Sql"
#include "blogger.h"
#include "cdefines.h"
#include "cbase.h"
#include "eloginresponse.h"
#include "json.h"
#include "dstore.h"
#include "cmeta.h"
#include "pandroid.h"
#include "pios.h"
#include "rest.h"
#include "qjsonrpcmessage.h"
#include "njsonrpc.h"
#include "enhancedqmlapplicationengine.h"
#include "cqml.h"
#include "inject.h"
#include "firebase.h"

QPointer<CTezine>  CTezine::o=0;
CTezine::CTezine(QObject *parent) :CTezineBase(parent){
    previewMode=true;
    screenHeight=0;
    screenWidth=0;
    autoCancelPreviousRequest=true;
    currentCodeUniqueID="";
    setLightTheme();
}


void CTezine::setScreenOrientationMask(int orientation){
    Qt::ScreenOrientations s;
    s|=(Qt::ScreenOrientation) orientation;
    qApp->primaryScreen()->setOrientationUpdateMask(s);
}


/*
 *Creates a qml entity
 * Dynamic QOBJECT properties nao sao acessadas no qml
 */
QJSValue CTezine::createEntity(const QString &entityFileName){  
    QObject *obj= CMeta::obj()->createObject(entityFileName);
    if(!obj)return QJSValue();
    QJSValue jsValue=CQML::obj()->getQMLEngine()->newQObject(obj);
    obj->deleteLater();
    return jsValue;
}

bool CTezine::setQObjectProperty(QObject *obj, const QString &propertyName, const QVariant &propertyValue){
    qDebug()<<"setando object property:"<<obj<<propertyName<<propertyValue;
    return true;
}

void CTezine::logError(QJSValue error){
    qDebug()<<"(CTezine)logError:"<<error.isObject()<<error.isQObject()<< error.toString();
}



/*
 *Converts the QObject to a JSON object
 */
QString CTezine::convertObjectToJson(QJSValue jsValue){
    if(!jsValue.isObject() || jsValue.isNull()){BLogger::error(this,__FUNCTION__,"jValue not object or null");return "error";}
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QJSValueIterator it(jsValue);
    while (it.hasNext()) {
	it.next();
	//qDebug() << it.name() << ": " << it.value().toString();
	if(it.value().isBool())jsonObj.insert(it.name(),it.value().toBool());
	else if(it.value().isDate())jsonObj.insert(it.name(),it.value().toDateTime().toString(Qt::ISODate));
	else if(it.value().isNumber())jsonObj.insert(it.name(),it.value().toNumber());
	else if(it.value().isString())jsonObj.insert(it.name(),it.value().toString());
    }
    jsonDoc.setObject(jsonObj);
    QString result=jsonDoc.toJson();
    qDebug()<<"(EWikios)convertObjectToJson:"<<result;
    return result;
}



void CTezine::closeDialogs(QObject *currentStackItem){
	//qDebug()<<"lista:"<<MShared::obj()->getEngine()->rootObjects();
	if(!currentStackItem)return;
	const QObjectList childList= currentStackItem->children();
	foreach(QObject *o, childList){
		//qDebug()<<o->metaObject()->className();
		closeDialogs(o);
		QString className(o->metaObject()->className());
		if(className.contains("Dlg")){
			//qDebug()<<"fechando:"<<className;
			if(o->metaObject()->indexOfMethod("close()")==-1)continue;
			o->metaObject()->invokeMethod(o,"close");
		}
	}
	//qDebug()<<"vai fechar os dialogs"<<currentStackItem;

}


/*
 *Returns true if running on a desktop
 */
bool CTezine::isDesktop(){
#if defined(Q_OS_MACX) || defined(Q_OS_WIN)
    return true;
#endif
    return false;
}


bool CTezine::isIOS(){
#ifdef Q_OS_IOS
    return true;
#endif
    return false;
}

bool CTezine::isAndroid(){
#ifdef Q_OS_ANDROID
    return true;
#endif
    return false;
}

QString CTezine::getOSVersion(){
    return QSysInfo::productVersion();
}


void CTezine::setRestAutoCancelPreviousRequest(bool autoCancel){
    this->autoCancelPreviousRequest=autoCancel;
}


QString CTezine::createGUID(){
    return QUuid::createUuid().toString().remove('{').remove('}');
}

QString CTezine::getDataLocation(){
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
}


QString CTezine::readFileInBase64(const QString &completeFilePath){
    QFile file(completeFilePath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"failed to read file "<<completeFilePath;
        return QString();
    }
    return file.readAll().toBase64();
}

/*
 *
 */
void CTezine::openUrl(const QString &u){
	QUrl url;
	if(u.contains("www") && (!u.contains("http")))url.setUrl("http://"+u);
	else url.setUrl(u);
    QDesktopServices::openUrl(url);
}


/*
 *Shares a text with other app
 */
void CTezine::shareText(const QString &content){
#ifdef Q_OS_ANDROID
    PAndroid::obj()->shareText(content);
#endif
#ifdef Q_OS_IOS
    PIOS::obj()->shareText(content);
#endif
}


/*
 *Called after login or createUser
 */
void CTezine::setLoginResponse(ELoginResponse *eLoginResponse){
	DStore *dStore=DStore::obj();
	dStore->setCurrentUserID(eLoginResponse->getUserID());
	dStore->setCurrentUserName(eLoginResponse->getUserName());
    availableAppVersion=eLoginResponse->getSwVersion();
    dStore->setIsProfileComplete(eLoginResponse->getIsProfileComplete());
}


/*
 *Debugs anything from QML file
 */
void CTezine::debug(const QVariant &v1, const QVariant &v2, const QVariant &v3, const QVariant &v4, const QVariant &v5){
    QVariantList vList;
    if(!v1.isNull()) vList.append(v1);
    if(!v2.isNull()) vList.append(v2);
    if(!v3.isNull()) vList.append(v3);
    if(!v4.isNull()) vList.append(v4);
    if(!v5.isNull()) vList.append(v5);
}

void CTezine::emitApplePushReceived(const QString& msg){
  qDebug()<<"(CTezine)chegou push"<<msg;
  QJsonParseError jsonError;
  QJsonDocument jsonDocument=QJsonDocument::fromJson(msg.toUtf8(),&jsonError);
  if(jsonError.error!=QJsonParseError::NoError){BLogger::error(0,__FUNCTION__, "Unable to create QJsonDocument from string:"+jsonError.errorString());return ;}
  QJsonObject jsonObj=jsonDocument.object();
  QStringList jsonObjKeys=jsonObj.keys();
  if(!jsonObj.contains("data")){
    qDebug()<<"nao contem data key";
    return;
  }
  QJsonObject dataObj=jsonObj.value("data").toObject();
  EnhancedQmlApplicationEngine *engine= CQML::obj()->getQMLEngine();
  QJSValue jsObject= engine->newObject();
  foreach(QString jsonKey, dataObj.keys()){
    QJsonValue jsonValue= dataObj.value(jsonKey);
    qDebug()<<"(CTezine)emitApplePushReceived. key"<<jsonKey<<".Value"<<jsonValue.toString();
    jsObject.setProperty(jsonKey, jsonValue.toString());
    FireBase::obj()->setValue(jsonKey,jsonValue.toString());
  }
  FireBase::obj()->emitMessageReceived(jsObject);
}


QDate CTezine::convertFromTextToDate(const QString &isoDateTxt){
    QDate dt=QDate::fromString(isoDateTxt,Qt::ISODate);
    return dt;
}

QString CTezine::formatISODateTimeToBRString(const QString &isoDateTimeTxt){
    QDateTime dtTime=QDateTime::fromString(isoDateTimeTxt,Qt::ISODate);
    return dtTime.toString("dd/MM/yyyy hh:mm");
}


QString CTezine::convertFromDateToText(const QDate &dt){
    QString txt=dt.toString(Qt::ISODate);
    //qDebug()<<"convertFromDateToText:"<<txt;
    return txt;
}


/*
 *Quit the wikios app
 */
void CTezine::quit(){
    qApp->quit();
}


/*
 *Returns true if 'txt' is number
 */
bool CTezine::isNumber(const QString &txt){
    bool ok;
    txt.toDouble(&ok);
    return ok;
}


/*
 *Returns true if 'txt' is integer
 */
bool CTezine::isInteger(const QString &txt){
    bool ok;
    txt.toInt(&ok);
    return ok;
}

QJSValue CTezine::createRestObj(){
    Rest *rest=new Rest();
    CQML::obj()->getQMLEngine()->setObjectOwnership(rest,QQmlEngine::JavaScriptOwnership);//redundante
    QJSValue jsValue= CQML::obj()->getQMLEngine()->newQObject(rest);
    return jsValue;
}


QDate CTezine::addMonths(const QDate &dt, int nMonths){
    QDate d(dt);
    d=d.addMonths(nMonths);
    return d;
}

QDate CTezine::addDays(const QDate &dt, int days){
    QDate d(dt);
    d=d.addDays(days);
    return d;
}


qint32 CTezine::daysTo(const QDate &startDate, const QDate &endDate){
    return startDate.daysTo(endDate);
}


/*
 *Nao funciona dt.setFullYear
 */
QDate CTezine::setYear(const QDate &dt, int year){
    QDate d(year,dt.month(),dt.day());
    return d;
}


/*
 *No qml, nao existe date. Assim, o date vira datetime no qml.
 */
QDate CTezine::convertToDate(const QDateTime &dateTime){
    return QDate(dateTime.date().year(), dateTime.date().month(), dateTime.date().day());
}

QString CTezine::convertDateToBRString(const QDate &dt){
    return dt.toString("dd/MM/yyyy");
}


QDate CTezine::convertToDateOnly(const QDateTime &dateTime){
    //qDebug()<<"convertendo para data apenas"<<dateTime<<".Date:"<<dateTime.date()<<".Time:"<<dateTime.time();
    //qDebug()<<"offset:"<<dateTime.offsetFromUtc();
    QDate dt=dateTime.date();
    return dt;
}

/*
 *Sempre enviar datas dessa forma para o SVC
 */
QString CTezine::convertToDateISOString(const QDateTime &dateTime){
    QDate dt=QDate(dateTime.date().year(), dateTime.date().month(), dateTime.date().day());
    return dt.toString(Qt::ISODate);
}

QString CTezine::convertToDateTimeISOString(const QDateTime &dateTime){
    QDate dt=QDate(dateTime.date().year(), dateTime.date().month(), dateTime.date().day());
    QTime tm=QTime(dateTime.time().hour(),dateTime.time().minute(),dateTime.time().second());
    QDateTime dtTm=QDateTime(dt,tm);
    return dtTm.toString(Qt::ISODate);
}


QDate CTezine::convertToUTCDate(const QDateTime &dateTime){
    QDate dt=dateTime.toUTC().date();
    qDebug()<<"convertendo para utc:"<<dateTime<<".Resultado:"<<dt;
    return dt;
}

QString CTezine::convertToUTCDateTimeString(const QDateTime &dateTime){
    QDateTime utcDateTime= dateTime.toUTC();
    qDebug()<<"retornando "<<utcDateTime;
    return utcDateTime.toString(Qt::ISODate);
}

int CTezine::getAvailableHoursUntilAppointment(const QDate &appointmentDate, const QTime &appointmentTime){
    QDateTime now=QDateTime::currentDateTime();
    QDateTime appointmentDateTime=QDateTime(appointmentDate,appointmentTime);
    if(now>appointmentDateTime)return -1;
    qint64 secs=now.secsTo(appointmentDateTime);
    qint64 hoursAvailable= (secs/60)/60;
    qDebug()<<"hours until appointment:"<<hoursAvailable;
    return hoursAvailable;
}


int CTezine::compareTimes(const QTime &tm1, const QTime &tm2){
    //todo arrumar
    return 0;
}

bool CTezine::areDatesEqual(const QDate &dt1, const QDate &dt2){
    if(dt1==dt2)return true;
    else return false;
}


QDate CTezine::getTodayDate(){
    return QDate::currentDate();
}

QDateTime CTezine::getNow(){
    return QDateTime::currentDateTime();
}

QTime CTezine::getCurrentTime(){
    return QTime::currentTime();
}

QTime CTezine::convertStringToTime(const QString &tm){
    return QTime::fromString(tm,"HH:mm");
}

int CTezine::getOffsetFromUTC(){
    return QDateTime::currentDateTime().offsetFromUtc();
}


QDate CTezine::getFirstDayOnMonth(const QDateTime &dt){
    return QDate(dt.date().year(),dt.date().month(),1);
}

QTime CTezine::convertToTime(qint32 hour, qint32 minute){
    return QTime(hour,minute);
}

QString CTezine::convertToTimeString(const QTime &tm){
    return tm.toString("HH:mm");
}


bool CTezine::isValidTime(const QString &txt){
    QTime time;
    time=QTime::fromString(txt,"hh:mm");
    if(time.isValid())return true;
    time=QTime::fromString(txt,"hh:mm:ss");
    if(time.isValid())return true;
    return false;
}

bool CTezine::isValidDate(const QDate &dt){
    return dt.isValid();
}


QString CTezine::getPeriod(const QTime &start, const QTime &end){
    if(!start.isValid() || start.isNull())return "";
    if(!end.isValid() || end.isNull())return "";
    QString startString=start.toString("hh:mm");
    QString endString=end.toString("hh:mm");
    if(startString=="00:00" && endString=="00:00")return "";
    return startString+" "+ "until"+" "+endString;//todo adicionar traducao
}

bool CTezine::injectIntoTypescript(const QString &propertyName, QJSValue jsValue){
    qDebug()<<"(CTezine)injectIntoTypescript: "+propertyName;
    if(jsValue.isQObject())CQML::obj()->getQMLEngine()->setObjectOwnership(jsValue.toQObject(),QQmlEngine::CppOwnership);//para nao ser destruido pelo garbage collector
    injectJS.setProperty(propertyName,jsValue);
    injectObjects[propertyName]=jsValue;//talvez remover essa linha
    return true;
}


void CTezine::driveToLatLong(double latitude, double longitude){
    if(isAndroid()){
        QString complete="geo:"+QString::number(latitude)+","+QString::number(longitude);
        qDebug()<<"vai navegar para "<<complete;
        QDesktopServices::openUrl(QUrl(complete));
    }else if(isIOS()){
        QString complete="http://maps.apple.com/?ll="+QString::number(latitude)+","+QString::number(longitude);        
        qDebug()<<"vai navegar para "<<complete;
        QDesktopServices::openUrl(QUrl(complete));
    }else{
        QString complete="geo:"+QString::number(latitude)+","+QString::number(longitude);
        qDebug()<<"vai navegar para "<<complete;
        BLogger::debug(this,__FUNCTION__,QString::number(latitude)+","+QString::number(longitude));
    }
}


void CTezine::driveToLatLongUsingGoogle(double latitude, double longitude){
    QString complete="google.navigation:q="+QString::number(latitude)+","+QString::number(longitude)+"&mode=d";//mode d indica drive
    qDebug()<<"vai navegar usango google maps para "<<complete;
    if(!isAndroid())return;
    QDesktopServices::openUrl(QUrl(complete));
}


void CTezine::driveToAddress(const QString &address){
    if(isAndroid()){
        QString complete="geo:0,0?q="+QString(QUrl::toPercentEncoding(address));
        qDebug()<<"vai navegar para "<<complete;
        QDesktopServices::openUrl(QUrl(complete));
    }else if(isIOS()){
        QString complete="http://maps.apple.com/?q="+address;
        complete=QString(QUrl::toPercentEncoding(complete));
        qDebug()<<"vai navegar para "<<complete;
        QDesktopServices::openUrl(QUrl(complete));
    }
}

void CTezine::requestAndroidCameraPermission(){
#ifdef Q_OS_ANDROID
auto  result = QtAndroid::checkPermission(QString("android.permission.CAMERA"));
   if(result == QtAndroid::PermissionResult::Denied){
    qDebug()<<"requisitando permissao de camera...";
       QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.CAMERA"}));
       if(resultHash["android.permission.CAMERA"] == QtAndroid::PermissionResult::Denied)qDebug()<<"nao permitiu camera";
   }
#endif
}


void CTezine::setValue(const QString &key, QJSValue jsValue){
    if(jsValue.isQObject())CQML::obj()->getQMLEngine()->setObjectOwnership(jsValue.toQObject(),QQmlEngine::CppOwnership);//para nao ser destruido pelo garbage collector
    jsValueHash[key]=jsValue;
}

QJSValue CTezine::getValue(const QString &key){
    if(!jsValueHash.contains(key)){BLogger::debug(this,__FUNCTION__,"key not found in hash:"+key);return QJSValue();}
    return jsValueHash[key];
}

bool CTezine::containsKey(const QString &key){
    if(!jsValueHash.contains(key))return false;
    return true;
}

bool CTezine::rotateImageFile(const QString &completeFilePath, int rotation){
    QPixmap pixmap(completeFilePath);
    QMatrix rm;
    qDebug()<<"(CTezine)rotateImageFile"<<rotation;
    rm.rotate(rotation);
    pixmap = pixmap.transformed(rm);
    return pixmap.save(completeFilePath,"JPEG",70);
}

void CTezine::removeKey(const QString &key){
    if(!jsValueHash.contains(key))return;
    jsValueHash.remove(key);
}

QString CTezine::getIpAddress(){
    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol){
                    //if(address.ip().toString().startsWith("10."))continue;
                    //if(address.ip().toString().startsWith("192."))continue;
                    qDebug()<<"IP:"<<  address.ip().toString();
                }
            }
        }
    }

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
             qDebug() << "IP mobile:"<<address.toString();
             return address.toString();
        }
    }

    return "";
}
