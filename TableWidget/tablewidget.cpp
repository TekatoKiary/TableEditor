#include "tablewidget.h"
#include "utils.h"

TableWidget::TableWidget(QWidget *parent)
    : QTableWidget(parent)
{

}

void TableWidget::addRow()
{
    table.addRow(QStringList());
    setRowCount(rowCount() + 1);
}

QList<QStringList> TableWidget::getRows()
{
    return table.getRows();
}

void TableWidget::setRows(QList<QStringList> rows)
{
    table.setRows(rows);
    setRowCount(0);
    for(int rowIndex = 0; rowIndex < rows.size(); rowIndex++)
    {
        setRowCount(rowCount() + 1);
        for(int columnIndex = 0; columnIndex < rows[rowIndex].size(); columnIndex++)
            setItem(rowIndex, columnIndex, rows[rowIndex][columnIndex]);
    }
}

void TableWidget::setRow(QStringList row, int rowIndex)
{
    for(int columnIndex = 0; columnIndex < row.size(); columnIndex++)
        setItem(rowIndex, columnIndex, row[columnIndex]);
}

QStringList TableWidget::getTitles()
{
    return table.getTitles();
}

void TableWidget::setTitles(QStringList titles)
{
    table.setTitles(titles);
    setViewTitles(table.getTitles());
}

void TableWidget::setViewTitles(QStringList titles)
{
    setColumnCount(titles.count());
//        enumerateStringList(table.getTitles());
    setHorizontalHeaderLabels(titles);
}

void TableWidget::addColumn()
{
    QString title = getUserTitleWithDialog("Добавление столбца",
                                           "Введите название столбца:",
                                           "Название столбца");
    if (!title.isEmpty())
    {
        table.addTitle(title);
        setViewTitles(table.getTitles());
    }
}
void TableWidget::renameColumn()
{
    QString newColumnName = getUserTitleWithDialog(QString("Переименование столбца №%1").arg(currentColumn() + 1),
                                                    QString("Новое название столбца №%1").arg(currentColumn() + 1),
                                                   table.getTitle(currentColumn()));
    if(!newColumnName.isEmpty())
        table.setTitle(currentColumn(), newColumnName);
}

void TableWidget::setItem(int rowIndex, int columnIndex, QString textCell)
{
    if((columnIndex >= columnCount() || columnIndex < 0) ||
            (rowIndex >=rowCount() || rowIndex < 0))
        return;
    QTableWidget::setItem(rowIndex, columnIndex, new QTableWidgetItem(textCell));
    table.setItem(rowIndex, columnIndex, textCell);
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
        table.rebase(getOpenFileReader(parentWidget()));
        setTitles(table.getTitles());
        setRows(table.getRows());
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

QString TableWidget::getUserTitleWithDialog(QString title, QString label, QString text)
{
    bool ok;
    QString userTitle = QInputDialog::getText(parentWidget(), title, label,
                                               QLineEdit::Normal, text, &ok);
    if(ok && !userTitle.isEmpty())
        return userTitle;
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

void TableWidget::pasteRowPart(QString row, int columnIndex, int rowIndex)
{
    for(QString textCell: row.split(cellDelimiter))
        if(!textCell.isEmpty())
            setItem(rowIndex, columnIndex++, textCell);
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

//void TableWidget::combineSelectedColumns()
//{
//    QTableWidgetSelectionRange selectionRanges = selectedRanges().first();
//    for (int index = selectionRanges.rightColumn(); index > selectionRanges.leftColumn(); index--)
//        removeColumn(index);
//}

void TableWidget::tableChanged(QTableWidgetItem* item)
{
    table.setItem(item->row(), item->column(), item->text());
}
