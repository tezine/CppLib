#include <QtCore>
#include "blogger.h"
#include "json.h"
#include "cdefines.h"

static QString dumpFileName="";
QFile file;

/*
 *Setup the logger. It must be called before any debug, assert or critical message.
 *It's called from Studio MMobile constructor, EngineRuntime constructor
 */
void BLogger::setup(const QString &dumpFile){	
	openDumpFile(dumpFile);
}


/*
 *Debug the message to the console when compiled in debug mode
 *If dumpFile is set, also dumps the message to the file.
 */
void BLogger::debug(QObject *o, const QString &methodName, const QString &msg) {
	log(LogTypeDebug,o,methodName,msg);
}


/*
 *
 */
void BLogger::warning(QObject *o, const QString &methodName, const QString &msg, bool displayDialog){
	log(LogTypeWarning,o,methodName,msg,QString(),0,displayDialog);
}


/*
 *If displayDialog is set, shows the error to the user.
 *If dumpFile is set, also dumps the message to the file.
 */
void BLogger::error(QObject *o, const QString &methodName, const QString &msg, bool displayDialog) {
	log(LogTypeError,o,methodName,msg,QString(),0,displayDialog);
}


/*
 *If displayDialog is set, shows the error to the user.
 *If dumpFile is set, also dumps the message to the file.
 *Also exists the application
 */
void BLogger::criticalError(QObject *o, const QString &methodName, const QString &internalMsg, qint32 lineNumber, bool displayDialog){
	log(LogTypeCritical,o,methodName,INTERNALERRORMSG,internalMsg,lineNumber,displayDialog);
}


/*
 *If displayDialog is set, shows the error to the user.
 *If dumpFile is set, also dumps the message to the file.
 *Also exists the application
 */
void BLogger::criticalError(QObject *o, const QString &methodName, qint32 lineNumber, bool displayDialog){
	log(LogTypeCritical,o,methodName,INTERNALERRORMSG,"",lineNumber,displayDialog);
}


/*
 *If displayDialog is set, shows the error to the user.
 *If dumpFile is set, also dumps the message to the file.
 *Also exists the application
 */
void BLogger::criticalError(QObject *o, const QString &methodName, const QString &userMsg, const QString &internalMsg, qint32 lineNumber, bool displayDialog){
	log(LogTypeCritical,o,methodName,userMsg,internalMsg,lineNumber,displayDialog);
}


/*
 *Checks the test condition.
 *If it fails, procced as a critical error.
 */
void BLogger::assertError(bool test, QObject *o, const QString &methodName, const QString &userMsg, const QString &internalMsg, qint32 lineNumber, bool displayDialog){
	if(test)return;
	log(LogTypeCritical,o,methodName,QString("Assert failure: ")+userMsg,internalMsg,lineNumber,displayDialog);
}


/*
 *Checks the test condition.
 *If it fails, procced as a critical error.
 */
void BLogger::assertError(bool test, QObject *o, const QString &methodName, qint32 lineNumber, bool displayDialog){
	if(test)return;
	log(LogTypeCritical,o,methodName,QString("Assert failure"),"",lineNumber,displayDialog);
}


void BLogger::doubleAssert(bool test1, bool test2, QObject *o, const QString &methodName, qint32 lineNumber){
	if(!test1)log(LogTypeCritical,o,methodName,QString("Assert failure"),"test1",lineNumber);
	if(!test2)log(LogTypeCritical,o,methodName,QString("Assert failure"),"test2",lineNumber);
}


/*
 *This function should be used internally only.
 */
void BLogger::log(LogType logType, QObject *o, const QString &methodName, const QString &msg, const QString &internalMessage, qint32 lineNumber, bool displayDialog) {
	QString className;
	QString completeMethodName;
	if(o){
		className=QString(o->metaObject()->className());
		completeMethodName="("+className+")"+methodName+".";
	}else{
		if(!methodName.isEmpty())completeMethodName=methodName+".";
	}
	QString completeMsg=completeMethodName+msg;
	if(!internalMessage.isEmpty())completeMsg.append(".InternalMsg:"+ internalMessage);
	if(lineNumber>0)completeMsg.append(".EditorLine:"+QString::number(lineNumber));
	switch(logType){
		case LogTypeWarning:
			qWarning()<<"Warning:"<<completeMsg;
			if(!dumpFileName.isEmpty())dumpToFile(completeMsg);
            //if(displayDialog)QMessageBox::warning(0,"Warning",msg);
			break;
		case LogTypeDebug:{//debug nao grava no arquivo
			qDebug()<<completeMsg;
            //if(displayDialog)QMessageBox::information(0,"Information",msg);
			break;
		}
		case LogTypeError:{
			qCritical()<<"Error:"<<completeMsg;
			if(!dumpFileName.isEmpty())dumpToFile(completeMsg);
            //if(displayDialog)QMessageBox::critical(0,"Error",msg);
			break;
		}
		case LogTypeCritical:{			
            //if(displayDialog)QMessageBox::critical(0,"Critical Error",msg+"\nApplication will exit.");
			qFatal(completeMsg.toUtf8().constData());//exits the app
			if(!dumpFileName.isEmpty())dumpToFile(completeMsg);
			/*if(sendCriticalErrorsToServer)sendErrorToWebService(className,methodName,msg,internalMessage,lineNumber);
			qWarning()<<"Critical:"<<completeMsg;
			#ifdef Q_OS_WIN
			  enum { ExitCode = 0 };
			  ::TerminateProcess(::GetCurrentProcess(), ExitCode);
			#else
			  qint64 pid = QCoreApplication::applicationPid();
			  QProcess::startDetached("kill -9 " + QString::number(pid));
			#endif*/
			break;
		}
	}
}


/*
 *Closes the dump file
 */
void BLogger::closeDumpFile(){
	dumpFileName="";
	if(file.isOpen())file.close();
}


/*
 *Uses the 'dumpFileName' as dump file
 */
void BLogger::openDumpFile(const QString &dumpFile){
	if(dumpFile.trimmed().isEmpty())	return;
	dumpFileName=dumpFile;
	QFileInfo fileInfo(dumpFile);
	if(fileInfo.exists() && (fileInfo.size()>(DUMPFILESIZEMB*1000000))){
		if(!QFile::remove(dumpFile))qWarning()<<"Unable to remove dump file "+dumpFile;
	}
	file.setFileName(dumpFile);
	if(file.isOpen())return;
	if(!file.open(QIODevice::Text | QIODevice::Append))qWarning()<<"Unable to open dump file "+dumpFile;
}


/*
 *Appends completeMessage to the end of the dumpfile
 *If dumpFile is not set, it does nothing.
 */
void BLogger::dumpToFile(const QString &completeMessage){
	QString msg=QDateTime::currentDateTime().toString(Qt::ISODate)+": "+completeMessage+"\n";
	if(file.isOpen()){
		file.write(msg.toUtf8().constData());
		file.flush();//do not remove this line
	}
}
