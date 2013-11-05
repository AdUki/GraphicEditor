#include "VerticalGrid.h"

#include <QDebug>

#include "../Items/BaseItem.h"

////////////////////////////////////////////////////////////////
VerticalGrid::VerticalGrid(QGraphicsLayoutItem *parent)
: LinearGrid(Qt::Vertical, parent)
{
}
