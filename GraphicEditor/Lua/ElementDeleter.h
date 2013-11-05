#ifndef ELEMENTDELETER_H
#define ELEMENTDELETER_H

#include <QObject>

class QGraphicsItem;

class ElementDeleter
{
public:
    ElementDeleter(void* pointerInstance);

    QGraphicsItem** pointer;
    QGraphicsItem* element;
};

#endif // ELEMENTDELETER_H
