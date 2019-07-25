#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include <QObject>
#include <QHash>
#include <QList>
#include <QSqlDatabase>
#include "cdefines.h"
#include "cpplib_global.h"
class SqlTable;
class QSqlQuery;

/*
 *
 */
class CPPLIB_EXPORT  SqlDB : public QObject {
	Q_OBJECT

	public:
		enum DBType {
			DBTypePostgreSQL=1,
			DBTypeSQLite,
			DBTypeFirebird,
			DBTypeMySql
		};
		SqlDB(const QList<QMetaObject>& listTableTypes=QList<QMetaObject>(),qint32 dbType=DBTypeSQLite, const QString& connectionName="default");
		QString getLastError();
		int startTransaction();
		bool commitTransaction(qint64 transactionID);
		bool rollbackTransaction(qint64 transactionID);
		bool containsTable(const QString& tableName);
		bool containsSqlTable(const QString &tableName);
		bool createSQLiteTable(const QString& schemaName, QMetaObject& tableMeta);
		bool createSQLiteTable(const QString& schemaName, const QString& tableName, const QList<TableColumn>& columns);
		bool createMySQLTable(const QString& tableName, const QList<TableColumn>& columns, QString &outError,QString &outExecutedQuery);
		SqlTable* getTable(const QString& tableName, bool sendErrorToServer=false);
		SqlTable* getTable(const QString& schemaName, const QString& tableName);
		bool execQuery(const QString& query);
		bool registerTable(const QString& schemaName, const QString& tableName);
		QStringList getAllMySQLTableNames();
		QSqlQuery* execReaderProcedure(const QString& query, bool& b);
		QSqlDatabase database();
		bool clearTablesData();
		void close(bool debugIntoConsole=true);
		bool vacuum();
		bool renameTable(const QString& oldName, const QString& newName);
		void enableForeignKey();
		void refresh();
		bool deleteTable(const QString& tableName);
		qint32 getDBType(){return dbType; }

	private:
		bool createSqliteTable(const QString& schemaName, const QString& tableName, QMetaObject& tableMeta);
		QMetaObject getMetaObjectForTableName(const QString& tableName);
		QString getMetaNameFromTableName(const QString& tableName);
		QHash<qint64,QString> transactionHash;
		QString connectionName;
		QHash<QString, SqlTable*> hashTables;
		QList<QMetaObject> listTableTypes;
		qint64 nextTransactionID;
		qint32 dbType;
};
#endif
