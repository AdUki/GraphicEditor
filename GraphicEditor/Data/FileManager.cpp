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
    // TODO zavolat destruktor nakonci
    static FileManager* INSTANCE = new FileManager();
    return INSTANCE;
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
