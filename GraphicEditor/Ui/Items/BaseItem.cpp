#include "BaseItem.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsLinearLayout>

////////////////////////////////////////////////////////////////
BaseItem::BaseItem(QGraphicsLayoutItem *parent)
: QGraphicsLayoutItem(parent)
{
}

////////////////////////////////////////////////////////////////
BaseItem::~BaseItem()
{
    qDebug() << "~BaseItem()";
}

////////////////////////////////////////////////////////////////
void BaseItem::setText(const QString &text)
{
    if (_text == text)
        return;

    _text = text;

    update();
}

////////////////////////////////////////////////////////////////
QRectF BaseItem::boundingRect() const
{
    QSizeF size = measureSize();
    const QRectF& rect = QRectF(QPointF(0, 0), size);
    // TODO: pri editacii sa vrati text edit
    return rect;
}

////////////////////////////////////////////////////////////////
void BaseItem::setGeometry(const QRectF &rect)
{
    // Top left position for vertical layout
    QPointF position(rect.topLeft());

    // Center position for horizontal layout
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout*>(parentLayoutItem());
    if (layout != nullptr && layout->orientation() == Qt::Horizontal)
        position.setY(position.y() + (layout->geometry().size().height() - rect.height()) / 2);

    // Set position
    setPos(position.toPoint());

    updateGeometry();
}

////////////////////////////////////////////////////////////////
QSizeF BaseItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

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
    QRectF bounds = boundingRect();

    painter->fillRect(bounds, brush[cycle]);
    cycle = (cycle + 1) % 7;


    // TODO: pri editacii sa vykresli TextEdit
    draw(painter, boundingRect());
}
