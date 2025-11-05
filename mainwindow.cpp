#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidgetAction>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
