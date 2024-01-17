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
        this->ui->renameColumnAction->setEnabled(!this->ui->tableWidget->getSelectedIndexes().isEmpty());
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
        setFilePath(getOpenFilePath());
        CsvFileReader fileReader(filePath);
        loadTable(fileReader.getTitles(), fileReader.getRows());
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
        setFilePath(getSaveFilePath());
        CsvFileWriter fileWriter(filePath);
        fileWriter.write(ui->tableWidget->getTitles());
        fileWriter.write(ui->tableWidget->getRows());
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
        QStringList combiningTitles = ui->tableWidget->getTitles() + rebasingFileReader.getTitles();
        combiningTitles.removeDuplicates();
        ui->tableWidget->setTitles(combiningTitles);
        addRowsFromRebasingFile(rebasingFileReader);
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

void MainWindow::addRowsFromRebasingFile(CsvFileReader rebasingFileReader)
{
    for(QStringList row: rebasingFileReader.getRows())
        ui->tableWidget->addRow(createNewRowBasedOnRebasingOne(row, rebasingFileReader.getTitles()));
}

QStringList MainWindow::createNewRowBasedOnRebasingOne(QStringList rebasingRow, QStringList rebasingTitles)
{
    QStringList newRow;
    for(QString title : ui->tableWidget->getTitles())
        if(rebasingTitles.contains(title))
            newRow.append(rebasingRow[rebasingTitles.indexOf(title)]);
        else
            newRow.append("");
    return newRow;
}

void MainWindow::copyTablePart()
{
    if(!ui->tableWidget->selectedItems().isEmpty())
        setClipboard(takeTablePart(ui->tableWidget->selectedRanges().first()));
}

void MainWindow::setClipboard(QString text)
{
    QClipboard *board = QGuiApplication::clipboard();
    board->setText(text);
}

void MainWindow::pasteTablePart()
{
    if(ui->tableWidget->selectedItems().isEmpty())
        return;
    stringstream stream(QGuiApplication::clipboard()->text().toStdString());
    string row;
    int rowIndex = ui->tableWidget->currentRow();
    int startColumnIndex = ui->tableWidget->currentColumn();
    while(getline(stream, row, rowDelimiter.toLatin1()))
    {
        int columnIndex = startColumnIndex;
        for(QString textCell: QString::fromStdString(row).split(cellDelimiter))
            ui->tableWidget->setItem(rowIndex, columnIndex++, textCell);
        rowIndex++;
    }
}

void MainWindow::cutTablePart()
{
    if(!ui->tableWidget->selectedItems().isEmpty())
        setClipboard(takeTablePart(ui->tableWidget->selectedRanges().first(), true));
}

QString MainWindow::takeTablePart(QTableWidgetSelectionRange range, bool is_removed)
{
    QStringList copiedTablePart;
    for (int rowIndex = range.topRow(); rowIndex <= range.bottomRow(); rowIndex++)
        copiedTablePart.append(takeRow(rowIndex, range.leftColumn(), range.rightColumn(), is_removed));
    return copiedTablePart.join(rowDelimiter);
}

QString MainWindow::takeRow(int rowIndex, int startColumnIndex, int endColumnIndex, bool is_removed)
{
    QStringList copiedRow;
    for (int columnIndex = startColumnIndex; columnIndex <= endColumnIndex; columnIndex++)
    {
        copiedRow.append(ui->tableWidget->item(rowIndex, columnIndex)->text());
        if(is_removed)
            ui->tableWidget->setItem(rowIndex, columnIndex, "");
    }
    return copiedRow.join(cellDelimiter);
}

