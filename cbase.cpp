#include <QtCore>
#include "cbase.h"
#include "blogger.h"

int CBase::debugTabCount=0;
bool CBase::studioMode=false;
qint64 CBase::debugLine=0;
QString CBase::storagePath=QString();
CBase::CBase(){
    debugLine=0;
}


/*
 *Windows: it's something like: C:/Users/tezine/AppData/Local/Tezine Technologies/Wikios Studio
 */
QString CBase::getWikiosStoragePath(){
#ifdef Q_OS_ANDROID
	return "/sdcard/Tezine/WikiosBrowser";
#endif
#if defined(Q_OS_WIN) || defined(Q_OS_MACX) || defined(Q_OS_LINUX)
    if(storagePath.isEmpty())return QCoreApplication::applicationDirPath();
    else return storagePath;
#endif
#ifdef Q_OS_IOS
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif
	BLogger::error(0,__FUNCTION__,"StoragePath not found");
	return QString();
}


/*
 * This is used only by EngineRuntime in server mode
 */
void CBase::setWikiosStoragePath(const QString &storagePath){
    CBase::storagePath=storagePath;
}



void CBase::internalDebug(const QString &msg){
	qDebug()<<msg;
}


void CBase::checkAndDebug(bool check, qint32 debugTabCount, const QString &msg){
    if(check){
        debugLine++;
        BLogger::debug(0,"",QString::number(debugLine).leftJustified(5)+QString().fill('\t',debugTabCount)+msg);
    }
    CBase::debugTabCount=debugTabCount;
}


/*
 *Returns the uptime from msecs to QString
 */
QString CBase::getTimeFromMsecs(double msecs){
	QString formattedTime;
	int hours = msecs/(1000*60*60);
	int minutes = (msecs-(hours*1000*60*60))/(1000*60);
	int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;
	int milliseconds = msecs-(seconds*1000)-(minutes*1000*60)-(hours*1000*60*60);
	formattedTime.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')) + ":" +
						 QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0')));
	return formattedTime;
}


/*
 *Copies the file 'sourcePath' to 'destPath'
 */
bool CBase::copyFileAndMakeWritable(const QString &sourcePath, const QString &destPath, bool replaceIfExists){
	QFile sourceFile(sourcePath);
	QFile destFile(destPath);
	if(destFile.exists() && (!replaceIfExists))return true;
	if(!sourceFile.open(QIODevice::ReadOnly)){qDebug()<<"unable to open file "+sourcePath;return false;}
	QByteArray array=sourceFile.readAll();
	if(!destFile.open(QIODevice::WriteOnly|QIODevice::Truncate)){qDebug()<<"unable to open "+destPath;return false;}
	destFile.write(array);
	sourceFile.close();
	destFile.close();
	return true;
}



/*
 *excludeList must be like d.dll,  .dll, .pdb
 */
bool CBase::copyDirRecursively(const QString &sourceDirPath, const QString &destDirPath, const QStringList &excludeList){
	QDir dir(sourceDirPath);
	if (! dir.exists())return false;
	foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		QString dst_path = destDirPath + QDir::separator() + d;
		dir.mkpath(dst_path);
		copyDirRecursively(sourceDirPath+ QDir::separator() + d, dst_path,excludeList);
	}
	foreach (QString f, dir.entryList(QDir::Files)) {
		QString completeSourcePath=sourceDirPath+QDir::separator() + f;
		QString completeDestPath=destDirPath + QDir::separator() + f;
		bool copy=true;
		foreach(QString excludeEnd, excludeList){
			if(completeSourcePath.endsWith(excludeEnd)){
				copy=false;
				break;
			}
		}
		if(!copy)continue;
		//qDebug()<<"copying file from:"+completeSourcePath+" to:"+completeDestPath;
		QFile::copy(completeSourcePath, completeDestPath);
	}
	return true;
}


bool CBase::removeDirRecursively(const QString & dirName){
	//qDebug() <<"removendo dir:"<<dirName;
	bool result = true;
	QDir dir(dirName);
	if (dir.exists(dirName)) {
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
			if (info.isDir()) {
				result = removeDirRecursively(info.absoluteFilePath());
			}
			else {
				result = QFile::remove(info.absoluteFilePath());
			}
			if (!result) {
				return result;
			}
		}
		result = dir.rmdir(dirName);
	}
	return result;
}


/*
 *Returns the MD5 in base64
 */
QString CBase::getMD5InBase64(const QString &txt){
	return QString(QCryptographicHash::hash(txt.toUtf8(),QCryptographicHash::Md5).toHex().toBase64());
}



QStringList CBase::convertToStringList(const QList<qint64> &list){
    QStringList stringList;
    foreach(qint64 i, list){
        stringList.append(QString::number(i));
    }
    return stringList;
}



/*
 *Removes any duplication like 1, 1
 */
QList<qint64> CBase::removeDuplication(const QList<qint64> &list){
    QList<qint64> tempList;
    foreach(qint64 i, list){
        if(!tempList.contains(i))tempList.append(i);
    }
    return tempList;
}



/*

qint64 Base::getCSDataHeaderSize(const CSDataHeader &csDataHeader) {
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    qint64 startPos=out.device()->pos();
    out<<csDataHeader;
    qint64 endPos=out.device()->pos();
    return (endPos-startPos);
}
qint64 Base::getCSDataSize(const CSData &csData) {
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    qint64 startPos=out.device()->pos();
    out<<csData;
    qint64 endPos=out.device()->pos();
    return (endPos-startPos);
}*/


/*
  *
  */
QByteArray CBase::serializeVariant(QVariant variant) {
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (qint64) 0;
    out<<variant;
    out.device()->seek(0);
    out << (qint64)(array.size() - sizeof(qint64));
    return array;
}

/*
  *
  */
QVariant CBase::deserializeArray(const QByteArray &array) {
    QVariant v;
    QDataStream in(array);
    in.setVersion(QDataStream::Qt_5_0);
    qint64 blockSize=0;
    in >> blockSize;
    in>> v;
    return v;
}


/*
 *Writes the array to the file specified by 'completeFilePath'
 */
bool CBase::writeArrayToFile(const QByteArray &array, const QString &completeFilePath){
    QFile file(completeFilePath);
    if(!file.open(QIODevice::Truncate| QIODevice::WriteOnly))return false;
    file.write(array);
    return true;
}


/*
 *Reads the file into the array
 */
bool CBase::readFileToArray(const QString &completeFilePath, bool textOnly, QByteArray &outArray, QString &outError){
    if(!QFile::exists(completeFilePath)){outError="File does not exist: "+completeFilePath;return false;}
    QFile file(completeFilePath);
    bool ok;
    if(textOnly)ok=file.open(QIODevice::ReadOnly| QIODevice::Text);
    else ok=file.open(QIODevice::ReadOnly);
    if(!ok){outError="Unable to open file "+completeFilePath;return false;}
    outArray=file.readAll();
    file.close();
    return true;
}
