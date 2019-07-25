#ifndef INJECT_H
#define INJECT_H
#include <QPointer>
#include "cpplib_global.h"

class CPPLIB_EXPORT  Inject : public QObject{
    Q_OBJECT


public:
    static Inject* obj(){if(!o) o=new Inject(); return o; }
    Q_INVOKABLE QString getBla(){return "BLA BLABLA";}

signals:

public slots:

private:
    explicit Inject(QObject *parent = nullptr);
    static QPointer<Inject> o;
};

#endif // INJECT_H
