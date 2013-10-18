#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QDockWidget>
#include <QPlainTextEdit>
#include <QGraphicsScene>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>

#include "Ui/Grids/HorizontalGrid.h"
#include "Ui/Grids/VerticalGrid.h"
#include "Ui/Items/ImageItem.h"
#include "Ui/Items/TextItem.h"
#include "Ui/console.h"

#include "Data/interpreter.h"

////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    UI(new Ui::MainWindow)
{
    UI->setupUi(this);

    createScene();
    createDockWidgets();

//    testCanvas();
}

////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete UI;
}

////////////////////////////////////////////////////////////////
void MainWindow::createScene()
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    UI->graphicsView->setScene(scene);
}

////////////////////////////////////////////////////////////////
void MainWindow::createDockWidgets()
{
    Console* console = new Console(this);
    console->nextCommand();

    connect(console, SIGNAL(executeCommand(QByteArray)),
            Interpreter::getInstance(), SLOT(makeUserCall(const QByteArray&)));

    connect(Interpreter::getInstance(), SIGNAL(emitError(const QByteArray&)),
            console, SLOT(writeError(const QByteArray&)));

    connect(Interpreter::getInstance(), SIGNAL(emitOutput(QByteArray)),
            console, SLOT(writeOutput(QByteArray)));

    connect(Interpreter::getInstance(), SIGNAL(workDone()),
            console, SLOT(nextCommand()));

    connect(console, SIGNAL(terminateCommand()),
            Interpreter::getInstance(), SLOT(stopWork()));

    // NOTE ak chceme, aby konzola necakala na dokoncenie prikazu, odkomentujeme
    connect(console, SIGNAL(executeCommand(QByteArray)),
            console, SLOT(waitForCommand()));


    QDockWidget *dock = new QDockWidget(tr("Console"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(console);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

////////////////////////////////////////////////////////////////
void MainWindow::testCanvas()
{
    QGraphicsScene& scene = *UI->graphicsView->scene();
//    BaseGrid* grid = new HorizontalGrid();

    QGraphicsLinearLayout* grid = new QGraphicsLinearLayout(Qt::Vertical);
//    grid->setSpacing(0);

    QGraphicsWidget* container = new QGraphicsWidget;
    container->setLayout(grid);
    container->resize(QSizeF(100,100));

    BaseItem* item;

    item = new ImageItem("D:\\cog.png");
    grid->addItem(item);
    scene.addItem(item);

    for (int n = 0; n < 10; ++n) {
        QGraphicsLinearLayout* subGrid = new QGraphicsLinearLayout(Qt::Horizontal);
//        subGrid->setSpacing(0);

        item = new TextItem("Aaaa bbb");
        subGrid->addItem(item);
        scene.addItem(item);

        item = new TextItem("byby\nbyby");
        subGrid->addItem(item);
        scene.addItem(item);

        item = new TextItem("Aaaa");
        subGrid->addItem(item);
        scene.addItem(item);

        item = new TextItem("byby");
        subGrid->addItem(item);
        scene.addItem(item);

        item = new TextItem("vvv");
        subGrid->addItem(item);
        scene.addItem(item);

        item = new TextItem("h j k l");
        subGrid->addItem(item);
        scene.addItem(item);

        grid->addItem(subGrid);
    }

    scene.addItem(container);
}
