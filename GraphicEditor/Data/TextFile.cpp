#include "TextFile.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "./FileManager.h"
#include "./Interpreter.h"

////////////////////////////////////////////////////////////////
TextFile::TextFile(QObject *parent)
    : QObject(parent)
    , _file(nullptr)
    , _modified(false)
{
    FileManager::getInstance()->addFile(this);
    Interpreter::getInstance()->makeRegisterFileCall(thisToString());

    qDebug() << QString("File " + thisToString() + " created");
}

////////////////////////////////////////////////////////////////
TextFile::TextFile(const QString& fileName, QObject *parent)
    : QObject(parent)
    , _file(nullptr)
    , _modified(false)
{
    FileManager::getInstance()->addFile(this);
    Interpreter::getInstance()->makeRegisterFileCall(thisToString());

    open(fileName);

    qDebug() << QString("File " + thisToString() + " created");
}

////////////////////////////////////////////////////////////////
TextFile::~TextFile()
{
    Interpreter::getInstance()->makeUnregisterFileCall(thisToString());
    qDebug() << "File " + thisToString() + " closed";
}

////////////////////////////////////////////////////////////////
void TextFile::setText(const QString &text)
{
    _modified = true;
    _text = text;
    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::addText(const QString& newText, int startIndex)
{
    Q_ASSERT(startIndex <= _text.size());
    if (newText.isEmpty())
        return;

    _modified = true;
    _text.insert(startIndex, newText);

    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::removeText(int startIndex, int endIndex)
{
    Q_ASSERT(startIndex <= endIndex && endIndex <= _text.size());
    if (startIndex == endIndex)
        return;

    _modified = true;
    _text.remove(startIndex, endIndex - startIndex);

    emit changed(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::updateText(const QString& newText, int startIndex, int endIndex)
{
    Q_ASSERT(startIndex <= endIndex && endIndex <= _text.size());
    if (startIndex == endIndex)
        return;

    _modified = true;
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

    QFileInfo info(*_file);
    Interpreter::getInstance()->makeSetFileAbsolutePathCall(thisToString(), info.absolutePath());

    emit opened(_text);
}

////////////////////////////////////////////////////////////////
void TextFile::save()
{
    if (_file == nullptr) {
        QString newFileName = QFileDialog::getSaveFileName();

        // TODO: vytvorit triedu pre exception
        if (newFileName.isEmpty())
            throw 1; // User canceled saving

        _file = new QFile(newFileName, this);
    }

    // TODO handle exception
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(_file);
    out << _text;

    // TODO: testovat ci sa subor zatvoril spravne
    _file->close();

    _modified = false;

    emit closed();
}

////////////////////////////////////////////////////////////////
void TextFile::saveAs(const QString& filename)
{
    if (_file != nullptr)
        delete _file;

    _file = new QFile(filename, this);
    save();
}

////////////////////////////////////////////////////////////////
void TextFile::close()
{
    if (_file == nullptr)
        return;

    _modified = false;
    _text.clear();
    delete _file;

    emit closed();
}

////////////////////////////////////////////////////////////////
void TextFile::reparse() const
{
    Interpreter::getInstance()->makeReparseFileCall(thisToString(), _text);
}

////////////////////////////////////////////////////////////////
void TextFile::setGrammar(const QString &grammar) const
{
    Interpreter::getInstance()->makeSetFileGrammarCall(thisToString(), grammar);
}

////////////////////////////////////////////////////////////////
QString TextFile::thisToString() const
{
    if (_pointer.isEmpty())
        _pointer.sprintf("%p", this);

    return _pointer;
}
