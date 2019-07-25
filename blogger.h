#ifndef CLOGGER_H
#define CLOGGER_H
#include <QObject>
#include "cdefines.h"
#include "cpplib_global.h"

/*
 * Logs all messages.
 * Depending on the level(log, error, critical), it may also send the message to the server
 */
class CPPLIB_EXPORT BLogger : public QObject {
	public:
        static void assertError(bool test, QObject* o, const QString& methodName, const QString& userMsg, const QString& internalMsg, qint32 lineNumber, bool displayDialog=false); //exit app if false
        static void assertError(bool test, QObject* o,const QString& methodName, qint32 lineNumber,bool displayDialog=false); //exit app if false
		static void setup(const QString& dumpFileName);
		static void debug(QObject* o, const QString& methodName, const QString& msg);
		static void warning(QObject* o, const QString& methodName, const QString& msg,bool displayDialog=false);
		static void error(QObject* o, const QString& methodName, const QString& msg, bool displayDialog=false);
		static void doubleAssert(bool test1, bool test2,QObject* o,const QString& methodName, qint32 lineNumber); //exit app if false
		static void criticalError(QObject* o, const QString& methodName, const QString& userMsg, const QString& internalMsg, qint32 lineNumber,bool displayDialog=false); //exit app
		static void criticalError(QObject* o, const QString& methodName, const QString& internalMsg, qint32 lineNumber,bool displayDialog=false); //exit app
		static void criticalError(QObject* o, const QString& methodName, qint32 lineNumber,bool displayDialog=false); //exit app
		static void log(LogType logType,QObject* o, const QString& methodName, const QString& msg=0, const QString& internalMessage=0, qint32 lineNumber=0, bool displayDialog=false);
		static void closeDumpFile();
		static void openDumpFile(const QString& dumpFileName);

	private:
		static void dumpToFile(const QString& completeMessage);
};
#endif
