#ifndef EBASESETUP_H
#define EBASESETUP_H
#include "QStringList"
#include "QDateTime"
#include "QVariant"
#include "cpplib_global.h"
#include "qmetaobject.h"
class EnhancedQmlApplicationEngine;

/**
*@class EBaseSetup
**/
class CPPLIB_EXPORT EBaseSetup : public QObject {
	Q_OBJECT
	Q_PROPERTY(qint32 width READ getWidth WRITE setWidth USER true)
	Q_PROPERTY(qint32 height READ getHeight WRITE setHeight USER true)
	Q_PROPERTY(QString resourceDBPath READ getResourceDBPath WRITE setResourceDBPath USER true)
	Q_PROPERTY(QString appVersion READ getAppVersion WRITE setAppVersion USER true)
	Q_PROPERTY(bool isPreview READ getIsPreview WRITE setIsPreview USER true)
	Q_PROPERTY(bool recreateDBIfNewVersion READ getRecreateDBIfNewVersion WRITE setRecreateDBIfNewVersion USER true)
	Q_PROPERTY(QList<QMetaObject> entities READ getEntities WRITE setEntities USER true)
	Q_PROPERTY(QString svcBaseAddress READ getSvcBaseAddress WRITE setSvcBaseAddress USER true)
	Q_PROPERTY(QString svcCompleteAddress READ getSvcCompleteAddress WRITE setSvcCompleteAddress USER true)
	Q_PROPERTY(QString appOrganizationName READ getAppOrganizationName WRITE setAppOrganizationName USER true)
	Q_PROPERTY(QString appOrganizationDomain READ getAppOrganizationDomain WRITE setAppOrganizationDomain USER true)
	Q_PROPERTY(QString appName READ getAppName WRITE setAppName USER true)

public:
    Q_INVOKABLE EBaseSetup(QObject *parent=nullptr);
	~EBaseSetup(){}
	inline qint32 getWidth() const {return width;}
	void setWidth(qint32 d){width=d;}
	inline qint32 getHeight() const {return height;}
	void setHeight(qint32 d){height=d;}
	Q_INVOKABLE QString getResourceDBPath() const {return resourceDBPath;}
	void setResourceDBPath(const QString &d){resourceDBPath=d;}
	Q_INVOKABLE QString getAppVersion() const {return appVersion;}
	void setAppVersion(const QString &d){appVersion=d;}
	inline bool getIsPreview() const {return isPreview;}
	void setIsPreview(bool d){isPreview=d;}
	inline bool getRecreateDBIfNewVersion() const {return recreateDBIfNewVersion;}
	void setRecreateDBIfNewVersion(bool d){recreateDBIfNewVersion=d;}
	QList<QMetaObject> &getEntitiesByRef() {return entities;}
	Q_INVOKABLE QList<QMetaObject> getEntities() const {return entities;}
	void setEntities(const QList<QMetaObject> &d){entities=d;}
	Q_INVOKABLE QString getSvcBaseAddress() const {return svcBaseAddress;}
	void setSvcBaseAddress(const QString &d){svcBaseAddress=d;}
	Q_INVOKABLE QString getSvcCompleteAddress() const {return svcCompleteAddress;}
	void setSvcCompleteAddress(const QString &d){svcCompleteAddress=d;}
	Q_INVOKABLE QString getAppOrganizationName() const {return appOrganizationName;}
	void setAppOrganizationName(const QString &d){appOrganizationName=d;}
	Q_INVOKABLE QString getAppOrganizationDomain() const {return appOrganizationDomain;}
	void setAppOrganizationDomain(const QString &d){appOrganizationDomain=d;}
	Q_INVOKABLE QString getAppName() const {return appName;}
	void setAppName(const QString &d){appName=d;}
    inline EnhancedQmlApplicationEngine* getQmlEngine() const {return qmlEngine;}
    void setQmlEngine(EnhancedQmlApplicationEngine* d){qmlEngine=d;}

private:
	qint32 width;
	qint32 height;
	QString resourceDBPath;
	QString appVersion;
	bool isPreview;
	bool recreateDBIfNewVersion;
	QList<QMetaObject> entities;
	QString svcBaseAddress;
    QString svcCompleteAddress;
    EnhancedQmlApplicationEngine* qmlEngine;
	QString appOrganizationName;
	QString appOrganizationDomain;
	QString appName;
};
#endif
