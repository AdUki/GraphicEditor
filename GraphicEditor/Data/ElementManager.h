#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <QObject>

class QGraphicsScene;

class BaseGrid;

class ElementAllocator;
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
    QList<ElementAllocator*> _allocators;
    QList<ElementUpdater*> _updaters;
    QList<ElementDeleter*> _deleters;

    QGraphicsScene* _scene;
    BaseGrid* _root;
};

#endif // ELEMENTMANAGER_H
