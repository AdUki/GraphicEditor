#include "HorizontalGrid.h"

////////////////////////////////////////////////////////////////
HorizontalGrid::HorizontalGrid(QGraphicsLayoutItem *parent)
: QGraphicsLinearLayout(Qt::Horizontal, parent)
, BaseGrid()
{
}

////////////////////////////////////////////////////////////////
QRectF HorizontalGrid::boundingRect() const
{
//    QRectF bounds(this->pos(), sizeHint(Qt::PreferredSize));
    return geometry();
}
