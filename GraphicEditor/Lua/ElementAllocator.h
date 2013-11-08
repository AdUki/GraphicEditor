#pragma once

#include <QDebug>
#include <QString>

class BaseGrid;

////////////////////////////////////////////////////////////////
enum class ElementType {
    Item = 1,
    Grid
};

////////////////////////////////////////////////////////////////
class ElementAllocator
{
public:
    /// Creates allocator with parent and allocates @variable{allocatedPointer}
    /// @attention you still must set up other arguments
    /// @param parent can be null if it is root
    ElementAllocator(void *parent);

    /// Indicates position in parent child hierarchy.
    int index;

    /// Element type (Grid or Item)
    ElementType type;

    /// Text of element
    const char* text;

    /// Allocated pointer on witch can point to another pointer.
    /// It is uset to delayed allocation or possible reuse of new elements.
    void* const allocatedPointer;

    /// Pointer of parent, which can be only grid
    /// @attention Is null if parent is root
    BaseGrid** parent;

    /// @return true if parent is root, false otherwise
    bool isParentRoot() const { return parent == nullptr; }

    template<typename T>
    T* allocatePointer(T* object) {
        T** pointerToPointer = static_cast<T**>(allocatedPointer);
        *pointerToPointer = object;
        return *pointerToPointer;
    }

    quint64 qHash(uint seed) const
    {
        Q_UNUSED(seed);
        return parent == nullptr ? 0 : reinterpret_cast<quint64>(*parent);
    }

    bool operator==(const ElementAllocator& elem) const {
        return elem.qHash(0) == elem.qHash(0);
    }

    bool operator<(const ElementAllocator& elem) const {
        return index < elem.index;
    }

    BaseGrid* getParent();
};
