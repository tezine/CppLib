#include <QtCore>
#include <QtSql>
#include "Sql"
#include "blogger.h"
#include "csingletonperthread.h"

SqlDBs *SqlDBs::obj() {	
	if(!CSingletonPerThread::containsObject(staticMetaObject.className()))CSingletonPerThread::addObject(new SqlDBs());
	return (SqlDBs*)CSingletonPerThread::getObject(staticMetaObject.className());
}


/**
  *
  */
SqlDBs::SqlDBs() {
}


/**
  *
  */
SqlDB *SqlDBs::openPostgreSQL(const QList<QMetaObject> &listTableTypes,const QString &dbName, const QString &userName, const QString &password, const QHostAddress &hostAddress, const QString &connectionName, int port) {
	if(QSqlDatabase::contains(connectionName)) {
		QSqlDatabase db=QSqlDatabase::database(connectionName,false);
		if(!db.isOpen()) {
			bool ok=db.open(userName,password);
			if(ok)return getDB(connectionName);
			else return NULL;
		}
		return getDB(connectionName);
	}
	QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",connectionName);
	db.setHostName(hostAddress.toString());
	db.setDatabaseName(dbName);
	db.setUserName(userName);
	db.setPassword(password);
	db.setPort(port);
	bool ok= db.open();
	qDebug()<<"postgresql has blob:"<<db.driver()->hasFeature(QSqlDriver::BLOB);
	if(ok) {
		SqlDB *sdb=new SqlDB(listTableTypes,SqlDB::DBTypePostgreSQL, connectionName);
		hashDatabases[connectionName]=sdb;
		return sdb;
	}
	BLogger::log(LogTypeError,this, "openPostgreSQL","Unable to connect:"+db.lastError().text());
	return NULL;
}


/*
 *Opens a MySQL database
 */
SqlDB *SqlDBs::openMySql(const QString &hostName, const QString &databaseName, const QString &userName, const QString &password, const QList<QMetaObject> &tablesList, const QString &connectionName, qint32 port, bool sendError){
	Q_UNUSED(sendError);
	if(hashDatabases.contains(connectionName))return hashDatabases[connectionName];
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",connectionName);
	db.setHostName(hostName);
	db.setDatabaseName(databaseName);
	db.setUserName(userName);
	db.setPassword(password);
	db.setPort(port);
	bool ok=db.open();
	if(ok){
		SqlDB *sdb=new SqlDB(tablesList, SqlDB::DBTypeMySql, connectionName);
		hashDatabases[connectionName]= sdb;
		return sdb;
	}
	BLogger::error(this,__FUNCTION__,"Unable to open database "+databaseName);
	return NULL;
}


/**
  *
  */
SqlDB* SqlDBs::openSQLite(const QString &completePath, const QList<QMetaObject> &listTableTypes, const QString &connectionName, bool debugIntoConsole) {
	if(hashDatabases.contains(connectionName)){
		if(debugIntoConsole)BLogger::debug(this,__FUNCTION__,"database with same name already exists. Closing...");
		hashDatabases[connectionName]->close(debugIntoConsole);
	}
	QSqlDatabase db;
	bool ok=true;
	if(QSqlDatabase::contains(connectionName))db=QSqlDatabase::database(connectionName);//we have to do this, since database is shared even on static libs
	else{		
		db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
		if(!QFile::exists(completePath)){BLogger::criticalError(this,__FUNCTION__,"Unable to find path:"+completePath,"",__LINE__);return NULL;}
		db.setDatabaseName(completePath);
		ok= db.open();
	}
	if(ok){
		SqlDB *sdb=new SqlDB(listTableTypes, SqlDB::DBTypeSQLite, connectionName);
		sdb->enableForeignKey();
		//qDebug()<<"adicionando no hash:"<<connectionName<<"thread:"<<this->thread();
		hashDatabases[connectionName]= sdb;
		return sdb;
	}
	BLogger::criticalError(this,__FUNCTION__,"Unable to open database file "+completePath,"",__LINE__);
	return NULL;
}


SqlDB *SqlDBs::createSQLiteDB(const QString &completePath, const QString &connectionName, const QList<QMetaObject> &tablesList) {
	QDir dataDir=QFileInfo(completePath).absoluteDir();
	if(!dataDir.exists())dataDir.mkpath(dataDir.absolutePath());
	QFile dbFile(completePath);
	if(dbFile.exists()) {
		if(!dbFile.remove()) {
			BLogger::log(LogTypeError,this,"createSQLiteDB","unable to remove old sqlitefile");
			return NULL;
		}
	}
	if(!dbFile.open(QIODevice::ReadWrite)) {
		BLogger::log(LogTypeError,this,"createSQLiteDB","unable to open sqlite file");
		return NULL;
	}
	dbFile.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
	db.setDatabaseName(completePath);
	bool ok= db.open();
	if(ok) {
		SqlDB *sdb=new SqlDB(tablesList, SqlDB::DBTypeSQLite, connectionName);
		sdb->enableForeignKey();
		hashDatabases[connectionName]=sdb;
		/*foreach(QMetaObject metaObj, tablesList) {
			sdb->createTable("",metaObj);
		}*/
		return sdb;
	}
	BLogger::log(LogTypeError,this, "createSQLiteDB","Unable to create Sqlite DB:"+db.lastError().text());
	return NULL;
}


/*
 *Create a database from a sql instead from the metaObjects
 */
SqlDB *SqlDBs::createSQLiteDB(const QString &completePath, const QString &connectionName, const QString &sql, QString &outError, const QList<QMetaObject> &tablesList){
	QDir dataDir=QFileInfo(completePath).absoluteDir();
	if(!dataDir.exists())dataDir.mkpath(dataDir.absolutePath());
	QFile dbFile(completePath);
	if(dbFile.exists()) {
		if(!dbFile.remove()) {
			outError="unable to remove old sqlitefile";
			return NULL;
		}
	}
	if(!dbFile.open(QIODevice::ReadWrite)) {
		outError="unable to open sqlite file";
		return NULL;
	}
	dbFile.close();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
	db.setDatabaseName(completePath);
	bool ok= db.open();
	if(!ok){
		outError="unable to open sqlite database";
		return NULL;
	}
	//o sqlite nao executa  o script completo de uma vez so como o postgresql.
	QStringList list=sql.split(";");
	QSqlQuery q(db);
	foreach(QString createSt, list){
		if(createSt.trimmed().simplified().isEmpty())break;
		ok=q.exec(createSt);
		if(!ok){outError="unable to create table. Sql:"+createSt;return NULL;}
	}
	db.close();
	QSqlDatabase::removeDatabase(connectionName);
	return openSQLite(completePath,tablesList,connectionName);
}


/**
  *
  */
SqlDB *SqlDBs::getDB(const QString &connectionName) {
	if(!hashDatabases.contains(connectionName)) {
		qDebug()<<"(SqlDBs)getDB. database with connection name "+connectionName+" not found. Thread:"<<this->thread();
		return NULL;
	}
	return hashDatabases[connectionName];
}


/**
  *
  */
void SqlDBs::closeAllToPostgreSQL() {
	QStringList connectionNames=QSqlDatabase::connectionNames();
	foreach(QString connection, connectionNames) {
		QSqlDatabase db=QSqlDatabase::database(connection,false);
		if(!db.isOpen())continue;
		if(db.driverName()==QString("QPSQL"))db.close();
	}
}


/**
  *
  */
bool SqlDBs::openFireBird(const QString &completePath, const QList<QMetaObject> &listTableTypes, const QString &connectionName) {
	if(QSqlDatabase::contains(connectionName)) {
		QSqlDatabase db=QSqlDatabase::database(connectionName,false);
		if(!db.isOpen())return db.open();
		return true;
	}
	QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE",connectionName);
	db.setDatabaseName(completePath);
	bool ok= db.open();
	if(ok)hashDatabases[connectionName]=new SqlDB(listTableTypes, SqlDB::DBTypeFirebird, connectionName);
	return ok;
}


/*
 *Copies the data from 'fromDB.fromTableName'  to 'toDB.toTableName'
 */
bool SqlDBs::copyTableData(SqlDB *fromDB, const QString &fromTableName, SqlDB *toDB, const QString &toTableName, bool createTableFirst, QString &outError){
	SqlTable *fromTable=fromDB->getTable(fromTableName);
	if(createTableFirst){
		QSqlQuery q(toDB->database());
		q.exec("drop table if exists "+toTableName);
		if(!q.exec(fromTable->getSqliteDDL())){outError="unable to create table "+toTableName;return false;}
	}
	QSqlQuery qFrom(fromDB->database());
	if(!qFrom.exec("select * from "+fromTableName)){outError= "unable to select from source table:"+qFrom.lastError().text();return false;}
	if(!qFrom.first())return true;
	QStringList toTablesList=toDB->database().tables();
	if(!toTablesList.contains(toTableName)){outError="table "+toTableName+" does not exist";return false;}
	QSqlRecord toRecord=toDB->database().record(toTableName);
	QString names=fromTable->getColumnNamesForQuery(toRecord);
	QStringList valueNames=fromTable->getColumnValueNamesForQuery(toRecord);
	qint32 transactionID=toDB->startTransaction();
	do{
		QSqlQuery qTo(toDB->database());
		qTo.prepare("insert into "+toTableName +names+" values "+ "("+valueNames.join(",")+");");
		//qDebug()<<"query:"<<"insert into "+toTableName +names+" values "+ "("+valueNames.join(",")+");";
		for(int i=0;i<valueNames.count();i++){
			QString columnName=valueNames.at(i);
			columnName=columnName.remove(0,1);//ex :name
			qTo.bindValue(valueNames.at(i),qFrom.record().value(columnName));
			//qDebug()<<"bind:"<<valueNames.at(i)<<qFrom.record().value(columnName);
		}
		if(!qTo.exec()){outError= "unable to insert: "+qTo.lastError().text()+".To table:"+toTableName;return false;}
	}while(qFrom.next());
	return toDB->commitTransaction(transactionID);
}


/*
 *Sqlite does not support qint64
 */
QVariant SqlDBs::getValueForSqlite(QVariant &v){
	switch(v.userType()){
		case QMetaType::LongLong:
			return v.toInt();
		case QMetaType::Long:
			return v.toInt();
		case QMetaType::Double:
			return v.toString();
	}
	return v;
}
