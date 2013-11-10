#pragma once

#include <QObject>
#include <QHash>
#include <set>

#include "Lua/ElementAllocator.h"

class QMutex;
class QGraphicsScene;

class BaseGrid;

class ElementDeleter;
class ElementUpdater;

class ElementManager : public QObject
{
    Q_OBJECT

public:
    explicit ElementManager(QObject *parent = 0);

    static ElementManager* getInstance();

    void addAllocator(ElementAllocator* allocator);
    void addDeleter(ElementDeleter* deleter);
    void addUpdater(ElementUpdater* updater);

    void setScene(QGraphicsScene* scene) { _scene = scene; }
    void setRoot(BaseGrid* root) { _root = root; }
    
signals:

public slots:
    void commit();
    void reset();

private:
    QMutex* _commitMutex;

    struct AllocatorCompare {
        bool operator() (ElementAllocator* lhs, ElementAllocator* rhs) const{
            return lhs->index < rhs->index;
        }
    };

    typedef std::set<ElementAllocator*, AllocatorCompare> SortedAllocators;
    typedef QHash<quint64, SortedAllocators> AllocatorsBuckets;
    typedef QList<quint64> AllocatorsIndexes;
    typedef QList<ElementUpdater*> Updaters;
    typedef QList<ElementDeleter*> Deleters;

    AllocatorsBuckets _allocatorsBuckets;
    AllocatorsIndexes _allocators;
    Updaters _updaters;
    Deleters _deleters;

    QGraphicsScene* _scene;
    BaseGrid* _root;
};
