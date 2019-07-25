#ifndef REST_H
#define REST_H
#include <QPointer>
#include <QNetworkReply>
#include <QJSValue>
#include <QQuickItem>
#include <QNetworkAccessManager>
#include "cpplib_global.h"

class QNetworkRequest;
class RestReply;


/**
 * @brief C++ Restful implementation
 */
class CPPLIB_EXPORT Rest : public QObject{
    Q_OBJECT
    Q_PROPERTY(QVariantMap parameters READ getParameters WRITE setParameters NOTIFY parametersChanged)
    Q_PROPERTY(QString baseUrl READ getBaseURL WRITE setBaseUrl NOTIFY baseURLChanged)
    Q_PROPERTY(QString route READ getRoute WRITE setRoute NOTIFY routeChanged)
    Q_PROPERTY(QStringList urlSegments READ getUrlSegments WRITE setUrlSegments NOTIFY urlSegmentsChanged)
    Q_PROPERTY(QQuickItem* loadingObject READ getLoadingObject WRITE setLoadingObject NOTIFY loadingObjectChanged)
    Q_PROPERTY(QString body READ getBody WRITE setBody NOTIFY bodyChanged)

public:
    struct STRReceiver {
        QObject* obj;
        QByteArray slotName;
        bool isCallBack;
        QJSValue callBackFunction;
        QString completeUrl;
    };
    explicit Rest(QObject *parent=NULL);
    ~Rest(){/*qDebug()<<"destruiu rest ===================================================================================="<<this;*/}
    Q_INVOKABLE QNetworkReply* get();
    Q_INVOKABLE void put();
    Q_INVOKABLE void post();
    Q_INVOKABLE void del();
    Q_INVOKABLE void get(QJSValue callBackFunction);
    Q_INVOKABLE QObject* getForTypescript(QJSValue progressBarValue);
    Q_INVOKABLE QObject* putForTypescript(QJSValue progressBarValue);
    Q_INVOKABLE QObject* postForTypescript(QJSValue progressBarValue);
    Q_INVOKABLE QObject* delForTypescript(QJSValue progressBarValue);
    Q_INVOKABLE void post(QJSValue callBackFunction);
    Q_INVOKABLE void put(QJSValue callBackFunction);
    Q_INVOKABLE void del(QJSValue callBackFunction);
    Q_INVOKABLE void post(QObject *objReceiver, const char *slotResponseName);
    Q_INVOKABLE Rest* clear();
    Q_INVOKABLE Rest* setBaseUrl(const QString &baseUrl);
    Q_INVOKABLE Rest *setRoute(const QString &route);
    Q_INVOKABLE Rest *setToken(const QString &token);
    Q_INVOKABLE Rest* setParameters(QVariantMap parameters);
    Q_INVOKABLE Rest *appendParameter(const QString &parameterName, const QString &value);
    Q_INVOKABLE Rest *setParametersObject(QJSValue parameters);
    Q_INVOKABLE Rest* setUrlSegments(const QStringList &urlSegments);
    Q_INVOKABLE Rest *appendUrlSegment(const QString &urlSegment);
    Q_INVOKABLE Rest* setBody(const QString &body);    
    static QPointer<QNetworkAccessManager> staticAccessManager;

signals:
    void sigProgress(bool inProgress, int percentage=-1);
    void sigSessionExpired();
    void sigNetworkError(const QString &errorName);    
    void parametersChanged(QVariantMap parameters);
    void baseURLChanged(QString baseUrl);
    void routeChanged(QString route);
    void success(QByteArray response);
    void error(QString message);
    void urlSegmentsChanged(QStringList urlSegments);
    void loadingObjectChanged(QQuickItem* loadingObject);
    void bodyChanged(QString body);
    void sigCancelAllRequests();

private slots:
    void slotReplyFinished(QNetworkReply *reply);
    void slotReplyError(QNetworkReply::NetworkError error);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QUrl configureUrl(const QString &baseUrl, const QString &route, const QStringList &urlSegments, const QVariantMap &parameters);
    void configureCallbackReceiver(QJSValue callBackFunction, const QString &completeServerUrl, quint64 msgID);
    void configureSLOTReceiver(QObject *objReceiver, const QString &completeServerUrl, qint64 msgID, const char *slot);
    void invokeCallBack(QJSValue callBackFunction, const QString &completeServerFunction, const QByteArray &array);
    void invokeSLOT(const QString &requestURL, QObject *obj, const QByteArray &slot, const QByteArray &response);
    void configureRequest(QNetworkRequest &request, qint64 msgID=-1);
    QByteArray configureBody(QJSValue body);
    void configureReply(QNetworkReply *reply);
    QVariantMap getParameters() const{return parameters;}
    QString getBaseURL() const{return baseUrl;}
    QString getRoute() const{return route;}
    QStringList getUrlSegments() const{return urlSegments;}
    QString getBody() const{return body;}
    void setLoadingObject(QQuickItem *loadingObject);
    QQuickItem* getLoadingObject() const{return loadingObject;}
    QByteArray acceptHeader() const{return m_acceptHeader;}
    QByteArray accept() const{return m_accept;}
    QByteArray authTokenHeader() const{return m_authTokenHeader;}
    QByteArray authToken() const{return m_authToken;}
    QByteArray m_acceptHeader;
    QByteArray m_accept;
    QByteArray m_authToken;
    QByteArray m_authTokenHeader;
    qint64 messageID;
    QHash<quint64, STRReceiver*> receiverHash;
    QVariantMap parameters;
    QString baseUrl;
    QString token;
    QString route;
    QString lastError;
    QNetworkAccessManager *manager;
    QStringList urlSegments;
    QPointer<QQuickItem> loadingObject;
    QString body;
    bool isNewRest;
};
#endif
