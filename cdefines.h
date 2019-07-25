#ifndef TEZINECDEFINES_H
#define TEZINECDEFINES_H
#include <QObject>
#include <QIcon>
#include <QSqlQuery>
#include <QVariant>
#define TABLEENUMS "enums"
#define TablePreferences "preferences"
#define INVALIDNUMBER -1
#define INVALIDTEXT "wkinvalid"
#define INTERNALERRORMSG "Internal error"
#define APIDB "APIDB"
#define DUMPFILESIZEMB 2
#define INTERNALERRORMSG "Internal error"
#define DefaultDB "DefaultDB"
#ifdef INTERNALMODE
    #define debugIfInternalMode(WHAT) Base::internalDebug(WHAT)
    #define checkAndDebugIfInternalMode(check, msg) Base::checkAndDebug(check, msg)
#else
    #define debugIfInternalMode(WHAT)
    #define checkAndDebugIfInternalMode(check, msg)
#endif

struct STRStyleSheet {
    QString toolBarStyle;
    QString toolBarLabelStyle;
    QString menuBarStyle;
};
struct StudioBaseSetup {
    STRStyleSheet* strStyleSheet;
};
enum SVCResp {
	SVCRespError = 1,
	SVCRespRespOK
};
enum LogType {
    LogTypeDebug=1,
    LogTypeError,
    LogTypeCritical,
    LogTypeWarning
};
enum DataColumnType {
    DataColumnTypeInteger=1,
    DataColumnTypeText,
    DataColumnTypePKInteger,
    DataColumnTypePKIntegerAutoIncrement,
    DataColumnTypeReal,
    DataColumnTypeBlob
};
enum ResponseType{
    ResponseTypeObject=1,
    ResponseTypeArray=2,
    ResponseTypeValue=3
};

struct SQLParameter {
	QString name;
	QVariant value;
	QSql::ParamType paramType;
	SQLParameter(QString Name, QVariant Value, QSql::ParamType ParamType) {
		name=Name;
		value=Value;
		paramType=ParamType;
	}
};
struct TableColumn {
	QString name;
	DataColumnType type;
	QString typeName;
	bool unique;
	bool notNull;
	bool autoIncrement;
	bool primaryKey;
	QVariant defaultValue;
};
#endif
