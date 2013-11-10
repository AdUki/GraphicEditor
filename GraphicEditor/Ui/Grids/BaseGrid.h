#pragma once

#include <QGraphicsRectItem>

#include "../BaseElement.h"

#include "../Items/BaseItem.h"

class QGraphicsLayout;

class BaseGrid :
    public BaseElement,
    public QGraphicsRectItem
{
public:
    BaseGrid();
    ~BaseGrid() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void insertElement(BaseItem* item, int index) = 0;
    virtual void insertElement(BaseGrid* grid, int index) = 0;

//    void addItem(BaseItem* item) { insertItem(item, -1); }
//    void addItem(BaseGrid* grid) { insertItem(grid, -1); }

    /// @return layout interface with layout functions
    QGraphicsLayout* layout();
    const QGraphicsLayout* constLayout() const;

protected:

private:

};
