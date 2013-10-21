#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QSet>

#include "./TextFile.h"

class FileManager : public QObject
{
    Q_OBJECT

    explicit FileManager(QObject *parent = 0);

public:
    ~FileManager() override;
    static FileManager* getInstance();

signals:
    
public slots:

private slots:
    void removeFile(QObject* sender);
    
private:
    friend class TextFile;

    typedef QSet<TextFile*> Files;

    Files _files;

    void addFile(TextFile* file);
};

#endif // FILEMANAGER_H
