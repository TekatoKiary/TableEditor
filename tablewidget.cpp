#include "tablewidget.h"
#include "utils.h"


TableWidget::TableWidget(QWidget *parent)
    : QTableWidget(parent)
{

}

void TableWidget::addRow()
{
    setRowCount(rowCount() + 1);
}

void TableWidget::addRow(QStringList row)
{
    int rowIndex = rowCount();
    setRowCount(rowCount() + 1);
    for(int columnIndex = 0; columnIndex < columnCount(); columnIndex++)
        setItem(rowIndex, columnIndex, row[columnIndex]);
}

QStringList TableWidget::getRow(int rowIndex)
{
    QStringList row;
    for(int columnIndex = 0; columnIndex < columnCount(); columnIndex++)
        row.append(item(rowIndex, columnIndex)->text());
    return row;
}

QList<QStringList> TableWidget::getRows()
{
    QList<QStringList> rows;
    for(int rowIndex = 0; rowIndex < rowCount(); rowIndex++)
        rows.append(getRow(rowIndex));
    return rows;
}

void TableWidget::setRow(QStringList row, int rowIndex)
{
    for(int columnIndex = 0; columnIndex < row.size(); columnIndex++)
        setItem(rowIndex, columnIndex, row[columnIndex]);
}

void TableWidget::setRows(QList<QStringList> rows)
{
    setRowCount(0);
    for(int rowIndex = 0; rowIndex < rows.size(); rowIndex++)
    {
        setRowCount(rowCount() + 1);
        setRow(rows[rowIndex], rowIndex);
    }
}

void TableWidget::addTitle(QString columnName)
{
    QStringList titles = getTitles();
    titles.append(columnName);
    setTitles(titles);
}

QString TableWidget::getTitle(int index)
{
    return horizontalHeaderItem(index)->text().remove(0, 3);
}

QStringList TableWidget::getTitles()
{
    QStringList headers;
    for(int index = 0; index < model()->columnCount(); index++)
        headers.append(getTitle(index));
    return headers;
}

void TableWidget::setTitle(int columnIndex, QString title)
{
    if(columnIndex >= columnCount() || columnIndex < 0)
        return;
    setHorizontalHeaderItem(currentColumn(), new QTableWidgetItem(enumerateString(columnIndex, title)));
}

void TableWidget::setTitles(QStringList titles)
{
    setColumnCount(titles.count());
    enumerateStringList(titles);
    setHorizontalHeaderLabels(titles);
}

void TableWidget::addColumn()
{
    QString columnName = getColumnNameWithDialog("Добавление столбца",
                                                 "Введите название столбца:",
                                                 "Название столбца");
    if (!columnName.isEmpty())
        addTitle(columnName);
}

void TableWidget::renameColumn()
{
    QString newColumnName = getColumnNameWithDialog(QString("Переименование столбца %1").arg(currentColumn() + 1),
                                                    QString("Новое название столбца %1").arg(currentColumn() + 1),
                                                    getTitles()[currentColumn()]);
    if(!newColumnName.isEmpty())
        setTitle(currentColumn(), newColumnName);
}

void TableWidget::setItem(int rowIndex, int columnIndex, QString textCell)
{
    if((columnIndex >= columnCount() || columnIndex < 0) &&
            (rowIndex >=rowCount() || rowIndex < 0))
        return;
    QTableWidget::setItem(rowIndex, columnIndex, new QTableWidgetItem(textCell));
}

void TableWidget::copy()
{
    if(!selectedItems().isEmpty())
        setClipboard(takePart(selectedRanges().first()));
}

void TableWidget::cut()
{
    if(!selectedIndexes().isEmpty())
        setClipboard(takePart(selectedRanges().first(), true));
}

void TableWidget::paste()
{
    if(selectedIndexes().isEmpty())
        return;
    std::stringstream stream(QGuiApplication::clipboard()->text().toStdString());
    std::string row;
    int rowIndex = currentRow();
    while(getline(stream, row, rowDelimiter.toLatin1()))
        pasteRowPart(QString::fromStdString(row), currentColumn(), rowIndex++);
}

void TableWidget::rebase()
{
    try
    {
        CsvFileReader* rebasingFileReader = getOpenFileReader(parentWidget());
        QStringList combiningTitles = getTitles() + rebasingFileReader->getTitles();
        combiningTitles.removeDuplicates();
        setTitles(combiningTitles);
        addRowsFromRebasingFile(rebasingFileReader);
    }
    catch (domain_error) {}
}

QString TableWidget::takePart(QTableWidgetSelectionRange range, bool is_removed)
{
    QStringList copiedTablePart;
    for (int rowIndex = range.topRow(); rowIndex <= range.bottomRow(); rowIndex++)
        copiedTablePart.append(takeRowPart(rowIndex, range.leftColumn(), range.rightColumn(), is_removed));
    return copiedTablePart.join(rowDelimiter);
}

QString TableWidget::takeRowPart(int rowIndex, int startColumnIndex, int endColumnIndex, bool is_removed)
{
    QStringList copiedRow;
    for (int columnIndex = startColumnIndex; columnIndex <= endColumnIndex; columnIndex++)
    {
        copiedRow.append(item(rowIndex, columnIndex)->text());
        if(is_removed)
            setItem(rowIndex, columnIndex, "");
    }
    return copiedRow.join(cellDelimiter);
}

void TableWidget::addRowsFromRebasingFile(CsvFileReader* rebasingFileReader)
{
    for(QStringList row: rebasingFileReader->getRows())
        addRow(createNewRowBasedOnRebasingOne(row, rebasingFileReader->getTitles()));
}

QStringList TableWidget::createNewRowBasedOnRebasingOne(QStringList rebasingRow, QStringList rebasingTitles)
{
    QStringList newRow;
    for(QString title : getTitles())
        if(rebasingTitles.contains(title))
            newRow.append(rebasingRow[rebasingTitles.indexOf(title)]);
        else
            newRow.append("");
    return newRow;
}

QString TableWidget::getColumnNameWithDialog(QString title, QString label, QString text)
{
    bool ok;
    QString columnName = QInputDialog::getText(parentWidget(), title, label,
                                               QLineEdit::Normal, text, &ok);
    if(ok && !columnName.isEmpty())
        return columnName;
    return "";
}

bool TableWidget::getPermission(QString title, QString text)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, title, text,
                                                              QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
}

QList<QModelIndex> TableWidget::getSelectedIndexes()
{
    return selectedIndexes();
}

void TableWidget::pasteRowPart(QString row, int startColumnIndex, int rowIndex)
{
    for(QString textCell: row.split(cellDelimiter))
        if(!textCell.isEmpty())
            setItem(rowIndex, startColumnIndex++, textCell);
    rowIndex++;
}

void TableWidget::removeSelectedColumns()
{
    if (selectedIndexes().isEmpty())
        return;
    if (getPermission("Удаление столбца", "Вы уверены?"))
    {
        QTableWidgetSelectionRange selectionRanges = selectedRanges().first();
        for (int index = selectionRanges.rightColumn(); index >= selectionRanges.leftColumn(); index--)
            removeColumn(index);
    }
}

void TableWidget::removeSelectedRows()
{
    if (!selectedIndexes().isEmpty())
    {
        QTableWidgetSelectionRange selectionRanges = selectedRanges().first();
        for (int index = selectionRanges.bottomRow(); index >= selectionRanges.topRow(); index--)
            removeRow(index);
    }
}
