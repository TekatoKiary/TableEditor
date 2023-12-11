#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileManager/csvfilereader.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(sizeWindow);
    ui->setupUi(this);
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

void MainWindow::loadTable(QStringList titles, QList<QList<QString>> elements)
{
    setTitles(titles);
    setElements(elements);
}

void MainWindow::setTitles(QStringList titles)
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
        QStringList element = elements[rowIndex];
        for(int columnIndex = 0; columnIndex < element.size(); columnIndex++)
            ui->tableWidget->setItem(rowIndex, columnIndex, new QTableWidgetItem(element[columnIndex]));
    }
}

void MainWindow::addColumn()
{
    bool ok;
    QString columnName = QInputDialog::getText(
                this, "Добавление столбца",
                "Введите название столбца:",
                QLineEdit::Normal, "Название столбца", &ok
                );
    if (ok && !columnName.isEmpty())
    {
        QStringList titles = getTitles();
        titles.append(columnName);
        setTitles(titles);
    }
}

void MainWindow::addElement()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
}


QStringList MainWindow::getTitles()
{
    QStringList headers;
    for(int index = 0; index < ui->tableWidget->model()->columnCount(); index++)
        headers.append(ui->tableWidget->model()->headerData(index, Qt::Horizontal).toString());
    return headers;
}
