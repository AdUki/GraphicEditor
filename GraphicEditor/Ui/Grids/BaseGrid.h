#pragma once

#include <QGraphicsRectItem>

#include "../BaseElement.h"

class BaseGrid :
    public BaseElement,
    public QGraphicsRectItem
{
public:
    ~BaseGrid() override;

//    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

private:

};
