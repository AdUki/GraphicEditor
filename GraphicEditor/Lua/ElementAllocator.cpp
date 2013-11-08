#include "ElementAllocator.h"

#include "Ui/Grids/BaseGrid.h"

#include "Data/ElementManager.h"

////////////////////////////////////////////////////////////////
ElementAllocator::ElementAllocator(void* parent)
: index(-1)
, allocatedPointer(new (void**)()) // Allocates pointer
, parent(nullptr)
{
    // Parent can be null if it is root
    if (parent != nullptr)
        this->parent = static_cast<BaseGrid**>(parent);
}

////////////////////////////////////////////////////////////////
BaseGrid* ElementAllocator::getParent()
{
    Q_ASSERT(parent != nullptr);

    BaseGrid* parentPointer = *parent;

    Q_ASSERT(parentPointer != nullptr);

    return parentPointer;
}
