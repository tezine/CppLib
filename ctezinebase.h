#ifndef BASESIZES_H
#define BASESIZES_H
#include "ctezinebaseproperties.h"
#include "cpplib_global.h"
#include <QJSValue>
#include <QHash>

class EBaseSetup;

/*
 *Classe base do CTezine.
 * Aqui ficam as funcoes que não podem ser invocadas pelo qml
 */
class CPPLIB_EXPORT CTezineBase : public CTezineBaseProperties{
    Q_OBJECT

public:
    explicit CTezineBase(QObject *parent = nullptr);
    void setup(EBaseSetup &setup);
    Q_INVOKABLE void setDarkTheme();
    void setLightTheme();
    QString getDBFilePath(){return dbFilePath;}
    void setupScreen(qint32 desktopWidth, qint32 desktopHeight);
    QHash<QString,QJSValue> getTypescriptInjects(){return injectObjects;}//todo talvez remover
    void setQmlPagesFolder(const QString &folder){this->qmlPagesFolder=folder;}
    QString getQmlPagesFolder(){return qmlPagesFolder;}
    void setInjectJS(QJSValue jsValue){this->injectJS=jsValue;}
    QJSValue getInjectJS(){return injectJS;}

signals:
    void sigHeightChanged(qint32 height);
    void sigWidthChanged(qint32 width);

protected:
    qint32 getPixelDensity();
    bool isHDPI();
    QString dbFilePath;
    QHash<QString, QJSValue> injectObjects;
    QString qmlPagesFolder;
    QJSValue injectJS;
};
#endif
