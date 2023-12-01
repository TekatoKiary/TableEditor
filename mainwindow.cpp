#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(sizeWindow);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

