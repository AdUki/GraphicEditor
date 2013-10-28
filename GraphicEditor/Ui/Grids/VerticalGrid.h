#pragma once

#include "./BaseGrid.h"

#include <QGraphicsLinearLayout>

class VerticalGrid :
    public QGraphicsLinearLayout,
    public BaseGrid
{
public:
    VerticalGrid(QGraphicsLayoutItem *parent = 0);

    QRectF boundingRect() const override;
};
