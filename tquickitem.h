#ifndef TQUICKITEM_H
#define TQUICKITEM_H

#include <QQuickItem>

class TQuickItem : public QQuickItem
{
    Q_OBJECT
public:
    TQuickItem(QQuickItem *parent = nullptr);


protected:
    void	componentComplete();

signals:

public slots:
};
QML_DECLARE_TYPE(TQuickItem)
#endif // TQUICKITEM_H
