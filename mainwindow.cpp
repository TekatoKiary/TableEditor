#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileManager/csvfilereader.h"
#include "FileManager/csvfilewriter.h"

#include <QFileDialog>
#include <QList>

using namespace std;


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

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Открыть файл",
                QDir::homePath(),
                "Csv files (*.csv)"
                );

    CsvFileReader fileReader(fileName.toStdString());
    CsvFileWriter fileWriter("test.csv");
    fileWriter.write(fileReader.getTitles());
    for(QList<string> element : fileReader.getElements())
        fileWriter.write(element);

}
