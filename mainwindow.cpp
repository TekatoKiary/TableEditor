#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileManager/csvfilereader.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(sizeWindow);
    ui->setupUi(this);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

    CsvFileReader fileReader(fileName);
    loadTable(fileReader.getTitles(), fileReader.getElements());
}

void MainWindow::loadTable(QList<QString> titles, QList<QList<QString>> elements)
{
    setTitles(titles);
    setElements(elements);
}

void MainWindow::setTitles(QList<QString> titles)
{
    ui->tableWidget->setColumnCount(titles.count());
    ui->tableWidget->setHorizontalHeaderLabels(titles);
    for(int index = 0; index < titles.size(); index++)
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(index, QHeaderView::Stretch);
}

void MainWindow::setElements(QList<QList<QString>> elements)
{
    ui->tableWidget->setRowCount(0);
    for(int rowIndex = 0; rowIndex < elements.size(); rowIndex++)
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        QList<QString> element = elements[rowIndex];
        for(int columnIndex = 0; columnIndex < element.size(); columnIndex++)
            ui->tableWidget->setItem(rowIndex, columnIndex, new QTableWidgetItem(element[columnIndex]));
    }
}
