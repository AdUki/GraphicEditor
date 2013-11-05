#include "ElementManager.h"

#include <QGraphicsScene>
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
ElementManager::ElementManager(QObject *parent) :
    QObject(parent)
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
    _allocators.push_back(allocator);
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

    qDebug() << "Commiting chagnes...\n";

    ////////////////////////////////////////////////////////////////
    // First we delete old items
    for (ElementDeleter* deleter : _deleters) {

        qDebug() << "Remove element " << deleter->pointer;

        // TODO: reuse objects

        // Remove created object
        _scene->removeItem(deleter->element);
        delete deleter->element;

        // Remove allocated pointer
        delete deleter->pointer;

        delete deleter;
    }

    ////////////////////////////////////////////////////////////////
    // Then we update existing
    for (ElementUpdater* updater : _updaters) {

        qDebug() << "Update element " << updater->item;

        // TODO: co ked sa text pred tym zmenil???
        updater->item->setText(updater->text);

        delete updater;
    }

    ////////////////////////////////////////////////////////////////
    // At last we add new items
    for (ElementAllocator* allocator : _allocators) {

        qDebug() << "Allocate element " << allocator->allocatedPointer << " to parent " << allocator->parent;

        switch(allocator->type) {
        case ElementType::Item: {
            BaseItem* item = allocator->allocatePointer(new TextItem(allocator->text));

            if (allocator->isParentRoot())
                _root->insertElement(item, allocator->index);
            else {
                BaseGrid* parent = allocator->getParent();
                QGraphicsLayout* layout = parent->layout();
                qDebug() << "Adding to layout with " << layout->count() << " items";
                parent->insertElement(item, allocator->index);
            }
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
        }

        delete allocator;
    }

    reset();
}

////////////////////////////////////////////////////////////////
void ElementManager::reset()
{
    // We disconnect from Interpreter
    disconnect(Interpreter::getInstance(), SIGNAL(workDone()), this, SLOT(commit()));

    // Clear all changes
    _deleters.clear();
    _allocators.clear();
    _updaters.clear();
}
