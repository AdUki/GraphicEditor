#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <QObject>
#include <QHash>
#include <set>

#include "Lua/ElementAllocator.h"

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
    typedef std::set<ElementAllocator*> SortedAllocators;

    QHash<quint64, SortedAllocators> _allocatorsBuckets;
    QList<quint64> _allocators;

    QList<ElementUpdater*> _updaters;
    QList<ElementDeleter*> _deleters;

    QGraphicsScene* _scene;
    BaseGrid* _root;
};

#endif // ELEMENTMANAGER_H
