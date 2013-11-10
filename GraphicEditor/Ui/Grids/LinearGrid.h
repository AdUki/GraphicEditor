#pragma once

#include "./BaseGrid.h"

#include <QGraphicsLinearLayout>

class LinearGrid :
    public BaseGrid,
    public QGraphicsLinearLayout
{
public:
    LinearGrid(Qt::Orientation orientation, QGraphicsLayoutItem *parent = 0);

    void setGeometry(const QRectF &rect) override;

    void insertElement(BaseItem* item, int index) override;
    void insertElement(BaseGrid* grid, int index) override;
};
