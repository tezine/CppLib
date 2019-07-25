#ifndef SQLDATABASES_H
#define SQLDATABASES_H
#include <QPointer>
#include <QHash>
#include <QString>
#include <QHostAddress>
#include "cpplib_global.h"

class SqlDB;

/*
 *
 */
class CPPLIB_EXPORT  SqlDBs : public QObject {
	Q_OBJECT

	public:
		static SqlDBs* obj();
		SqlDB* createSQLiteDB(const QString& completePath, const QString& connectionName, const QList<QMetaObject>& tablesList=QList<QMetaObject>());
		SqlDB* createSQLiteDB(const QString& completePath, const QString& connectionName, const QString& sql, QString& outError,const QList<QMetaObject>& tablesList=QList<QMetaObject>());
		SqlDB* openPostgreSQL(const QList<QMetaObject>& listTableTypes,const QString& dbName, const QString& userName, const QString& password, const QHostAddress& hostAddress=QHostAddress(QHostAddress::LocalHost),
		                      const QString& connectionName="default", int port=5432);
		SqlDB* openSQLite(const QString& completePath, const QList<QMetaObject>& listTableTypes, const QString& connectionName, bool debugIntoConsole=true);
		SqlDB* openMySql(const QString& hostName, const QString& databaseName,  const QString& userName, const QString& password, const QList<QMetaObject>& tablesList=QList<QMetaObject>(), const QString& connectionName=QString("default"), qint32 port=3306,bool sendError=true );
		SqlDB* openSQLiteWithoutTables(const QString& completePath, const QString& connectionName=QString("default"));
		bool openFireBird(const QString& completePath, const QList<QMetaObject>& listTableTypes, const QString& connectionName=QString("default"));
		SqlDB* getDB(const QString& connectionName=QString("default"));
		void closeAllToPostgreSQL();
		bool copyTableData(SqlDB* fromDB, const QString& fromTableName, SqlDB* toDB, const QString& toTableName, bool createTableFirst, QString& outError);
		static QVariant getValueForSqlite(QVariant& v);
		void removeDBFromHash(const QString& dbName){hashDatabases.remove(dbName); }
		bool containsDB(const QString& dbName){return hashDatabases.contains(dbName); }

	private:
		SqlDBs();
		QHash<QString, SqlDB*> hashDatabases;
};
#endif
