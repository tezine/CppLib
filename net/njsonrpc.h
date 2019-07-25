#ifndef MSVC_H
#define MSVC_H
#include <QPointer>
#include <QVariant>
#include <QJSValue>
#include <QNetworkReply>
#include "cpplib_global.h"

class QJsonRpcHttpClient;
class QNetworkAccessManager;
class QJsonRpcMessage;


class CPPLIB_EXPORT NJsonRpc : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString mobileSeed READ getMobileSeed CONSTANT)

public:
	struct STRReceiver {
		bool isCallBack;
		QObject* obj;
		QByteArray slotName;
		QJSValue callBackFunction;
		QString completServerFunction;
	};
    static NJsonRpc *obj(){if(!o)o=new NJsonRpc();return o;}
    void call(const QString &serverClassTypeName, const QString& functionName, QObject* objReceiver, const char* slotResponseName, const QVariant &arg1=QVariant(),
              const QVariant &arg2=QVariant(), const QVariant &arg3=QVariant(), const QVariant &arg4=QVariant(), const QVariant &arg5=QVariant(), const QVariant &arg6=QVariant(),
              const QVariant &arg7=QVariant());
    Q_INVOKABLE void metaInvoke(const QString & serverClassTypeName, const QString& functionName, QJSValue callBackFunction, const QVariant &arg1=QVariant(),
								const QVariant &arg2=QVariant(), const QVariant &arg3=QVariant(), const QVariant &arg4=QVariant(), const QVariant &arg5=QVariant(), const QVariant &arg6=QVariant(),
								const QVariant &arg7=QVariant());
	void setUserTempKey(const QByteArray &userTempKey){this->userTempKey=userTempKey;}
    Q_INVOKABLE void authenticate(const QString &login, const QString &password, bool autoLogin, qint32 appType, QJSValue callBackFunction);
	Q_INVOKABLE void setServerUrl(const QString &serverUrl);
    QString getMobileSeed(){return mobileSeed;}

signals:
	void sigProgress(bool inProgress, int percentage=-1);
	void sigSessionExpired();
	void sigNetworkError(const QString &errorName);

public slots:
	void slotReplyFinished();
	void slotNetworkReplyError(QNetworkReply::NetworkError errorCode);
	void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void slotRespAuthenticate(const QJsonRpcMessage &resp);

private:
    explicit NJsonRpc(QObject *parent = 0);
    QUrl getSVCUrl();
	void addReceiver(QObject *objReceiver, quint64 id, const char *slot);
	void addCallBackReceiver(QJSValue callBackFunction, const QString &completeServerFunction, quint64 msgID);
	void invokeCallBack(QJSValue callBackFunction, const QString &completeServerFunction, const QJsonValue &jValue);
    QString getClassName(qint32 classType);
    static QPointer<NJsonRpc> o;
	QNetworkAccessManager *manager;
	QJsonRpcHttpClient *client;
	QHash<quint64, STRReceiver*> receiverHash;
	QString userTempKey;
    QJSValue jsFunctionAuthenticate;
    QString mobileSeed, serverSeed;
	QUrl serverUrl;
};
#endif
