#ifndef CTEZINE_H
#define CTEZINE_H
#include <QPointer>
#include <QHash>
#include <QVariant>
#include <QDate>
#include <QJSValue>
#include "ctezinebase.h"
#include "cpplib_global.h"

class EWS;
class ELoginResponse;
class QJsonRpcMessage;


/*
 * Aqui ficam todas as funcoes que podem ser invocadas no qml
 */
class CPPLIB_EXPORT CTezine : public CTezineBase {
    Q_OBJECT

public:
    static CTezine* obj(){if(!o) o=new CTezine(); return o; }
    Q_INVOKABLE void debug(const QVariant& v1, const QVariant& v2=QVariant(), const QVariant& v3=QVariant(), const QVariant& v4=QVariant(), const QVariant& v5=QVariant());
    Q_INVOKABLE void emitAppleDeviceToken(const QString &token){appleDeviceToken=token; emit sigAppleDeviceTokenReceived(token);}
    Q_INVOKABLE void emitApplePushReceived(const QString &msg);
    Q_INVOKABLE void emitSigRestError(const QString &msg){emit sigRestError(msg);}
    Q_INVOKABLE void quit();
    Q_INVOKABLE bool isNumber(const QString& txt);
    Q_INVOKABLE bool isInteger(const QString& txt);
    Q_INVOKABLE bool isPreview(){return previewMode; }
    Q_INVOKABLE QJSValue createRestObj();
    Q_INVOKABLE void setScreenOrientationMask(int orientation);
    Q_INVOKABLE QJSValue createEntity(const QString &entityFileName);
    Q_INVOKABLE QString convertObjectToJson(QJSValue jsValue);
	Q_INVOKABLE void closeDialogs(QObject *currentStackItem);
	Q_INVOKABLE void openUrl(const QString &url);
    Q_INVOKABLE void shareText(const QString &content);    
    Q_INVOKABLE void driveToLatLong(double latitude, double longitude);
    Q_INVOKABLE void driveToLatLongUsingGoogle(double latitude, double longitude);
    Q_INVOKABLE void driveToAddress(const QString &address);
    Q_INVOKABLE void requestAndroidCameraPermission();
    Q_INVOKABLE bool isDesktop();
    Q_INVOKABLE bool isIOS();
    Q_INVOKABLE bool isAndroid();
    Q_INVOKABLE QString getOSVersion();
    Q_INVOKABLE void setRestAutoCancelPreviousRequest(bool autoCancel);
    Q_INVOKABLE QString tr(const QString &txt){return txt;}
    Q_INVOKABLE QString createGUID();
    Q_INVOKABLE QString getDataLocation();
    Q_INVOKABLE QString readFileInBase64(const QString &completeFilePath);
    Q_INVOKABLE QString getAppleDeviceToken(){return appleDeviceToken;}
    Q_INVOKABLE bool setQObjectProperty(QObject *obj, const QString &propertyName, const QVariant &propertyValue);
    Q_INVOKABLE void logError(QJSValue error);
    Q_INVOKABLE void setValue(const QString &name, QJSValue jsValue);
    Q_INVOKABLE QJSValue getValue(const QString &key);
    Q_INVOKABLE bool containsKey(const QString &key);
    Q_INVOKABLE bool rotateImageFile(const QString &completeFilePath, int rotation);
    Q_INVOKABLE void removeKey(const QString &key);
    Q_INVOKABLE QString getIpAddress();
    void setLoginResponse(ELoginResponse *eLoginResponse);
    bool getAutoCancelPreviousRequest(){return autoCancelPreviousRequest;}//chamado pelo rest.cpp
    void setPreviewMode(bool isPreview){previewMode=isPreview;}

    //date and time below
    Q_INVOKABLE QDate convertFromTextToDate(const QString& isoDateTxt);
    Q_INVOKABLE QString formatISODateTimeToBRString(const QString &isoDateTimeTxt);
    Q_INVOKABLE QString convertFromDateToText(const QDate& dt);
    Q_INVOKABLE QDate addMonths(const QDate &dt, int nMonths);
    Q_INVOKABLE QDate addDays(const QDate &dt, int days);
    Q_INVOKABLE qint32 daysTo(const QDate &startDate, const QDate &endDate);
    Q_INVOKABLE QDate setYear(const QDate &dt,int year);
    Q_INVOKABLE QDate convertToDate(const QDateTime &dateTime);
    Q_INVOKABLE QString convertDateToBRString(const QDate &dt);
    Q_INVOKABLE QDate convertToDateOnly(const QDateTime &dateTime);
    Q_INVOKABLE QString convertToDateISOString(const QDateTime &dateTime);
    Q_INVOKABLE QString convertToDateTimeISOString(const QDateTime &dateTime);
    Q_INVOKABLE QDate convertToUTCDate(const QDateTime &dateTime);
    Q_INVOKABLE QString convertToUTCDateTimeString(const QDateTime &dateTime);
    Q_INVOKABLE QDateTime convertFromEpochToDateTime(qint64 ePochMsecs){return QDateTime::fromMSecsSinceEpoch(ePochMsecs);}
    Q_INVOKABLE int getAvailableHoursUntilAppointment(const QDate &appointmentDate, const QTime &appointmentTime);
    Q_INVOKABLE int compareTimes(const QTime &tm1, const QTime &tm2);
    Q_INVOKABLE bool areDatesEqual(const QDate &dt1, const QDate &dt2);
    Q_INVOKABLE QDate getTodayDate();
    Q_INVOKABLE QDateTime getNow();
    Q_INVOKABLE QTime getCurrentTime();
    Q_INVOKABLE QTime convertStringToTime(const QString &tm);
    Q_INVOKABLE int getOffsetFromUTC();
    Q_INVOKABLE QDate getFirstDayOnMonth(const QDateTime &dt);
    Q_INVOKABLE QTime convertToTime(qint32 hour, qint32 minute);
    Q_INVOKABLE QString convertToTimeString(const QTime &tm);
    Q_INVOKABLE QTime createNullTime(){return QTime();}
    Q_INVOKABLE bool isValidTime(const QString &txt);
    Q_INVOKABLE bool isValidDate(const QDate &dt);
    Q_INVOKABLE bool isNullTime(const QTime &tm){return tm.isNull();}
    Q_INVOKABLE QString getPeriod(const QTime &start, const QTime &end);
    Q_INVOKABLE bool injectIntoTypescript(const QString &propertyName, QJSValue jsValue);    

signals:
    void sigAppleDeviceTokenReceived(const QString &token);
    void sigRestError(const QString &message);

public slots:


private:
    QHash<QString,QJSValue> jsValueHash;
    bool autoCancelPreviousRequest;
    explicit CTezine(QObject* parent = 0);
    static QPointer<CTezine> o;
    QString currentCodeUniqueID;
    QString availableAppVersion;
    QString appleDeviceToken;
};
#endif
