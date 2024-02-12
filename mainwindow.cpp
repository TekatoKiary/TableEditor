#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileManager/csvfilereader.h"
#include "FileManager/csvfilewriter.h"
#include "utils.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(sizeWindow);
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setCurrentFileName(filePath);

    connect(ui->tableWidget, &TableWidget::itemSelectionChanged, this, [this](){
        setEnabledAction(ui->combineColumnsAction, ui->tableWidget->getSelectedIndexes().length() >= 2);
        setEnabledAction(ui->moveColumnAction, !ui->tableWidget->getSelectedIndexes().isEmpty());
        setEnabledAction(ui->renameColumnAction, !ui->tableWidget->getSelectedIndexes().isEmpty());
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    try
    {
        setFilePath(getOpenFilePath(this));
        CsvFileReader fileReader(filePath);
        loadTable(fileReader.getTitles(), fileReader.getRows());
    }
    catch (domain_error) {}
}

void MainWindow::setFilePath(QString newFilePath)
{
    if(newFilePath.isEmpty() || newFilePath.isNull())
        throw domain_error("Передаваемый параметр пуст или равен Null");
    filePath = newFilePath;
    setCurrentFileName(getFileNameFromAbsolutePath(filePath));
}

void MainWindow::loadTable(QStringList titles, QList<QStringList> rows)
{
    ui->tableWidget->setTitles(titles);
    ui->tableWidget->setRows(rows);
}

void MainWindow::saveFile()
{
    CsvFileWriter fileWriter(filePath);
    fileWriter.write(ui->tableWidget->getTitles());
    fileWriter.write(ui->tableWidget->getRows());
    ui->statusbar->showMessage("Файл сохранен");
}

void MainWindow::setCurrentFileName(QString currentFileName)
{
    ui->currentFileNameLabel->setText(currentFileName);
}

void MainWindow::saveAsFile()
{
    try
    {
        setFilePath(getSaveFilePath(this));
        saveFile();
    }
    catch (domain_error) {}
}

void MainWindow::setEnabledAction(QAction* action, bool isEnabled)
{
    action->setEnabled(isEnabled);
}
