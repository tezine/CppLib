#ifndef EFIRSTSVCARG_H
#define EFIRSTSVCARG_H
/**
*@author Tezine Technologies
*Machine generated. DO NOT EDIT THIS FILE!
**/
#include "QStringList"
#include "QDateTime"
#include "QVariant"
#include "cpplib_global.h"


/**
*@class EFirstSVCArg
**/
class CPPLIB_EXPORT EFirstSVCArg : public QObject {
	Q_OBJECT
	Q_PROPERTY(qint32 appType READ getAppType WRITE setAppType USER true)
	Q_PROPERTY(qint32 argsCount READ getArgsCount WRITE setArgsCount USER true)
	Q_PROPERTY(QString userTempKey READ getUserTempKey WRITE setUserTempKey USER true)
	Q_PROPERTY(QString className READ getClassName WRITE setClassName USER true)
	Q_PROPERTY(QString functionName READ getFunctionName WRITE setFunctionName USER true)

public:
	Q_INVOKABLE EFirstSVCArg(QObject *parent=0);
	~EFirstSVCArg(){}
	static QMetaObject getMeta();
	Q_INVOKABLE EFirstSVCArg(const EFirstSVCArg &d) : QObject () { setData(d); }
	Q_INVOKABLE EFirstSVCArg &operator=(const EFirstSVCArg &d){ return setData(d); }
	Q_INVOKABLE bool operator== (const EFirstSVCArg &other) const{ if(equal(other))return true;return false;}
	Q_INVOKABLE inline bool operator!= (const EFirstSVCArg &other) const{ if(equal(other))return false;return true;}
	inline qint32 getAppType() const {return appType;}
	void setAppType(qint32 d){appType=d;}
	inline qint32 getArgsCount() const {return argsCount;}
	void setArgsCount(qint32 d){argsCount=d;}
	Q_INVOKABLE QString getUserTempKey() const {return userTempKey;}
	void setUserTempKey(const QString &d){userTempKey=d;}
	Q_INVOKABLE QString getClassName() const {return className;}
	void setClassName(const QString &d){className=d;}
	Q_INVOKABLE QString getFunctionName() const {return functionName;}
	void setFunctionName(const QString &d){functionName=d;}

protected:
	EFirstSVCArg &setData(const EFirstSVCArg &d){
		appType=d.appType;
		argsCount=d.argsCount;
		userTempKey=d.userTempKey;
		className=d.className;
		functionName=d.functionName;
		return *this;
	}
	bool equal(const EFirstSVCArg &other) const {
		if(appType!=other.appType)return false;
		if(argsCount!=other.argsCount)return false;
		if(userTempKey!=other.userTempKey)return false;
		if(className!=other.className)return false;
		if(functionName!=other.functionName)return false;
		return true;
	}
	qint32 appType;
	qint32 argsCount;
	QString userTempKey;
	QString className;
	QString functionName;
};

Q_DECLARE_METATYPE(EFirstSVCArg)
Q_DECLARE_METATYPE(EFirstSVCArg*)
Q_DECLARE_METATYPE(QList<EFirstSVCArg*>)
Q_DECLARE_METATYPE(QList<EFirstSVCArg>)
#endif
