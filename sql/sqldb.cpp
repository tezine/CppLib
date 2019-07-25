#include <QtCore>
#include <QtSql>
#include "Sql"
#include "blogger.h"


SqlDB::SqlDB(const QList<QMetaObject> &listTableTypes,qint32 dbType, const QString &connectionName) {
	nextTransactionID=0;
	this->dbType=dbType;
	this->listTableTypes=listTableTypes;
	this->connectionName=connectionName;
	if(listTableTypes.isEmpty())return;
	refresh();
}


void SqlDB::refresh(){
	hashTables.clear();
	QStringList tablesList=QSqlDatabase::database(connectionName).tables();
	foreach(QString table, tablesList) {
		if(table=="sqlite_sequence")continue;
		QString tableName;
		QString schemaName;
		if(table.contains(".")) {
			schemaName=table.left(table.indexOf("."));
			tableName=table.mid(table.indexOf(".")+1);
		} else tableName=table;
		SqlTable *sqlTable=new SqlTable(this,schemaName,tableName, getMetaObjectForTableName(table),connectionName);
		hashTables[table.toLower()]=sqlTable;
	}
}


bool SqlDB::deleteTable(const QString &tableName){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	return q.exec("drop table "+tableName);
}


/**
 *
 */
QString SqlDB::getLastError() {
	return QSqlDatabase::database(connectionName).lastError().text();
}


/**
 *
 */
int SqlDB::startTransaction() {
	if(nextTransactionID>100000000)nextTransactionID=1;
	else nextTransactionID++;
	if(QSqlDatabase::database(connectionName).transaction()) {
		transactionHash[nextTransactionID]=connectionName;
		return nextTransactionID;
	} else return 0;
}


/**
 *
 */
bool SqlDB::commitTransaction(qint64 transactionID) {
	if(!transactionHash.contains(transactionID))return false;
	return QSqlDatabase::database(transactionHash[transactionID]).commit();
}


/**
 *
 */
bool SqlDB::rollbackTransaction(qint64 transactionID) {
	if(!transactionHash.contains(transactionID))return false;
	return QSqlDatabase::database(transactionHash[transactionID]).rollback();
}


bool SqlDB::containsTable(const QString &tableName){
	QStringList tablesList=QSqlDatabase::database(connectionName).tables();
	foreach(QString table, tablesList){
		if(table.toLower()==tableName.toLower())return true;
	}
	return false;
}


bool SqlDB::containsSqlTable(const QString &tableName){
	return hashTables.contains(tableName);
}


bool SqlDB::createSQLiteTable(const QString &schemaName, QMetaObject &tableMeta) {
	QSqlDatabase db=QSqlDatabase::database(connectionName);
	QString tableName=QString(tableMeta.className()).remove(0,1).toLower();
	if(tableName.endsWith("y"))tableName=tableName.remove(tableName.length()-1,1).append("ies");
	else tableName=tableName.append("s");
	if(db.driverName()==QString("QSQLITE"))return createSqliteTable(schemaName,tableName,tableMeta);
	return false;
}


/*
 *Creates the table in the database
 */
bool SqlDB::createSQLiteTable(const QString &schemaName, const QString &tableName, const QList<TableColumn> &columns){
	Q_UNUSED(schemaName);
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString query;
	QTextStream stream(&query, QIODevice::WriteOnly);
	stream<<"create table "+tableName +" ("<<endl;
	for(int i=0;i<columns.count();i++){
		TableColumn t=columns[i];
		QString columnName=QString(t.name).toLower();
		stream<<"\t"<<columnName<<" ";
		switch(t.type){
			case DataColumnTypeText:
				stream<< "TEXT";
				break;
			case DataColumnTypeInteger:
				stream<< "INTEGER";
				break;
			case DataColumnTypePKInteger:
				stream<<"INTEGER PRIMARY KEY";
				break;
			case DataColumnTypePKIntegerAutoIncrement:
				stream<<"INTEGER PRIMARY KEY AUTOINCREMENT";
				break;
			case DataColumnTypeBlob:
				stream<<"BLOB";
				break;
			case DataColumnTypeReal:
				stream<<"REAL";
				break;
		}
		if(t.unique && (!(t.type==DataColumnTypePKInteger || t.type==DataColumnTypePKIntegerAutoIncrement)))stream<<" UNIQUE";
		if(t.notNull &&(!(t.type==DataColumnTypePKInteger || t.type==DataColumnTypePKIntegerAutoIncrement)))stream<<" NOT NULL";
		if(i==(columns.count()-1))stream<<endl<< ");"<<endl;
		else stream<<","<<endl;
	}
	bool ok= execQuery(query);
	return ok;
}


/*
 *Creates a MySQL Table using the columns defined
 */
bool SqlDB::createMySQLTable(const QString &tableName, const QList<TableColumn> &columnsList, QString &outError, QString &outExecutedQuery){
	qDebug()<<"criando mysql table";
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString query;
	QTextStream stream(&query, QIODevice::WriteOnly);
	stream<<"create table `"+tableName +"` ("<<endl;
	for(int column=0;column<columnsList.count();column++){
		TableColumn tableColumn=columnsList.at(column);
		stream<<"\t`"<<tableColumn.name<<"` "<<tableColumn.typeName;
		if(tableColumn.primaryKey)stream<<" PRIMARY KEY";
		else if(tableColumn.notNull)stream<<" NOT NULL";
		if(tableColumn.defaultValue.isValid())stream<<" DEFAULT '"+tableColumn.defaultValue.toString()+"'";
		if(tableColumn.unique)stream<<" UNIQUE";
		if(tableColumn.autoIncrement)stream<<" AUTO_INCREMENT";
		if(column==(columnsList.count()-1))stream<<endl<< ");"<<endl;
		else stream<<","<<endl;
	}
	outExecutedQuery=query;
	qDebug()<<"query:"<<query;
	if(!q.exec(query)){
		outError=q.lastError().text();
		return false;
	}
	return true;
}


SqlTable *SqlDB::getTable(const QString &tableName, bool sendErrorToServer){
	Q_UNUSED(sendErrorToServer);
	SqlTable *table=getTable("",tableName);
    BLogger::assertError(table,this,"getTable","Unable to open table "+tableName,"",__LINE__);
	return table;
}


bool SqlDB::createSqliteTable(const QString &schemaName, const QString &tableName, QMetaObject &tableMeta) {
	Q_UNUSED(schemaName);
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString query;
	QTextStream stream(&query, QIODevice::WriteOnly);
	stream<<"create table "+tableName +" ("<<endl;
	for(int i = tableMeta.propertyOffset(); i < tableMeta.propertyCount(); ++i) {
		QMetaProperty property=tableMeta.property(i);
		QString columnName=QString(property.name()).toLower();
		stream<<"\t"<<columnName<<" ";
		switch(property.type()) {
			case QVariant::Date:
			case QVariant::DateTime:
			case QVariant::Time:
			case QVariant::String:
				stream<< "TEXT";
				break;
			case QVariant::LongLong:
			case QVariant::Int:
				stream<< "INTEGER";
				if(columnName==QString("id"))stream<<" PRIMARY KEY AUTOINCREMENT";
				break;
			case QVariant::ByteArray:
				stream<< "BLOB";
				break;
			default: { //usertype talvez
				qDebug()<<"usertype"<<property.typeName();
				if(QString(property.typeName())==QString("qint32") || QString(property.typeName())==QString("qint64")) {
					stream<<"INTEGER";
					if(columnName==QString("id"))stream<<" PRIMARY KEY AUTOINCREMENT";
				}
			}
		}
		if(i==(tableMeta.propertyCount()-1))stream<<endl<< ");"<<endl;
		else stream<<","<<endl;
	}
	bool ok= execQuery(query);
	if(ok) {
		SqlTable *sTable=new SqlTable(this,"",tableName,tableMeta,connectionName);
		hashTables[tableName.toLower()]=sTable;
	}
	return ok;
}


/**
 *
 */
SqlTable *SqlDB::getTable(const QString &schemaName, const QString &tableName) {
	if((tableName==NULL) || tableName.isEmpty()) {
		BLogger::log(LogTypeError,this,"getTable","Table name is empty or null");
		return  NULL;
	}
	if(schemaName.isEmpty()) {
		if(!hashTables.contains(tableName.toLower())) {
			BLogger::log(LogTypeError,this,"getTable","Table "+tableName+" not found. \nDbPath:"+QSqlDatabase::database(connectionName).databaseName()+"\n.ListCount:"+QString::number(hashTables.size()));
			return NULL;
		}
		return hashTables[tableName.toLower()];
	} else {
		if(!hashTables.contains(schemaName.toLower()+"."+ tableName.toLower())) {
			BLogger::log(LogTypeError,this,"getSqlTable", "Table "+schemaName+"."+ tableName+" not found");
			return NULL;
		}
		return hashTables[schemaName.toLower()+"."+tableName.toLower()];
	}
	return NULL;
}


/**
 *
 */
QMetaObject SqlDB::getMetaObjectForTableName(const QString &tableName) {
	QString metaNameFromTableName;
	if(tableName.contains("."))metaNameFromTableName=tableName.mid(tableName.indexOf(".")+1);
	else metaNameFromTableName=tableName;
	metaNameFromTableName=getMetaNameFromTableName(metaNameFromTableName);
	//qDebug()<<"procurando meta para "<<metaNameFromTableName;
	foreach(QMetaObject meta, listTableTypes) {
		if(QString(meta.className()).toLower()==metaNameFromTableName) {
//			BLogger::log(LogTypeDebug,this, "getMetaObjectForTableName", "MetaObject found for table "+tableName +" in database. MetaName:"+QString(meta.className()).toLower());
			return meta;
		}
	}
	//qDebug()<<"(SqlDB)getMetaObjectForTableName. returning empty metaObject for table "+connectionName+"." +tableName+".It's ok if it's an app table.";
	return QMetaObject();
}

/**
  *Return the table name adjusted to be compared with a MetaObject
  *For example, countries, return TCOUNTRY
  *users return TUSER
  */
QString SqlDB::getMetaNameFromTableName(const QString &tableName) {
	QString table=tableName;
	table="t"+tableName.toLower();
	table=table.replace(QRegExp("ies$",Qt::CaseInsensitive),"y");
	table=table.replace(QRegExp("s$",Qt::CaseInsensitive),"");
	//qDebug()<<"metaName:"<<table.toLower();
	return table.toLower();
}


/**
 *
 */
bool SqlDB::execQuery(const QString &query) {	
	QSqlQuery q(QSqlDatabase::database(connectionName));
	bool ok=false;
	switch(dbType){
		case DBTypePostgreSQL:
			ok= q.exec(query);
			break;
		case DBTypeMySql:
			ok=q.exec(query);
			break;
		case DBTypeSQLite:{
			//o sqlite nao executa todo o script de uma vez so como o postgresql.
			QStringList list=query.split(";");
			foreach(QString createSt, list){
				if(createSt.trimmed().simplified().isEmpty())break;
				ok=q.exec(createSt);
				if(!ok)break;
			}
			break;
		}
	}
	if(!ok)BLogger::log(LogTypeError,this, __FUNCTION__,"unable to exec query: "+q.lastError().text());
	return ok;
}


/**
  *
  */
bool SqlDB::registerTable(const QString &schemaName, const QString &tableName) {
	QString completeTableName=schemaName+"."+tableName;
	SqlTable *sqlTable=new SqlTable(this,schemaName,tableName, getMetaObjectForTableName(completeTableName),connectionName);
	hashTables[completeTableName.toLower()]=sqlTable;
	return true;
}


/*
 *Returns all MySQL table names
 */
QStringList SqlDB::getAllMySQLTableNames(){
	QStringList list;
	QSqlQuery q(QSqlDatabase::database(connectionName));
	bool ok=q.exec("select TABLE_NAME from information_schema.tables where TABLE_SCHEMA='"+QSqlDatabase::database(connectionName).databaseName()+"'");
	if(!ok){BLogger::error(this,__FUNCTION__,q.lastError().text());return list;}
	while(q.next()){
		list.append(q.value(0).toString());
	}
	return list;
}


/**
 *
 */
QSqlQuery *SqlDB::execReaderProcedure(const QString &query, bool &ok) {
	QSqlQuery *q=new QSqlQuery(QSqlDatabase::database(connectionName));
	ok= q->exec(query);
	if(!ok)BLogger::log(LogTypeError,this, __FUNCTION__,"unable to exec query: "+q->lastError().text());
	return q;
}


QSqlDatabase SqlDB::database(){
	return QSqlDatabase::database(connectionName);
}


/*
 *Clears all tables
 */
bool SqlDB::clearTablesData(){
	QList<SqlTable*> tablesList= hashTables.values();
	foreach(SqlTable *table, tablesList){
		if(!table->clear()){BLogger::error(this,__FUNCTION__,"Unable to clear table "+table->getCompleteTableName());return false;}
	}
	return true;
}


/**
 *
 */
void SqlDB::close(bool debugIntoConsole) {
	if(debugIntoConsole)qDebug()<<"closing db: "<<connectionName;
	QList<SqlTable*> tablesList= hashTables.values();
	qDeleteAll(tablesList.begin(), tablesList.end());
	hashTables.clear();
	QSqlDatabase::database(connectionName,false).close();
	QSqlDatabase::removeDatabase(connectionName);
	SqlDBs::obj()->removeDBFromHash(connectionName);
}


/**
  *
  */
bool SqlDB::vacuum() {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	return q.exec("vacuum");
}


bool SqlDB::renameTable(const QString &oldName, const QString &newName){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	bool ok=q.exec("alter table "+oldName+" rename to '"+newName+"'");
	if(!ok)BLogger::error(this,"renameTable","unable to rename table "+oldName+":"+ q.lastError().text());
	return ok;
}


/*
 *Enable foreign key on sqlite. Isso é valido somente durante a conexao
 */
void SqlDB::enableForeignKey(){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.exec("PRAGMA foreign_keys = ON;");
}
