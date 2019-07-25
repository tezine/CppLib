#include <QtSql>
#include <QtCore>
#include "Sql"
#include "blogger.h"

SqlTable::SqlTable(SqlDB *db, const QString &schemaName, const QString &tableName, const QMetaObject &meta, const QString &connectionName) {
	this->sqlDB=db;
	//qDebug()<<"criando tabela com nome "<<tableName<<" e meta "<<meta.className();
	this->schemaName=schemaName;
	if(!schemaName.isEmpty())tableNameWithSchema="\""+schemaName+"\"."+tableName;
	else tableNameWithSchema=tableName;
	table=tableName;
	metaObj=meta;
	this->connectionName=connectionName;
}


/**
  *Select from row start and return at most count rows
  */
QVariant SqlTable::select(qint64 start, qint32 count,const QString &sortByColumnName,Qt::SortOrder sortOrder) {
	Q_UNUSED(start);
	Q_UNUSED(count);
	QString tipo("QList<");
	tipo+=metaObj.className();
	tipo+=">";
	int id = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty()|| id==0) {setLastError(__FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");return QVariant();}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString sort;
	if(sortOrder==Qt::AscendingOrder)sort=" asc";
	else sort=" desc";
	QString query;
	if(sortByColumnName.isEmpty())query="select * from "+tableNameWithSchema;
	else query="select * from "+tableNameWithSchema+ " order by "+sortByColumnName+ sort;
	//qDebug()<<"select na tabela:"<<tableNameWithSchema;
	bool ok=q.exec(query);
	if(!ok) {
		BLogger::error(this, "select","unable to select.Query:"+query);
		BLogger::error(this, "select","unable to select.Error:"+ q.lastError().text());
		return QVariant();
	}
	QObject *obj;
	listaParaConverter.clear();
	while(q.next()) {
		obj=metaObj.newInstance();
		for(int i=0; i<q.record().count(); i++) {
			QVariant x=q.record().value(i);
			QString z(q.record().fieldName(i));
			setProp(obj,z,x);
		}
		listaParaConverter.append(obj);
	}
	QVariant v(id, &listaParaConverter);
	foreach(QObject *o, listaParaConverter)o->deleteLater();
	return v;
}


/**
  *Return the top rows from start. Return at most count rows
  *Ex: Select at most count rows from a table with the higher prices(columnName=price)
  */
QVariant SqlTable::selectTop(const QString &columnName, qint64 listStart, qint32 count) {
	Q_UNUSED(columnName);
	Q_UNUSED(listStart);
	Q_UNUSED(count);
	return QVariant();
}


/**
  *
  */
QVariant SqlTable::selectTop_Top2(const QString &topColumnName, const QString &higherColumnName, qint64 listStart, qint32 count) {
	Q_UNUSED(topColumnName);
	Q_UNUSED(higherColumnName);
	Q_UNUSED(listStart);
	Q_UNUSED(count);
	return QVariant();
}


/**
  *Select the max value for the column columnName
  *
  */
qint64 SqlTable::selectMax(const QString &columnName) {
	Q_UNUSED(columnName);
	return 0;
}


/**
  *Never return a null list
  */
QVariant SqlTable::selectAll(bool asPointer) {
	QString tipo("QList<");
	if(asPointer)tipo+=QString(metaObj.className())+"*";
	else tipo+=metaObj.className();
	tipo+=">";
	int id = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty()|| id==0) {BLogger::error(this, __FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");return QVariant();}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	int ok=q.exec("select * from "+tableNameWithSchema);
	if(!ok) {setLastError(__FUNCTION__, "unable to selectAll:"+ q.lastError().text());return QVariant();}
	QObject *obj;
	QList<QObject *> list;
	while(q.next()) {
		obj=metaObj.newInstance();
		QSqlRecord record=q.record();
		for(int i=0; i<record.count(); i++) {//todo acho que isso ta errado
			QString temp(record.fieldName(i));
			setProp(obj,temp,record.value(i));
		}
		list.append(obj);
	}
	if(!asPointer){
		QVariant v(id, &list);
		foreach(QObject *o, list)o->deleteLater();
		return v;
	}else{
		QVariant v(id,&list);
		return v;
	}
}


/**
  *Select from the table with filter filter from row start. Return count rows
  * Never return a null list
  */
QVariant SqlTable::selectWhere(const QString &filter, qint64 start, qint32 count, const QString &sortByColumnName, SFilters filters, const QStringList &columnNamesToSelect) {
	QString tipo("QList<");
	tipo+=metaObj.className();
	tipo+=">";
	QString sort;	
	if(filters.testFlag(SFilterSortAscending))sort="asc";
	else sort="desc";
	int id = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty() || id==0) {
		setLastError(__FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");
		return QVariant();
	}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString queryX;
	if(columnNamesToSelect.isEmpty())queryX="select * from "+tableNameWithSchema+ " where "+ filter;
	else queryX="select "+columnNamesToSelect.join(",")+" from "+tableNameWithSchema+ " where "+ filter;
	if(sortByColumnName.isEmpty())q.exec(queryX);
	else {
		QString query=queryX + " order by "+sortByColumnName+" "+sort;
		if(start==0&&count>0)query+=" limit "+QString::number(count);
		if(start>0&&count>0)query+=" limit "+QString::number(count)+" offset "+QString::number(start);
		q.exec(query);
	}
	//qDebug()<<"select * from "+table+ " where "+ filter;
	//qDebug()<<q.executedQuery();
	if(q.lastError().isValid()) {
		setLastError(__FUNCTION__,q.lastError().text()+".Query:"+q.lastQuery());
		return QVariant();
	}
	QObject *obj;
	listaParaConverter.clear();
	while(q.next()) {
		obj=metaObj.newInstance();
		for(int i=0; i<q.record().count(); i++) {
			QString temp(q.record().fieldName(i));//symbian
			setProp(obj,temp,q.record().value(i));
		}
		listaParaConverter.append(obj);
	}
	QVariant v(id, &listaParaConverter);
	foreach(QObject *o, listaParaConverter)o->deleteLater();
	return v;
}


/**
  *
  */
QVariant SqlTable::selectWhere_Top2(const QString &filter, const QString &topColumnName2, qint64 listStart, qint32 count) {
	Q_UNUSED(filter);
	Q_UNUSED(topColumnName2);
	Q_UNUSED(listStart);
	Q_UNUSED(count);
	//todo
	return QVariant();
}


/*
 *Returns the first row found that has column name='name'
 */
QVariant SqlTable::selectFirstByName(const QString &name){
	QString tipo=metaObj.className();
	int metaID = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty()|| metaID==0) {
		setLastError(__FUNCTION__,"MetaObject not defined for table "+tableNameWithSchema+".Did you use COMMONEXPORT or similar and use getMeta()?");
		return QVariant();
	}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("select * from "+tableNameWithSchema+ " where name=:name");
	q.bindValue(":name",name);
	q.exec();
	if(q.lastError().isValid()) {setLastError(__FUNCTION__,"error on table "+tableNameWithSchema+":"+q.lastError().text());return QVariant();}
	if(!q.first()) {setLastError(__FUNCTION__,"returning NULL for table "+tableNameWithSchema +" where name= "+name);return QVariant();}
	QObject *obj;
	obj=metaObj.newInstance();
	for(int i=0; i<q.record().count(); i++) {
		QString temp(q.record().fieldName(i));//symbian
		setProp(obj, temp,q.record().value(i));
	}
	QVariant v(metaID, obj);
	obj->deleteLater();
	return v;
}


/**
  *
  */
QVariant SqlTable::find(const QString &name, const QString &searchColumn1, qint64 start, qint32 count, const QString &searchColumn2, const QString &searchColumn3, const QString &searchColumn4) {
	Q_UNUSED(name);
	Q_UNUSED(searchColumn1);
	Q_UNUSED(start);
	Q_UNUSED(count);
	Q_UNUSED(searchColumn2);
	Q_UNUSED(searchColumn3);
	Q_UNUSED(searchColumn4);
	return QVariant();
}


/**
  *nao ta funcionando muito bem as pointer
  */
QVariant SqlTable::selectByID(int id, const QStringList &columnNamesToSelect, bool asPointer) {
	QString tipo(metaObj.className());
	int metaID = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty()|| metaID==0) {
		setLastError(__FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");
		return QVariant();
	}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString queryX;
	if(columnNamesToSelect.isEmpty())queryX="select * from "+tableNameWithSchema+ " where id=:id";
	else queryX="select "+columnNamesToSelect.join(",")+" from "+tableNameWithSchema+ " where id=:id";
	q.prepare(queryX);
	q.bindValue(":id",id);
	q.exec();
	if(q.lastError().isValid()) {
		setLastError(__FUNCTION__,"error on table "+table+":"+q.lastError().text());
		return QVariant();
	}
	if(!q.first()) {
		setLastError(__FUNCTION__,"returning NULL for table "+table+" and id "+QString::number(id));
		return QVariant();
	}
	QObject *obj;
	obj=metaObj.newInstance();
	for(int i=0; i<q.record().count(); i++) {
		QString temp(q.record().fieldName(i));//symbian
		setProp(obj, temp,q.record().value(i));
	}	
	if(!asPointer){
		QVariant v(metaID, obj);
		obj->deleteLater();
		return v;
	}else{
		QVariant v(metaID,obj);
		return v;
	}
}


bool SqlTable::containsID(qint64 id){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("select * from "+tableNameWithSchema+ " where id=:id");
	q.bindValue(":id",id);
	q.exec();
	if(q.first())return true;
	return false;
}


/**
 *Select only one record
 */
QVariant SqlTable::selectByColumn(const QString &columnName, const QVariant &value) {
	QString tipo=metaObj.className();
	int metaID = QMetaType::type(tipo.toUtf8().constData());
	if(QString(metaObj.className()).isEmpty()|| metaID==0) {setLastError( __FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");return QVariant();}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QString query("select * from "+tableNameWithSchema+ " where "+columnName.toLower()+" =:colValue");
	q.prepare(query);
	q.bindValue(":colValue",value);
	q.exec();
	if(q.lastError().isValid()) {setLastError(__FUNCTION__,"error on table "+table+":"+q.lastError().text());return QVariant();}
    if(!q.first()) {setLastError(__FUNCTION__,"returnin NULL for table "+table);return QVariant();}
	QObject *obj;
	obj=metaObj.newInstance();
	for(int i=0; i<q.record().count(); i++) {
		QString temp(q.record().fieldName(i));//symbian
		setProp(obj, temp,q.record().value(i));
	}
	QVariant v(metaID, obj);
	obj->deleteLater();
	return v;
}


/**
  *@returns the max id for the table
  */
qint64 SqlTable::selectMaxID() {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.exec("select max(id) from "+tableNameWithSchema);
	if(q.lastError().isValid())setLastError(__FUNCTION__, q.lastError().text());
	if(!q.first())return 0;
	return q.value(0).toLongLong();
}


/**
 *
 */
int SqlTable::selectCount() {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.exec("select count(id) from "+tableNameWithSchema);
	if(q.lastError().isValid())setLastError(__FUNCTION__,q.lastError().text());
	if(!q.first())return 0;
	return q.value(0).toInt();
}


/**
  *Insert the object in the table
  *@returns the id or 0 if unable to insert
  */
qint64 SqlTable::insert(QObject *obj,bool autoIncrementID, QString &outError) {
	QList<SQLParameter> parameters=getParameters(obj,autoIncrementID);
    BLogger::assertError(!parameters.isEmpty(),this,__FUNCTION__,__LINE__);
    BLogger::assertError(obj,this,__FUNCTION__,__LINE__);
	QString columnNames;
	if(sqlDB->getDBType()==SqlDB::DBTypeMySql)columnNames=getNames(obj,autoIncrementID,false);//mysql nao usa 'name', 'address', e sim name, address
	else columnNames=getNames(obj,autoIncrementID,true);
	QString query = "insert into " + tableNameWithSchema + " ("+columnNames+" ) values ("+getValues(obj,autoIncrementID)+")";
	QSqlQuery q(QSqlDatabase::database(connectionName));
	if(!q.prepare(query)) {
		outError="Unable to prepare. Query:"+q.lastQuery()+". Error:"+q.lastError().text();
		setLastError(__FUNCTION__,outError);
		return 0;
	}
	foreach(SQLParameter parameter, parameters){
		q.bindValue(":"+parameter.name,parameter.value,parameter.paramType);
	//	qDebug()<<parameter.name<<":"<< parameter.value<<parameter.paramType;
	}
	if(!q.exec()) {
		outError="Unable to insert. Query:"+q.lastQuery()+". Error:"+q.lastError().text();
		setLastError(__FUNCTION__,outError);
		return 0;
	}
	qint32 maxID= selectMaxID();
	obj->setProperty("id", maxID)	;
	return maxID;
}

/**
  *
  */
bool SqlTable::insertWithNegativeID(QObject *obj) {
	qint64 minID;
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.exec("select min(id) from "+tableNameWithSchema);
	if(q.lastError().isValid())setLastError(__FUNCTION__, q.lastError().text());
	if(!q.first())minID= -1;
	else minID= q.value(0).toLongLong();
	if(minID==0)minID=-1;
	qint64 t=(qint64)minID-1;
	QString temp("id");
	setProp(obj,temp,QVariant::fromValue(t));
    QString outError;
    if(insert(obj,false,outError)==0)return false;
	return true;
}


/**
  *
  */
bool SqlTable::update(QObject *obj, QString &outError) {
	if(!containsID(obj->property("id").toInt())){
		outError="Table "+tableNameWithSchema+" does not contain id: "+QString::number(obj->property("id").toInt());
		setLastError(__FUNCTION__,outError);
		return false;
	}
	QList<SQLParameter> parameters=getParameters(obj,false);
    BLogger::assertError(!parameters.isEmpty(),this,__FUNCTION__,__LINE__);
	QString query = "update " + tableNameWithSchema + " set "+getNamesAndValues(obj,true)+ " where id=:id";
	QSqlQuery q(QSqlDatabase::database(connectionName));
	if(!q.prepare(query)) {
		outError="unable to prepare query:"+query+":"+q.lastError().text();
		setLastError(__FUNCTION__,outError);
		return false;
	}
	foreach(SQLParameter parameter, parameters) {
		//qDebug()<<parameter.name<<":"<<parameter.value;
		q.bindValue(":"+parameter.name,parameter.value,parameter.paramType);
	}
	if(!q.exec()) {
		outError="Unable to update table "+tableNameWithSchema+".Error:"+ q.lastError().text();
		setLastError(__FUNCTION__,outError);
		return false;
	}
	return true;
}


/**
  *Update or insert.
  *@returns the id for the row
  */
qint64 SqlTable::updateOrInsert(QObject *obj,qint64 id) {
    QString outError;
    if(id==-1 || id==0)return insert(obj,true,outError);
	//Primeiro vamos fazer um select para verificar se fazemos um update ou um insert.
	//Nao adianta usar numRowsAffected, pq retorna 0 em update sem alteracoes e retorna o query como ok.
    QString query="select * from "+tableNameWithSchema+ QString(" where id=:id");
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare(query);
	q.bindValue(":id",id);
	bool selectOK=q.exec();
	if(!selectOK) {
		setLastError(__FUNCTION__,"There's no row with this id in "+tableNameWithSchema);
		return false;
	}
	if(selectOK && q.first()) { //vamos fazer update
        bool ok=update(obj,outError);
		if(ok)return id;
		else return 0;
	}
    if(id!=0)return insert(obj,false,outError);
    else return insert(obj,true,outError);
}


/**
  *
  */
bool SqlTable::deleteByID(int id, QString &outError) {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	if(!q.exec("delete from "+tableNameWithSchema+ " where id= "+QString::number(id))){
		outError=q.lastError().text();
		setLastError(__FUNCTION__,outError);
		return false;
	}
	return true;
}


/**
  *
  */
bool SqlTable::deleteRecord(QObject *obj) {
	Q_UNUSED(obj);
	return false;
}


/**
  *
  */
bool SqlTable::clear() {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	if(!q.exec("delete from "+tableNameWithSchema)) {
		setLastError(__FUNCTION__,q.lastError().text());
		return false;
	}
	return true;
}


bool SqlTable::recreateIDs(){
	qint32 maxID=selectMaxID();
	//first we save all with ids starting after max ID, than we save it again starting with 1
	QSqlQuery q(QSqlDatabase::database(connectionName));
	QSqlQuery qInsertTemp(QSqlDatabase::database(connectionName));
	QSqlQuery qInsert(QSqlDatabase::database(connectionName));
	qInsertTemp.prepare("update "+tableNameWithSchema+" set id=:id where id=:oldID");
	qInsert.prepare("update "+tableNameWithSchema+" set id=:id where id=:oldID");
	if(!q.exec("select id from "+tableNameWithSchema))return false;
	maxID+=100;//we start the temp ids with this number
	QList<int> oldIdsList;
	while(q.next()){
		oldIdsList.append(q.value("id").toInt());
	}
	QList<int> tempIDsList;
	foreach(int oldID, oldIdsList){
		tempIDsList.append(maxID);
		qInsertTemp.bindValue(":id",maxID);
		qInsertTemp.bindValue(":oldID",oldID);
		if(!qInsertTemp.exec()){setLastError(__FUNCTION__,"unable to insert tempID: "+QString::number(maxID)+":"+qInsertTemp.lastError().text());return false;}
		qDebug()<<"inserindo tempID: "<<maxID;
		maxID++;
	}
	for(int i=0;i<tempIDsList.size();i++){
		qInsert.bindValue(":id",i+1);
		qInsert.bindValue(":oldID",tempIDsList.at(i));
		if(!qInsert.exec()){setLastError(__FUNCTION__,"unable to insert newID: "+QString::number(i+1)+":" +qInsert.lastError().text());return false;}
		qDebug()<<"inserindo newID: "<<i+1;
	}
	return true;
}


/**
 *
 */
QList<QMetaProperty> SqlTable::getProperties() {
	QList<QMetaProperty> list;
	for(int i = metaObj.propertyOffset(); i < metaObj.propertyCount(); ++i)
		list.append(metaObj.property(i));
	return list;
}


QString SqlTable::getSqliteDDL(){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("SELECT sql FROM sqlite_master WHERE name = :name");
	q.bindValue(":name",tableNameWithSchema);
	if(!q.exec()){
		setLastError(__FUNCTION__,"Unable to get DDL:"+q.lastError().text());
		return QString();
	}
	q.first();
	return q.record().value("sql").toString();
}


QStringList SqlTable::getSqliteIndexes(){
	QStringList indexList;
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("SELECT * FROM (SELECT * FROM sqlite_master UNION ALL SELECT * FROM sqlite_temp_master) WHERE type='index' and tbl_name=:tableName ORDER BY name");
	q.bindValue(":tableName",tableNameWithSchema);
	q.exec();
	while(q.next()){
		indexList.append(q.record().value("name").toString());
	}
	return indexList;
}


/*
 *Ex: space(id, name, description,....)
 *We do not add id if autoIncrementID is true
 */
QString SqlTable::getColumnNamesForQuery(const QSqlRecord &record, bool autoIncrementID){
	QStringList parameters;
	for(int i=0; i<record.count(); i++) {
		if(record.fieldName(i).toLower()=="id" && autoIncrementID)continue;
		parameters.append("'"+record.fieldName(i).toLower()+"'");
	}
	QString result;
	result.append(" (");
	result.append(parameters.join(","));
	result.append(")");
	return result;
}


// space id=:id, name=:name where whereColumnName=:whereColumnName
QString SqlTable::getColumnValuesForUpdate(const QSqlRecord &record, const QString &whereColumnName){
	QStringList parameters;
	for(int i=0; i<record.count(); i++) {
		QString fieldName=record.fieldName(i).toLower();
		if(fieldName==whereColumnName)continue;
		parameters.append(fieldName+"=:"+fieldName);
	}
	QString result;
	result.append(parameters.join(","));
	result.append(" where "+whereColumnName+"=:"+whereColumnName);
	return result;
}


/*
 *Ex:  :id,:name, :description
 */
QStringList SqlTable::getColumnValueNamesForQuery(const QSqlRecord &record, bool autoIncrementID){
	QStringList parameters;
	for(int i=0; i<record.count(); i++) {
		if(record.fieldName(i).toLower()=="id" && autoIncrementID)continue;
		parameters.append(":"+record.fieldName(i).toLower());
	}
	return parameters;
}


bool SqlTable::renameTable(const QString &newName){
	QSqlQuery q(QSqlDatabase::database(connectionName));
	bool ok=q.exec("alter table "+tableNameWithSchema+" rename to '"+newName+"'");
	if(!ok)setLastError(__FUNCTION__,"unable to rename table "+tableNameWithSchema+":"+ q.lastError().text());
	return ok;
}


/*
Data must be received in the format:
"content": {
			"deletedRows": [
			],
			"insertedRows": [
				{
					"id": 0,
					"login": null,
					"name": "Luciane",
					"password": null
				}
			],
			"updatedRows": [
				{
					"id": 1,
					"login": "tezineX",
					"name": "Bruno Tezine",
					"password": "tata"
				}
			]
		},
 */
QJsonObject SqlTable::saveFromJsonObject(const QJsonObject &jsonObject){
	QJsonArray insertedJsonArray=jsonObject.value("insertedRows").toArray();
	QJsonArray deletedJsonArray=jsonObject.value("deletedRows").toArray();
	QJsonArray updatedJsonArray=jsonObject.value("updatedRows").toArray();
	bool ok;
	QString outError;
	QHash<int,int> newIDHash;
	for(int i=0;i<insertedJsonArray.count();i++){
		QObject *obj=createObject(insertedJsonArray.at(i).toObject(),ok,outError);
		if(!ok){obj->deleteLater();return createReturnObj(false,outError,newIDHash);}
		qint64 oldID=obj->property("id").toLongLong();
		qint64 id=insert(obj,true,outError);
		newIDHash[oldID]=id;
		if(id<1){obj->deleteLater();return createReturnObj(false,outError,newIDHash);}
		obj->deleteLater();
	}
	for(int i=0;i<updatedJsonArray.count();i++){
		QObject *obj=createObject(updatedJsonArray.at(i).toObject(),ok,outError);
		if(!ok){obj->deleteLater();return createReturnObj(false,outError,newIDHash);}
		if(!update(obj,outError)){obj->deleteLater();return createReturnObj(false,outError,newIDHash);}
		obj->deleteLater();
	}
	for(int i=0;i<deletedJsonArray.count();i++){
		qint32 id=deletedJsonArray.at(i).toInt();
		if(!deleteByID(id,outError))return createReturnObj(false,outError,newIDHash);
	}
	return createReturnObj(true,outError,newIDHash);
}


/*
 *
 */
QJsonObject SqlTable::createReturnObj(bool ok, const QString &error, const QHash<int,int> &newIDHash){
	QJsonObject jsonObj;
	jsonObj.insert("result",QJsonValue(ok));
	jsonObj.insert("error",QJsonValue(error));
	QJsonArray jsonArray;
	QHashIterator<int, int> i(newIDHash);
	while (i.hasNext()) {
		i.next();
		QJsonObject newObj;
		newObj.insert("oldID",i.key());
		newObj.insert("newID",i.value());
		jsonArray.append(QJsonValue(newObj));
	}
	jsonObj.insert("newIDList",QJsonValue(jsonArray));
	return jsonObj;
}


/*
 *
 */
QObject *SqlTable::createObject(const QJsonObject &jsonObj, bool &outOk, QString &outError){
	outOk=true;
	QObject *obj=metaObj.newInstance();
	QJsonObject::const_iterator it;
	it=jsonObj.begin();
	while(it!=jsonObj.end()){
		if(!obj->setProperty(it.key().toUtf8().constData(), jsonObj.value(it.key()).toVariant())){
			outError="unable to set property:"+it.key();
			BLogger::error(this,__FUNCTION__,outError);
			outOk=false;
			break;
		}
		it++;
	}
	return obj;
}


/*
 *Dumps the error into BLogger and save it to be retrived using getLastErrror
 */
void SqlTable::setLastError(const QString &functionName, const QString &error){
	BLogger::error(this, functionName,error);
	lastError=error;
}


/**
  *
  */
QList<SQLParameter> SqlTable::getParameters(QObject *obj,bool autoIncrementID) {
    BLogger::assertError(obj,this,__FUNCTION__,__LINE__);
	QList<SQLParameter> parameterList;
	const QMetaObject *metaObject = obj->metaObject();
	for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
		if(autoIncrementID && QString(metaObject->property(i).name())==QString("id"))continue;
		if(QString(metaObject->property(i).name())==QString("entryType"))continue;
		if(metaObject->property(i).typeName()==QString("signed char"))parameterList<<SQLParameter(QString(metaObject->property(i).name()),QVariant::fromValue(metaObject->property(i).read(obj).toInt()),QSql::In);//bug no sqlite nao entende qint8 como integer
		else parameterList<<SQLParameter(QString(metaObject->property(i).name()),metaObject->property(i).read(obj),QSql::In);
	}
	return parameterList;
}

/**
  *return nome1, nome2, nome3
  *or 'nome1', 'nome2', 'nome3'
  */
QString SqlTable::getNames(QObject *obj, bool autoIncrementID, bool withParentheses) {
    BLogger::assertError(obj,this,__FUNCTION__,__LINE__);
	QStringList nameList;
	QList<SQLParameter> parameterList=getParameters(obj,autoIncrementID);
	for(int i=0; i<parameterList.count(); i++) {
		SQLParameter parameter=parameterList.at(i);
		if(autoIncrementID && parameter.name=="id")continue;
		if(!withParentheses)nameList.append(parameter.name.toLower());
		else nameList.append("'"+parameter.name.toLower()+"'");
		if(i<(parameterList.count()-1)) nameList.append(",");
	}
	//BLogger::log(LogTypeDebug,this,"getNames","retornando nomes:"+nameList.join(""));
	return nameList.join("");
}

/**
  *return :nome1, :nome2, :nome3
  */
QString SqlTable::getValues(QObject *obj, bool autoIncrementID) {
    BLogger::assertError(obj,this,__FUNCTION__,__LINE__);
	QString values;
	QList<SQLParameter> parameters=getParameters(obj,autoIncrementID);
	for(int i=0; i<parameters.count(); i++) {
		SQLParameter parameter=parameters.at(i);
		if(autoIncrementID && parameter.name=="id")continue;
		values.append(":"+parameter.name);
		if(i!=(parameters.count()-1)) values.append(",");
	}
	//qDebug()<<"retornando valores:"<<values;
	return values;
}

/**
  *return nome1=:nome1, nome2=:nome2, nome3=nome3
  */
QString SqlTable::getNamesAndValues(QObject *obj, bool autoIncrementID) {
	QString namesAndValues;
	bool isMySQL=false;
	if(sqlDB->database().driverName()=="QMYSQL")isMySQL=true;
	QList<SQLParameter> parameters=getParameters(obj,autoIncrementID);
	for(int i=0; i<parameters.count(); i++) {
		SQLParameter parameter=parameters.at(i);
		if(autoIncrementID && parameter.name=="id")continue;
		if(isMySQL)namesAndValues.append(parameter.name);
		else	namesAndValues.append("'"+parameter.name+"'");
		namesAndValues.append(" =:"+parameter.name);
		if(i!=(parameters.count()-1)) namesAndValues.append(",");
	}
	return namesAndValues;
}


/**
  *
  */
bool SqlTable::setProp(QObject *obj, QString &name, const QVariant &value) {
	bool result=false;
	const QMetaObject *metaObject = obj->metaObject();
	for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
		if(QString(metaObject->property(i).name()).toLower()!=name.toLower())continue;
		if(value.isNull())result=true;
		else {
			//qDebug()<<"tentando "<<metaObject->property(i).name()<<"valor:"<<value;
			result=obj->setProperty(metaObject->property(i).name(),value);
		}
		break;
	}
	// recolocar if(!result)BLogger::error(this,"setProp","unable to set property: "+name+" from table "+table);
	return result;
}


/**
  *
  */
bool SqlTable::deleteWhere(const QString &filter) {
	if(QString(metaObj.className()).isEmpty()) {
		setLastError(__FUNCTION__,"MetaObject not defined for table "+table+".Did you use COMMONEXPORT or similar and use getMeta()?");
		return false;
	}
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.exec("delete from "+tableNameWithSchema+ " where "+filter);
	if(q.lastError().isValid())setLastError(__FUNCTION__,"error on table "+table+":"+q.lastError().text());
	return true;
}


/**
  *Update the column where the filter is valid
  */
bool SqlTable::updateColumn(const QString &columnName,const QString &filter, const QVariant &value) {
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("update "+tableNameWithSchema+ " set "+columnName+" = :value where "+filter);
	q.bindValue(":value",value);
	if(!q.exec()) {
		setLastError(__FUNCTION__,"Unable to update the column:"+q.lastError().text());
		return false;
	}
	return true;
}


/**
  *
  */
bool SqlTable::incrementColumn(const QString &columnName, const QString &filter) {
	QSqlQuery qOldValue(QSqlDatabase::database(connectionName));
	qOldValue.exec("select * from "+tableNameWithSchema +" where "+filter);
	if(!qOldValue.first()) {
		setLastError(__FUNCTION__,"record not found from filter:"+qOldValue.lastError().text());
		return false;
	}
	qint64 oldValue=qOldValue.record().value(columnName).toLongLong();
	QSqlQuery q(QSqlDatabase::database(connectionName));
	q.prepare("update "+tableNameWithSchema+ " set "+columnName+" = :value where "+filter);
	q.bindValue(":value",oldValue+1);
	if(!q.exec()) {
		setLastError(__FUNCTION__,"unable to increment");
		return false;
	}
	return true;
}
