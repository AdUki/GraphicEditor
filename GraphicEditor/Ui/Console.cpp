#include "console.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>

const QColor Console::InputColor = QColor::fromRgb(0,255,0);
const QColor Console::OutputColor = QColor::fromRgb(64,192,255);
const QColor Console::ErrorColor = QColor::fromRgb(255,0,0);
const QColor Console::SystemColor = QColor::fromRgb(0,0,255);

////////////////////////////////////////////////////////////////
Console::Console(QWidget *parent)
: QTextEdit(parent)
, _historyIndex(-1)
{
    setStyleSheet(R"(
    Console {
        background-color: #002900;
        color: green;
        font-family: Consolas, "consolas", monospace;
        font-size: 10pt
    }
    )");

    setReadOnly(true);
    setLineWrapMode(QTextEdit::NoWrap);
    setLineWrapMode(QTextEdit::WidgetWidth);
}

////////////////////////////////////////////////////////////////
void Console::nextCommand()
{
    setReadOnly(false);
    writeCommandSign();
    ensureCursorVisible();
}

////////////////////////////////////////////////////////////////
void Console::waitForCommand()
{
    setReadOnly(true);
}

////////////////////////////////////////////////////////////////
void Console::clear()
{
    clear();
    _command.clear();
}

////////////////////////////////////////////////////////////////
void Console::writeSystem(const QString& message)
{
    setTextColor(SystemColor);
    insertPlainText(message);
    ensureCursorVisible();
}

////////////////////////////////////////////////////////////////
void Console::writeOutput(const QByteArray& data)
{
    setTextColor(OutputColor);
    insertPlainText(data);
    ensureCursorVisible();
}

////////////////////////////////////////////////////////////////
void Console::writeError(const QByteArray& data)
{
    setTextColor(ErrorColor);
    insertPlainText(data);
    ensureCursorVisible();
}

////////////////////////////////////////////////////////////////
void Console::confirmCommand()
{
    if (!_command.isEmpty())
        _commandHistory.append(_command);

    QByteArray input(_command.toUtf8().append('\n'));
    emit executeCommand(input);

    _command.clear();
}

////////////////////////////////////////////////////////////////
void Console::writeCommandSign()
{
    setTextColor(SystemColor);
    insertPlainText("> ");
}

////////////////////////////////////////////////////////////////
void Console::keyPressEvent ( QKeyEvent * e )
{
    // Terminate running process
    if (isReadOnly() &&
        e->key() == Qt::Key_C &&
        QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        qDebug() << "TERMINATE";
        emit terminateCommand();
        return;
    }

    // History operations
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::EndOfLine);
    if ((e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) &&
        cursor.atEnd() &&
        QApplication::keyboardModifiers() != Qt::ShiftModifier)
    {
        if (_commandHistory.empty())
            return;

        const QString& currentCommand = _historyIndex == -1 ? _command : _commandHistory[_historyIndex];

        if (_historyIndex == -1) {
            if (e->key() == Qt::Key_Up)
                _historyIndex = _commandHistory.size() - 1;
            else
                return;
        }
        else if (e->key() == Qt::Key_Down) {
            if (_historyIndex == _commandHistory.size() - 1)
                _historyIndex = -1;
            else
                ++_historyIndex;
        }
        else {
            if (_historyIndex == 0)
                return;
            else
                --_historyIndex;
        }

        const QString& newCommand = _historyIndex == -1 ? _command : _commandHistory[_historyIndex];

        setTextCursor(cursor);
        cursor.setPosition(cursor.position() - currentCommand.length(), QTextCursor::KeepAnchor);
        cursor.removeSelectedText();

        setTextColor(QColor::fromRgb(0,255,0));
        insertPlainText(newCommand);

        ensureCursorVisible();

        return;
    }
    else if (_historyIndex != -1) {
        _command = _commandHistory[_historyIndex];
        _historyIndex = -1;
    }

    // Enter operation
    if ((e->key() == Qt::Key_Return ||
        e->key() == Qt::Key_Enter) &&
        QApplication::keyboardModifiers() != Qt::ShiftModifier)
    {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);

        QTextEdit::keyPressEvent(e);

        confirmCommand();
        return;
    }

    // Delete operations
    else if (e->key() == Qt::Key_Backspace ||
        e->key() == Qt::Key_Delete ||
        e->matches(QKeySequence::Cut))
    {
        unsigned cursor_pos;

        int selection_size = textCursor().selectedText().length();
        if (selection_size == 0) {
            if (e->key() == Qt::Key_Backspace)
                cursor_pos = textCursor().position() - 1;
            else
                cursor_pos = textCursor().position();
            selection_size = 1;
        } else {
            cursor_pos = textCursor().selectionStart();
        }

        int command_pos = cursor_pos + _command.length() - (document()->characterCount() - 1);

        if (command_pos < 0) {
            selection_size += command_pos;
            cursor_pos -= command_pos;
            command_pos = 0;
        }

        if (selection_size > 0) {
            cursor.setPosition(cursor_pos);
            cursor.setPosition(cursor_pos + selection_size, QTextCursor::KeepAnchor);
            setTextCursor(cursor);

            _command.remove(command_pos, selection_size);
            QTextEdit::keyPressEvent(e);
        }
        return;
    }
    // Input operations
    else if (e->matches(QKeySequence::Paste) ||
             (!e->text().isEmpty() &&
             QApplication::keyboardModifiers() != Qt::ControlModifier)) {

        unsigned cursor_pos;

        int selection_size = textCursor().selectedText().length();
        if (selection_size > 0)
            cursor_pos = textCursor().selectionEnd();
        else
            cursor_pos = textCursor().position();

        int command_pos = cursor_pos + _command.length() - (document()->characterCount() - 1);
        if (command_pos < 0)
            return;

        cursor_pos -= selection_size;
        command_pos -= selection_size;
        if (command_pos < 0) {
            selection_size += command_pos;
            cursor_pos -= command_pos;
            command_pos = 0;
        }

        // Delete selected text
        if (selection_size > 0) {

            if (command_pos >= 0)
                _command.remove(command_pos, selection_size);

            cursor.setPosition(cursor_pos);
            cursor.setPosition(cursor_pos + selection_size, QTextCursor::KeepAnchor);
            setTextCursor(cursor);

            cursor.removeSelectedText();
        }

        cursor.clearSelection();

        // Paste operation
        if (e->matches(QKeySequence::Paste)) {
            QClipboard *clipboard = QApplication::clipboard();
            pasteInput(clipboard->text().toUtf8(), command_pos);
        }
        // Write operation
        else if (!e->text().isEmpty()) {
            _command.insert(command_pos, e->text());
            setTextColor(InputColor);
            QTextEdit::keyPressEvent(e);
        }
        return;
    }

    QTextEdit::keyPressEvent(e);
}

////////////////////////////////////////////////////////////////
void Console::pasteInput(const QString &input, int commandCursorPosition)
{
    _command.insert(commandCursorPosition, input);

    setTextColor(InputColor);
    insertPlainText(input);

    if (_command.contains('\n')) {
        if (!_command.endsWith('\n'))
            insertPlainText("\n");
        confirmCommand();
    }

    ensureCursorVisible();
}
