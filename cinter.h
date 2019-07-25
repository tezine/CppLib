#ifndef CINTER_H
#define CINTER_H
#include <QPointer>
#include "cpplib_global.h"

/*
 *Interfaces between C++, Java and Objective C
 */
class CPPLIB_EXPORT CInter : public QObject{
    Q_OBJECT

public:
    static CInter *obj(){if(!o)o=new CInter();return o;}

signals:

public slots:

private:
    explicit CInter(QObject *parent = 0);
    static QPointer<CInter> o;
};
#endif
