#include "ElementDeleter.h"

#include <QDebug>

#include "Ui/Items/BaseItem.h"
#include "Ui/Grids/BaseGrid.h"

////////////////////////////////////////////////////////////////
ElementDeleter::ElementDeleter(void* pointerInstance)
{
    Q_ASSERT(pointerInstance != nullptr);

    pointer = static_cast<QGraphicsItem**>(pointerInstance);
    element = *pointer;
}
