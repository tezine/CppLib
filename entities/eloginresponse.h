#ifndef ELOGINRESPONSE_H
#define ELOGINRESPONSE_H
/**
*@author Tezine Technologies
*Machine generated. DO NOT EDIT THIS FILE!
**/
#include "QStringList"
#include "QDateTime"
#include "QVariant"
#include "cpplib_global.h"


/**
*@class ELoginResponse
**/
class CPPLIB_EXPORT ELoginResponse : public QObject {
	Q_OBJECT
	Q_PROPERTY(qint64 userID READ getUserID WRITE setUserID USER true)
	Q_PROPERTY(QString userName READ getUserName WRITE setUserName USER true)
	Q_PROPERTY(qint64 cityID READ getCityID WRITE setCityID USER true)
	Q_PROPERTY(QString cityName READ getCityName WRITE setCityName USER true)
	Q_PROPERTY(QString swVersion READ getSwVersion WRITE setSwVersion USER true)
	Q_PROPERTY(qint32 agreementVersion READ getAgreementVersion WRITE setAgreementVersion USER true)
	Q_PROPERTY(qint32 respCreateUser READ getRespCreateUser WRITE setRespCreateUser USER true)
	Q_PROPERTY(QString serverSeed READ getServerSeed WRITE setServerSeed USER true)
	Q_PROPERTY(bool canExecute READ getCanExecute WRITE setCanExecute USER true)
	Q_PROPERTY(bool isProfileComplete READ getIsProfileComplete WRITE setIsProfileComplete USER true)

public:
	Q_INVOKABLE ELoginResponse(QObject *parent=0);
	~ELoginResponse(){}
	static QMetaObject getMeta();
	Q_INVOKABLE ELoginResponse(const ELoginResponse &d) : QObject () { setData(d); }
	Q_INVOKABLE ELoginResponse &operator=(const ELoginResponse &d){ return setData(d); }
	Q_INVOKABLE bool operator== (const ELoginResponse &other) const{ if(equal(other))return true;return false;}
	Q_INVOKABLE inline bool operator!= (const ELoginResponse &other) const{ if(equal(other))return false;return true;}
	inline qint64 getUserID() const {return userID;}
	void setUserID(qint64 d){userID=d;}
	Q_INVOKABLE QString getUserName() const {return userName;}
	void setUserName(const QString &d){userName=d;}
	inline qint64 getCityID() const {return cityID;}
	void setCityID(qint64 d){cityID=d;}
	Q_INVOKABLE QString getCityName() const {return cityName;}
	void setCityName(const QString &d){cityName=d;}
	Q_INVOKABLE QString getSwVersion() const {return swVersion;}
	void setSwVersion(const QString &d){swVersion=d;}
	inline qint32 getAgreementVersion() const {return agreementVersion;}
	void setAgreementVersion(qint32 d){agreementVersion=d;}
	inline qint32 getRespCreateUser() const {return respCreateUser;}
	void setRespCreateUser(qint32 d){respCreateUser=d;}
	Q_INVOKABLE QString getServerSeed() const {return serverSeed;}
	void setServerSeed(const QString &d){serverSeed=d;}
	inline bool getCanExecute() const {return canExecute;}
	void setCanExecute(bool d){canExecute=d;}
	inline bool getIsProfileComplete() const {return isProfileComplete;}
	void setIsProfileComplete(bool d){isProfileComplete=d;}

protected:
	ELoginResponse &setData(const ELoginResponse &d){
		userID=d.userID;
		userName=d.userName;
		cityID=d.cityID;
		cityName=d.cityName;
		swVersion=d.swVersion;
		agreementVersion=d.agreementVersion;
		respCreateUser=d.respCreateUser;
		serverSeed=d.serverSeed;
		canExecute=d.canExecute;
		isProfileComplete=d.isProfileComplete;
		return *this;
	}
	bool equal(const ELoginResponse &other) const {
		if(userID!=other.userID)return false;
		if(userName!=other.userName)return false;
		if(cityID!=other.cityID)return false;
		if(cityName!=other.cityName)return false;
		if(swVersion!=other.swVersion)return false;
		if(agreementVersion!=other.agreementVersion)return false;
		if(respCreateUser!=other.respCreateUser)return false;
		if(serverSeed!=other.serverSeed)return false;
		if(canExecute!=other.canExecute)return false;
		if(isProfileComplete!=other.isProfileComplete)return false;
		return true;
	}
	qint64 userID;
	QString userName;
	qint64 cityID;
	QString cityName;
	QString swVersion;
	qint32 agreementVersion;
	qint32 respCreateUser;
	QString serverSeed;
	bool canExecute;
	bool isProfileComplete;
};

Q_DECLARE_METATYPE(ELoginResponse)
Q_DECLARE_METATYPE(ELoginResponse*)
Q_DECLARE_METATYPE(QList<ELoginResponse*>)
Q_DECLARE_METATYPE(QList<ELoginResponse>)
#endif
