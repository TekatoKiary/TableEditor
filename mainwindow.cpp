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
    setTitles(titles);
    setRows(rows);
}

void MainWindow::setTitles(QStringList titles)
{
    ui->tableWidget->setColumnCount(titles.count());
    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

void MainWindow::setRows(QList<QStringList> rows)
{
    ui->tableWidget->setRowCount(0);
    for(int rowIndex = 0; rowIndex < rows.size(); rowIndex++)
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        QStringList row = rows[rowIndex];
        for(int columnIndex = 0; columnIndex < row.size(); columnIndex++)
            setCell(rowIndex, columnIndex, row[columnIndex]);
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

void MainWindow::addRow()
{
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
        setCell(rowIndex, columnIndex, "");
}

void MainWindow::addRow(QStringList row)
{
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
        setCell(rowIndex, columnIndex, row[columnIndex]);
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
    fileWriter.write(getRows());
    ui->statusbar->showMessage("Файл сохранен");
}

QList<QStringList> MainWindow::getRows()
{
    QList<QStringList> rows;
    for(int rowIndex = 0; rowIndex < ui->tableWidget->rowCount(); rowIndex++)
        rows.append(getRow(rowIndex));
    return rows;
}

QStringList MainWindow::getRow(int rowIndex)
{
    QStringList row;
    for(int columnIndex = 0; columnIndex < ui->tableWidget->columnCount(); columnIndex++)
        row.append(getCell(rowIndex, columnIndex));
    return row;
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
        QTableWidgetSelectionRange selectedRanges = ui->tableWidget->selectedRanges().first();
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

void MainWindow::deleteRow()
{
    if (!ui->tableWidget->selectedItems().isEmpty())
    {
        QTableWidgetSelectionRange selectedRanges = ui->tableWidget->selectedRanges().first();
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
        fileWriter.write(getRows());
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
        addRow(createNewRowBasedOnRebasingOne(row, rebasingFileReader.getTitles()));
}

QStringList MainWindow::createNewRowBasedOnRebasingOne(QStringList rebasingRow, QStringList rebasingTitles)
{
    QStringList newRow;
    for(QString title : getTitles())
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
            setCell(rowIndex, columnIndex++, textCell);
        rowIndex++;
    }
}

void MainWindow::setCell(int rowIndex, int columnIndex, QString textCell)
{
    if(columnIndex >= ui->tableWidget->columnCount() || columnIndex < 0)
        return;
    if(rowIndex >= ui->tableWidget->rowCount() || rowIndex < 0)
        return;
    ui->tableWidget->setItem(rowIndex, columnIndex, new QTableWidgetItem(textCell));
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
        copiedRow.append(getCell(rowIndex, columnIndex));
        if(is_removed)
            setCell(rowIndex, columnIndex, "");
    }
    return copiedRow.join(cellDelimiter);
}
