#include "TextFile.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "./FileManager.h"

////////////////////////////////////////////////////////////////
TextFile::TextFile(QObject *parent) :
    QObject(parent)
{
    FileManager::getInstance()->addFile(this);

    QString t;
    t.sprintf("%8p", this); // QString::sprintf adds 0x prefix

    qDebug() << QString("File " + t + " created");
}

////////////////////////////////////////////////////////////////
TextFile::TextFile(const QString& filename, QObject *parent) :
    TextFile(parent)
{
    open(filename);
}

////////////////////////////////////////////////////////////////
TextFile::~TextFile()
{
    QString t;
    t.sprintf("%8p", this); // QString::sprintf adds 0x prefix

    qDebug() << QString("File " + t + " closed");
}

////////////////////////////////////////////////////////////////
void TextFile::addText(const QString& newText, int startIndex)
{
    Q_ASSERT(startIndex <= _text.size());
    if (newText.isEmpty())
        return;

    _text.insert(startIndex, newText);

    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::removeText(int startIndex, int endIndex)
{
    Q_ASSERT(startIndex <= endIndex && endIndex <= _text.size());
    if (startIndex == endIndex)
        return;

    _text.remove(startIndex, endIndex - startIndex);

    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::updateText(const QString& newText, int startIndex, int endIndex)
{
    Q_ASSERT(startIndex <= endIndex && endIndex <= _text.size());
    if (startIndex == endIndex)
        return;

    _text.replace(startIndex, endIndex - startIndex, newText);

    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::open(const QString& filename)
{
    _file = new QFile(filename, this);

    // TODO handle exception
    if (!_file->open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(_file);
    _text = in.readAll();

    emit opened(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::save()
{
    Q_CHECK_PTR(_file);

    // TODO handle exception
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(_file);
    out << _text;

    _file->close();

    emit closed();
}

////////////////////////////////////////////////////////////////
void TextFile::close()
{
    if (_file == nullptr)
        return;

    _text.clear();
    delete _file;

    emit closed();
}
