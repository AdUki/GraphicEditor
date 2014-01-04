#include "ElementManager.h"

#include <QGraphicsScene>
#include <QMutex>
#include <QDebug>

#include "Ui/Items/BaseItem.h"
#include "Ui/Items/TextItem.h"
#include "Ui/Grids/BaseGrid.h"
#include "Ui/Grids/HorizontalGrid.h"

#include "Lua/ElementAllocator.h"
#include "Lua/ElementUpdater.h"
#include "Lua/ElementDeleter.h"

#include "./Interpreter.h"

////////////////////////////////////////////////////////////////
ElementManager::ElementManager(QObject *parent)
: QObject(parent)
, _commitMutex(new QMutex())
{
}

////////////////////////////////////////////////////////////////
ElementManager* ElementManager::getInstance()
{
    // Destructor is called in main window destructor
    static ElementManager* INSTANCE = new ElementManager();
    return INSTANCE;
}

////////////////////////////////////////////////////////////////
void ElementManager::addAllocator(ElementAllocator *allocator)
{
    quint64 key = reinterpret_cast<quint64>(allocator->parent);

    if (_allocatorsBuckets.contains(key)) {
        auto iterator = _allocatorsBuckets.find(key);
        Q_ASSERT(iterator != _allocatorsBuckets.end());
        Q_ASSERT(iterator->find(allocator) == iterator->end());
        iterator->insert(allocator);
    }
    else {
        SortedAllocators newSet;
        newSet.insert(allocator);

        _allocatorsBuckets[key] = newSet;
        _allocators.push_back(key);
    }
}

////////////////////////////////////////////////////////////////
void ElementManager::addDeleter(ElementDeleter *deleter)
{
    _deleters.push_back(deleter);
}

////////////////////////////////////////////////////////////////
void ElementManager::addUpdater(ElementUpdater *updater)
{
    _updaters.push_back(updater);
}

////////////////////////////////////////////////////////////////
void ElementManager::commit()
{
    Q_ASSERT(_root != nullptr);
    Q_ASSERT(_scene != nullptr);

    // We swap all data structures with new one because another thread can is updating those structures while commiting changes
    AllocatorsBuckets allocatorsBuckets;
    AllocatorsIndexes allocatorsIndexes;
    Updaters updaters;
    Deleters deleters;

    std::swap(_allocatorsBuckets, allocatorsBuckets);
    std::swap(_allocators, allocatorsIndexes);
    std::swap(_updaters, updaters);
    std::swap(_deleters, deleters);

    qDebug() << "Commiting chagnes...\n";

    ////////////////////////////////////////////////////////////////
    // First we delete old items
    for (ElementDeleter* deleter : deleters) {

        qDebug() << "Remove element " << deleter->pointer;

        // TODO: reuse objects

        // Remove created object
        if (deleter->element != nullptr) {
            _scene->removeItem(deleter->element);
            delete deleter->element;
        }

        // Remove allocated pointer
        if (deleter->pointer != nullptr)
            delete deleter->pointer;

        delete deleter;
    }

    ////////////////////////////////////////////////////////////////
    // Then we update existing
    for (ElementUpdater* updater : updaters) {

        qDebug() << "Update element " << updater->item;

        // TODO: co ked sa text pred tym zmenil???
        updater->item->setText(updater->text);

        delete updater;
    }

    ////////////////////////////////////////////////////////////////
    // At last we add new items
    for (quint64 key : allocatorsIndexes) {
        SortedAllocators allocators = allocatorsBuckets.value(key);

        for (ElementAllocator* allocator : allocators) {

            qDebug() << "Allocate element [" << (allocator->type == ElementType::Item ? allocator->text : "grid") << ":" << allocator->allocatedPointer
                     << "] to parent " << allocator->parent << " at index" << allocator->index;

            switch(allocator->type) {

            case ElementType::Item: {
                BaseItem* item = allocator->allocatePointer(new TextItem(allocator->text));

                if (allocator->isParentRoot())
                    _root->insertElement(item, allocator->index);
                else
                    allocator->getParent()->insertElement(item, allocator->index);

                _scene->addItem(item);
                break;
            }

            case ElementType::Grid: {
                BaseGrid *grid = allocator->allocatePointer(new HorizontalGrid());
                _scene->addItem(grid);

                if (allocator->isParentRoot())
                    _root->insertElement(grid, allocator->index);
                else
                    allocator->getParent()->insertElement(grid, allocator->index);
                break;
            }

            default:
                Q_ASSERT_X(false, "switch", "Unknown element type!");
            }

            delete allocator;
        }

    }

    reset();
}

////////////////////////////////////////////////////////////////
void ElementManager::reset()
{
    // We disconnect from Interpreter
    disconnect(Interpreter::getInstance(), SIGNAL(workDone()), this, SLOT(commit()));
}
