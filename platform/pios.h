#ifndef PIOS_H
#define PIOS_H
#include <QPointer>
#include "cpplib_global.h"

class  CPPLIB_EXPORT PIOS : public QObject{
    Q_OBJECT

public:
    static PIOS *obj(){if(!o)o=new PIOS();return o;}
    void shareText(const QString &txt);

private:
    explicit PIOS();
    static QPointer<PIOS> o;
};
#endif
