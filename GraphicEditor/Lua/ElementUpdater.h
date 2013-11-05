#pragma once

#include <QObject>

class BaseItem;
class BaseGrid;

class ElementUpdater
{
public:
    ElementUpdater(void *pointerInstance);

    BaseItem* item;
    
    const char* text;
};
