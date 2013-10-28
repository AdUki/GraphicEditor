#include "VerticalGrid.h"

////////////////////////////////////////////////////////////////
VerticalGrid::VerticalGrid(QGraphicsLayoutItem *parent)
: QGraphicsLinearLayout(Qt::Vertical, parent)
, BaseGrid()
{
}

////////////////////////////////////////////////////////////////
QRectF VerticalGrid::boundingRect() const
{
//    QRectF bounds(this->pos(), sizeHint(Qt::PreferredSize));
    return geometry();
}
