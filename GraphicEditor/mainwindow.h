#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class TextFile;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void reparsePlainTextEdit();
    void reloadCanvas();

private:
    Ui::MainWindow *UI;

    TextFile* _textEditFile;

    void createDockWidgets();
    void createScene();
    void createConnections();

    void testCanvas();
    void testAllocator();
    void testFileManager();
};

#endif // MAINWINDOW_H
