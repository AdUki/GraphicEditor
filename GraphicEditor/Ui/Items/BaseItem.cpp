#include "BaseItem.h"

#include <QPainter>

////////////////////////////////////////////////////////////////
BaseItem::BaseItem()
{
}

////////////////////////////////////////////////////////////////
void BaseItem::setText(const QString &text)
{
    _text = text;
}

////////////////////////////////////////////////////////////////
QRectF BaseItem::boundingRect() const
{
    const QRectF& rect = QRectF(QPointF(0,0), measureSize());
    // TODO: pri editacii sa vrati text edit
    return rect;
}

////////////////////////////////////////////////////////////////
void BaseItem::setGeometry(const QRectF &rect)
{
    // Default is top left position
//    prepareGeometryChange();
//    QGraphicsLayoutItem::setGeometry(rect);

    QPointF position(rect.topLeft());
    position.setY(position.y() - rect.height() / 2);
    setPos(position.toPoint());
}

////////////////////////////////////////////////////////////////
QSizeF BaseItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    /*
    switch (which) {
    case Qt::MinimumSize: break;
    case Qt::PreferredSize: break;
    case Qt::MaximumSize: break;
    default: break;
    }
    */
    QSizeF size = measureSize();
    return size;
}

////////////////////////////////////////////////////////////////
void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: test code
    static int cycle = 0;
    const static QBrush brush[7] = {
        QBrush(QColor(255,200,200)),
        QBrush(QColor(200,255,200)),
        QBrush(QColor(200,200,255)),
        QBrush(QColor(228,228,200)),
        QBrush(QColor(228,200,228)),
        QBrush(QColor(200,228,228)),
        QBrush(QColor(210,210,210)),
    };
    painter->fillRect(boundingRect(), brush[cycle]);
    cycle = (cycle + 1) % 7;


    // TODO: pri editacii sa vykresli TextEdit
    draw(painter, boundingRect());
}
