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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    try
    {
        setFilePath(getOpenFilePath());
        CsvFileReader fileReader(filePath);
        loadTable(fileReader.getTitles(), fileReader.getElements());
    }
    catch (domain_error) {}
}

QString MainWindow::getOpenFilePath()
{
    return QFileDialog::getOpenFileName(
                this,
                "Открыть файл",
                QDir::homePath(),
                "Csv files (*.csv)"
                );
}

void MainWindow::setFilePath(QString newFilePath)
{
    if(newFilePath.isEmpty() || newFilePath.isNull())
        throw domain_error("Передаваемый параметр пуст или равен Null");
    filePath = newFilePath;
    setCurrentFileName(getFileNameFromAbsolutePath(filePath));
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
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
        ui->tableWidget->setItem(rowIndex, columnIndex, new QTableWidgetItem);
}

void MainWindow::addElement(QStringList element)
{
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
        ui->tableWidget->setItem(rowIndex, columnIndex, new QTableWidgetItem(element[columnIndex]));
}


QStringList MainWindow::getTitles()
{
    QStringList headers;
    for(int index = 0; index < ui->tableWidget->model()->columnCount(); index++)
        headers.append(ui->tableWidget->model()->headerData(index, Qt::Horizontal).toString());
    return headers;
}

void MainWindow::saveFile()
{
    CsvFileWriter fileWriter(filePath);
    fileWriter.write(getTitles());
    fileWriter.write(getElements());
    ui->statusbar->showMessage("Файл сохранен");
}

QList<QList<QString>> MainWindow::getElements()
{
    QList<QList<QString>> elements;
    for(int rowIndex = 0; rowIndex < ui->tableWidget->rowCount(); rowIndex++)
    {
        QList<QString> element;
        for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
            element.append(getCell(rowIndex, columnIndex));
        elements.append(element);
    }
    return elements;
}

void MainWindow::setCurrentFileName(QString currentFileName)
{
    ui->currentFileNameLabel->setText(currentFileName);
}

QString MainWindow::getCell(int rowIndex, int columnIndex)
{
    QString cell = ui->tableWidget->item(rowIndex, columnIndex)->text();
    if (cell.isNull())
        return "";
    return cell;
}

void MainWindow::deleteColumn()
{
    if (ui->tableWidget->selectedItems().isEmpty())
        return;
    if (getPermission("Удаление столбца","Вы уверены?"))
    {
        QTableWidgetSelectionRange selectedRanges = ui->tableWidget->selectedRanges()[0];
        for (int index = selectedRanges.rightColumn(); index >= selectedRanges.leftColumn(); index--)
            ui->tableWidget->removeColumn(index);
    }
}

bool MainWindow::getPermission(QString title, QString text)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, title, text,
                                                              QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
}

void MainWindow::deleteElement()
{
    if (!ui->tableWidget->selectedItems().isEmpty())
    {
        QTableWidgetSelectionRange selectedRanges = ui->tableWidget->selectedRanges()[0];
        for (int index = selectedRanges.bottomRow(); index >= selectedRanges.topRow(); index--)
            ui->tableWidget->removeRow(index);
    }
}

void MainWindow::saveAsFile()
{
    try
    {
        setFilePath(getSaveFilePath());
        CsvFileWriter fileWriter(filePath);
        fileWriter.write(getTitles());
        fileWriter.write(getElements());
        ui->statusbar->showMessage("Файл сохранен");
    }
    catch (domain_error) {}
}

QString MainWindow::getSaveFilePath()
{
    return QFileDialog::getSaveFileName(
                this,
                "Сохранить файл",
                QDir::homePath(),
                "Csv files (*.csv)"
                );
}

void MainWindow::rebaseTable()
{
    try
    {
        CsvFileReader rebasingFileReader = getRebasingFileReader();
        QStringList combiningTitles = getTitles() + rebasingFileReader.getTitles();
        combiningTitles.removeDuplicates();
        setTitles(combiningTitles);
        addElementsFromRebasingFile(rebasingFileReader);
    }
    catch (domain_error) {}
}

CsvFileReader MainWindow::getRebasingFileReader()
{
    QString rebasingFilePath = getOpenFilePath();
    if(rebasingFilePath.isEmpty() || rebasingFilePath.isNull())
        throw domain_error("Передаваемый параметр пуст или равен Null");
    return CsvFileReader(rebasingFilePath);
}

void MainWindow::addElementsFromRebasingFile(CsvFileReader rebasingFileReader)
{
    for(QStringList element: rebasingFileReader.getElements())
        createNewElementBasedOnRebasingOne(element, rebasingFileReader.getTitles());
}

QStringList MainWindow::createNewElementBasedOnRebasingOne(QStringList rebasingElement, QStringList rebasingTitles)
{
    QStringList newElement;
    for(QString title : getTitles())
        if(rebasingTitles.contains(title))
            newElement.append(rebasingElement[rebasingTitles.indexOf(title)]);
        else
            newElement.append("");
    addElement(newElement);
    return newElement;
}
