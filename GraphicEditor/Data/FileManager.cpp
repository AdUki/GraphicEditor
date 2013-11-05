#include "FileManager.h"

#include <QDebug>
#include <QFile>

////////////////////////////////////////////////////////////////
FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
    qDebug() << "File manager created.";
}

////////////////////////////////////////////////////////////////
FileManager::~FileManager()
{
    QList<TextFile*> todelete;
    for (Files::iterator it = _files.begin(); it != _files.end(); ++it) {
        todelete.append(*it);
    }

    for (TextFile* ptr : todelete)
        delete ptr;

    qDebug() << "File manager closed.";
}

////////////////////////////////////////////////////////////////
FileManager* FileManager::getInstance()
{
    // Destructor is called in main window destructor
    static FileManager* INSTANCE = new FileManager();
    return INSTANCE;
}

////////////////////////////////////////////////////////////////
QList<TextFile*> FileManager::getModified() const
{
    QList<TextFile*> modified;
    for (TextFile* ptr : _files)
        if (ptr->isModified())
            modified.append(ptr);

    return modified;
}

////////////////////////////////////////////////////////////////
void FileManager::saveAll()
{
    for (TextFile* ptr : _files)
        ptr->save();
}

////////////////////////////////////////////////////////////////
void FileManager::removeFile(QObject *sender)
{
    _files.remove(static_cast<TextFile*>(sender));
}

////////////////////////////////////////////////////////////////
void FileManager::addFile(TextFile *file)
{
    connect(file, SIGNAL(destroyed(QObject*)), this, SLOT(removeFile(QObject*)));
    _files.insert(file);
}
