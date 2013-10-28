#pragma once

#include <QGraphicsLinearLayout>
#include <QGraphicsRectItem>

#include "./BaseElement.h"

class Root :
    public QGraphicsLinearLayout,
    public QGraphicsRectItem,
    public BaseElement
{
public:
    Root();
};
