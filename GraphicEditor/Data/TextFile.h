#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <QObject>

class BaseGrid;
class QFile;
class QGraphicsWidget;
class QGraphicsScene;

class TextFile : public QObject
{
    Q_OBJECT
public:
    explicit TextFile(QObject *parent = 0);
    explicit TextFile(const QString& fileName, QObject *parent = 0);
    ~TextFile() override;

    bool isOpened() const { return _file == nullptr; }
    bool isClosed() const { return _file != nullptr; }
    bool isModified() const { return _modified; }

    QString getText() const { return _text; }
    BaseGrid* getRoot();

    void setScene(QGraphicsScene* scene);
    void resetScene(QGraphicsScene* scene);

    void reparse() const;
    void setGrammar(const QString& grammar) const;
    void setText(const QString& text);

    // For saving, renaming, filename etc.
    // Use with caution :)
    QFile& getFile()
    {
        Q_ASSERT(_file != nullptr);
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
    void saveAs(const QString& filename);
    void close();

private:
    mutable QString _pointer;
    QString _text;
    QFile* _file;
    BaseGrid* _root;
    QGraphicsWidget* _container;

    bool _modified;

    inline QString thisToString() const;
};

#endif // TEXTFILE_H
