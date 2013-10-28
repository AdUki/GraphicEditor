#pragma once

#include "./BaseGrid.h"

#include <QGraphicsLinearLayout>

class HorizontalGrid :
    public QGraphicsLinearLayout,
    public BaseGrid
{
public:
    HorizontalGrid(QGraphicsLayoutItem *parent = 0);

    QRectF boundingRect() const override;
};
