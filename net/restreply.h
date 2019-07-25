#ifndef RESTREPLY_H
#define RESTREPLY_H
#include <QObject>
#include <QJSValue>
#include <QUrl>

class Rest;
class QNetworkReply;

class RestReply : public QObject{
    Q_OBJECT

public:
    explicit RestReply(Rest *rest, QNetworkReply *reply, qint64 messageID, const QUrl &completeServerURL, QJSValue progressBarValue);
    ~RestReply();
    Q_INVOKABLE void connect(QJSValue callBackFunction);
    Q_INVOKABLE void connectWithThis(QJSValue qmlForm, QJSValue callBackFunction);

signals:
    void sigReplyFinished();

public slots:
    void slotReplyFinished();

private:
    QJSValue callBackFunction;
    bool isNewConnect;
    QJSValue progressBarValue;
    QJSValue qmlObject;
    QNetworkReply *reply;
    QUrl completeServerURL;
};
#endif
