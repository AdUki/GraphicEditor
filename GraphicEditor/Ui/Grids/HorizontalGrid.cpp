#include "HorizontalGrid.h"

#include <QDebug>

#include "../Items/BaseItem.h"

////////////////////////////////////////////////////////////////
HorizontalGrid::HorizontalGrid(QGraphicsLayoutItem *parent)
: LinearGrid(Qt::Horizontal, parent)
{
}
