#ifndef EQML_H
#define EQML_H
#include <QPointer>
#include "cpplib_global.h"

class ELine;
class EnhancedQmlApplicationEngine;

/*
 * Class responsible to invoke qml javascript functions, ...
 */
class CPPLIB_EXPORT CQML : public QObject {
	Q_OBJECT

	public:
        explicit CQML(QObject* parent = nullptr);
        static CQML* obj(){if(!o) o=new CQML(); return o; }
		static qint32 runFunction( QObject* obj, const QString& methodName, const QVariantList& argsList);
        static qint32 runFunctionWithoutArgs( QObject* obj, const QString& methodName);
        void setQMLEngine(EnhancedQmlApplicationEngine *engine){this->engine=engine;}
        EnhancedQmlApplicationEngine *getQMLEngine(){return engine;}

	private:
        static QPointer<CQML> o;
        EnhancedQmlApplicationEngine *engine;
};
#endif
