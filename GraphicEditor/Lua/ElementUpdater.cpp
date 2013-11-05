#include "ElementUpdater.h"

#include <QDebug>

#include "Ui/Items/BaseItem.h"
#include "Ui/Grids/BaseGrid.h"

////////////////////////////////////////////////////////////////
ElementUpdater::ElementUpdater(void* pointerInstance)
    : text("")
{
    Q_ASSERT(pointerInstance != nullptr);

    // We can only update items
    item = *static_cast<BaseItem**>(pointerInstance);

    Q_ASSERT(item != nullptr);
}
