#include "LinearGrid.h"

#include <QDebug>

#include "../Items/BaseItem.h"

////////////////////////////////////////////////////////////////
LinearGrid::LinearGrid(Qt::Orientation orientation, QGraphicsLayoutItem *parent)
: BaseGrid()
, QGraphicsLinearLayout(orientation, parent)
{
}

////////////////////////////////////////////////////////////////
QRectF LinearGrid::boundingRect() const
{
//    QRectF bounds(this->pos(), sizeHint(Qt::PreferredSize));
    return geometry();
}

////////////////////////////////////////////////////////////////
void LinearGrid::insertElement(BaseItem* item, int index)
{
    if (index < 0)
        QGraphicsLinearLayout::insertItem(childItems().size() + index + 1, item);
    else
        QGraphicsLinearLayout::insertItem(index, item);

    updateGeometry();
}

////////////////////////////////////////////////////////////////
void LinearGrid::insertElement(BaseGrid* grid, int index)
{
    if (index < 0)
        QGraphicsLinearLayout::insertItem(childItems().size() + index + 1, grid->layout());
    else
        QGraphicsLinearLayout::insertItem(index, grid->layout());

    updateGeometry();
}
