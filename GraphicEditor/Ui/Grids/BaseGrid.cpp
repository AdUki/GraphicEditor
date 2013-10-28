#include "BaseGrid.h"

#include <QPainter>
#include <QDebug>

////////////////////////////////////////////////////////////////
BaseGrid::~BaseGrid()
{
    qDebug() << "~BaseGrid()";
}

////////////////////////////////////////////////////////////////
void BaseGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: test code
    static int cycle = 0;
    const static QBrush brush[7] = {
        QBrush(QColor(150,150,205)),
        QBrush(QColor(178,178,150)),
        QBrush(QColor(178,150,178)),
        QBrush(QColor(150,178,178)),
        QBrush(QColor(160,160,160)),
        QBrush(QColor(205,150,150)),
        QBrush(QColor(150,205,150)),
    };
    QRectF bounds = boundingRect();

    painter->fillRect(bounds, brush[cycle]);
    cycle = (cycle + 1) % 7;
}
