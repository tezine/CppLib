#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>

class BaseItem : public QQuickItem
{
    Q_OBJECT
public:
    BaseItem();

signals:

public slots:

protected:
    void componentComplete();
};

#endif // BASEITEM_H
