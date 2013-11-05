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
        this->parent = parent;
}

////////////////////////////////////////////////////////////////
BaseGrid* ElementAllocator::getParent()
{
    Q_ASSERT(parent != nullptr);

    BaseGrid** pointerToPointer = static_cast<BaseGrid**>(parent);
    BaseGrid* parentPointer = *pointerToPointer;

    Q_ASSERT(parentPointer != nullptr);

    return parentPointer;
}
