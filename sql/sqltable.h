#ifndef SQLTABLE_H
#define SQLTABLE_H
#include <QPointer>
#include <QHostAddress>
#include <QSqlDatabase>
#include "cdefines.h"
#include "cpplib_global.h"

class STRObject;
class SqlDB;
class QJsonObject;


/**
 *  QObject usado deve extender o QObject e ter o property id
 * Remember that a foreign key must be a primary key in the other table, or SqlQuery.prepare will display "foreign key mismatch"
 */
class CPPLIB_EXPORT SqlTable : public QObject {
	Q_OBJECT
	Q_ENUMS(Operation)
	Q_FLAGS(SFilter SFilters)

	public:
		enum SFilter {
			SFilterSortAscending=1,
			SFilterSortDescending
		};
		enum Operation{
			OperationInsert=1,
			OperationUpate,
			OperationDelete
		};
		Q_DECLARE_FLAGS(SFilters, SFilter)
		SqlTable(SqlDB* db, const QString& schemaName, const QString& tableName, const QMetaObject& meta, const QString& connectionName);
		QVariant select(qint64 listStart=0,qint32 count=-1, const QString& sortByColumnName=QString(),Qt::SortOrder sortOrder=Qt::AscendingOrder);
		QVariant selectTop(const QString& columnName, qint64 listStart=0,qint32 count=-1);
		QVariant selectTop_Top2(const QString& topColumnName1, const QString& topColumnName2, qint64 listStart=0,qint32 count=-1);
		qint64 selectMax(const QString& columnName);
		QVariant selectAll(bool asPointer=false);
		QVariant selectWhere(const QString& filter,qint64 listStart=0,qint32 count=-1,const QString& sortByColumnName=QString(),SFilters filters=SFilterSortAscending,const QStringList& columnNamesToSelect=QStringList());
		QVariant selectWhere_Top2(const QString& filter, const QString& topColumnName2, qint64 listStart=0,qint32 count=-1);
		QVariant selectFirstByName(const QString& name);
		QVariant selectByColumn(const QString& columnName,const QVariant& value);
		qint64 selectMaxID();
		int selectCount();
		QVariant selectByID(int id,const QStringList& columnNamesToSelect=QStringList(),bool asPointer=false);
		QString getSqliteDDL();
		QStringList getSqliteIndexes();
		static QString getColumnNamesForQuery(const QSqlRecord& record, bool autoIncrementID=false);     // space(id, name, description,....)
		static QString getColumnValuesForUpdate(const QSqlRecord& record, const QString& whereColumnName);     // space id=:id, name=:name where whereColumnName=:whereColumnName
		static QStringList getColumnValueNamesForQuery(const QSqlRecord& record, bool autoIncrementID=false); // :id,:name, :description
		QString getCompleteTableName(){return tableNameWithSchema; }
		SqlDB* getDB(){return sqlDB; }
		QList<QMetaProperty> getProperties();
		QVariant find(const QString& name, const QString& searchColumn1, qint64 start=0, qint32 count=-1, const QString& searchColumn2=QString(),const QString& searchColumn3=QString(),const QString& searchColumn4=QString());
		bool deleteWhere(const QString& filter);
		bool containsID(qint64 id);
        qint64 insert(QObject* obj, bool autoIncrementID, QString &outError);
		bool insertWithNegativeID(QObject* obj);
        bool update(QObject* obj, QString &outError);
		qint64 updateOrInsert(QObject* obj, qint64 id=0);
		bool updateColumn(const QString& columnName, const QString& filter, const QVariant& value);
		bool incrementColumn(const QString& columnName, const QString& filter);
        bool deleteByID(int id, QString &outError);
		bool deleteRecord(QObject* obj);
		bool clear();
		bool recreateIDs();
		bool renameTable(const QString& newName);
		QString getLastError(){return lastError;}
		QJsonObject saveFromJsonObject(const QJsonObject &jsonObject);

	private:
		QJsonObject createReturnObj(bool ok, const QString &error, const QHash<int, int> &newIDHash);
		QObject *createObject(const QJsonObject &jsonObject, bool &outOk, QString &outError);
		void setLastError(const QString &functionName, const QString &error);
		QList<SQLParameter> getParameters(QObject* obj,bool autoIncrementID=false);
		QString getNames(QObject* obj, bool autoIncrementID=false, bool withParentheses=false);
		QString getValues(QObject* obj,bool autoIncrementID=false);
		QString getNamesAndValues(QObject* obj,bool autoIncrementID=false);
		bool setProp(QObject* obj, QString& name, const QVariant& value);
		QString table;
		QString schemaName;
		QString connectionName;
		QString lastError;
		QMetaObject metaObj;
		QString tableNameWithSchema;
		QList<QObject*> listaParaConverter;
		SqlDB* sqlDB;
		QString emptyString;
};
#endif
