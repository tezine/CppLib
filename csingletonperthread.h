#ifndef SINGLETONPERTHREAD_H
#define SINGLETONPERTHREAD_H
#include <QObject>
#include "cpplib_global.h"

/*
 *Creates one object per thread
 */
class CPPLIB_EXPORT CSingletonPerThread {
	public:
        CSingletonPerThread();
		static void addObject(QObject* object);
		static bool containsObject(const QString& className);
		static QObject* getObject(const QString& className);
};
#endif
