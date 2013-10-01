#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QPlainTextEdit>

#include "Ui/console.h"

#include "Data/interpreter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createDockWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
