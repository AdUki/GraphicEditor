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
void LinearGrid::setGeometry(const QRectF &rect)
{
    QRectF newRect(rect);
    newRect.adjust(-5, -5, 5, 5); // FIXME: nefunguje pridanie borderu
    QGraphicsLinearLayout::setGeometry(rect);
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
