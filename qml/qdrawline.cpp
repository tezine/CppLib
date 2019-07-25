#include "qdrawline.h"

/*QLine::QLine()
{

}*/


QDrawLine::QDrawLine(QQuickItem *parent) :QQuickPaintedItem(parent), m_x1(0), m_y1(0), m_x2(0), m_y2(0),m_color(Qt::black), m_penWidth(1){
    // Important, otherwise the paint method is never called
    //setFlag(QGraphicsItem::ItemHasNoContents, false);
}

void QDrawLine::paint(QPainter *painter){
    QPen pen(m_color, m_penWidth);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    if(smooth() == true) {
        painter->setRenderHint(QPainter::Antialiasing, true);
    }
    int x = qMin(m_x1, m_x2) - m_penWidth/2;
    int y = qMin(m_y1, m_y2) - m_penWidth/2;
    painter->drawLine(m_x1 - x, m_y1 - y, m_x2 - x, m_y2 - y);
}

void QDrawLine::setX1(int x1) {
    if(m_x1 == x1) return;
    m_x1 = x1;
    updateSize();
    emit x1Changed();
    update();
}

void QDrawLine::setY1(int y1) {
    if(m_y1 == y1) return;
    m_y1 = y1;
    updateSize();
    emit y1Changed();
    update();
}

void QDrawLine::setX2(int x2) {
    if(m_x2 == x2) return;
    m_x2 = x2;
    updateSize();
    emit x2Changed();
    update();
}

void QDrawLine::setY2(int y2) {
    if(m_y2 == y2) return;
    m_y2 = y2;
    updateSize();
    emit x2Changed();
    update();
}

void QDrawLine::setColor(const QColor &color) {
    if(m_color == color) return;
    m_color = color;
    emit colorChanged();
    update();
}

void QDrawLine::setPenWidth(int newWidth) {
    if(m_penWidth == newWidth) return;
    m_penWidth = newWidth;
    updateSize();
    emit penWidthChanged();
    update();
}

void QDrawLine::updateSize() {
    setX(qMin(m_x1, m_x2) - m_penWidth/2);
    setY(qMin(m_y1, m_y2) - m_penWidth/2);
    setWidth(qAbs(m_x2 - m_x1) + m_penWidth);
    setHeight(qAbs(m_y2 - m_y1) + m_penWidth);
}
