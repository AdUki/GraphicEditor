#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <QObject>

class QFile;

class TextFile : public QObject
{
    Q_OBJECT
public:
    explicit TextFile(QObject *parent = 0);
    explicit TextFile(const QString& fileName, QObject *parent = 0);
    ~TextFile() override;

    bool isOpened() const { return _file == nullptr; }
    bool isClosed() const { return _file != nullptr; }

    QString getText() const { return _text; }

    // For saving, renaming, filename etc.
    // Use with caution :)
    QFile& getFile()
    {
        Q_CHECK_PTR(_file);
        return *_file;
    }

signals:
    void opened(const QString& text);
    void changed(const QString& text);
    void closed();

public slots:
    void addText(const QString& newText, int startIndex);
    void removeText(int startIndex, int endIndex);
    void updateText(const QString& newText, int startIndex, int endIndex);

    void open(const QString& filename);
    void save();
    void close();

private:
    QString _text;
    QFile* _file;
};

#endif // TEXTFILE_H
