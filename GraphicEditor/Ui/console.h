#pragma once

#include <QTextEdit>
#include <QBuffer>
#include <QKeyEvent>

/* TODO pridat znak kde piseme do konzoly
 * TODO sipkou hore pridame posledny prikaz
 *
 */
class Console : public QTextEdit
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = 0);

signals:
    /// Emits command that user wants to run
    void executeCommand(const QByteArray& command);

    /// Emits that user wants to terminate process
    void terminateCommand();

public slots:

    /// Prepared console for the user input
    void nextCommand();

    /// Sets console to working state
    void waitForCommand();

    void clear();

    void writeOutput(const QByteArray& data);
    void writeError (const QByteArray& data);
    void writeSystem(const QString& message);

protected:
    void keyPressEvent ( QKeyEvent * e );

private:
    const static QColor InputColor;
    const static QColor OutputColor;
    const static QColor ErrorColor;
    const static QColor SystemColor;

    bool _somethingWritten;

    QString _command;
    QList<QString> _commandHistory;
    int _historyIndex;

    void confirmCommand();
    void writeCommandSign();
    void pasteInput(const QString& input, int commandCursorPosition);
};
