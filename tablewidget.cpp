#include "tablewidget.h"
#include "utils.h"


TableWidget::TableWidget(QWidget *parent)
    : QTableWidget(parent)
{

}

QList<QStringList> TableWidget::getRows()
{
    QList<QStringList> rows;
    for(int rowIndex = 0; rowIndex < rowCount(); rowIndex++)
        rows.append(getRow(rowIndex));
    return rows;
}

QStringList TableWidget::getRow(int rowIndex)
{
    QStringList row;
    for(int columnIndex = 0; columnIndex < columnCount(); columnIndex++)
        row.append(item(rowIndex, columnIndex)->text());
    return row;
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

bool TableWidget::getPermission(QString title, QString text)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, title, text,
                                                              QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
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

QStringList TableWidget::getTitles()
{
    QStringList headers;
    for(int index = 0; index < model()->columnCount(); index++)
        headers.append(getTitle(index));
    return headers;
}

QString TableWidget::getTitle(int index)
{
    return horizontalHeaderItem(index)->text().remove(0, 3);
}

void TableWidget::addColumn()
{
    QString columnName = getColumnName("Добавление столбца",
                                       "Введите название столбца:",
                                       "Название столбца");
    if (!columnName.isEmpty())
    {
        QStringList titles = getTitles();
        titles.append(columnName);
        setTitles(titles);
    }
}

QString TableWidget::getColumnName(QString title, QString label, QString text)
{
    bool ok;
    QString columnName = QInputDialog::getText(this, title, label,
                                               QLineEdit::Normal, text, &ok);
    if(ok && !columnName.isEmpty())
        return columnName;
    return "";
}

void TableWidget::setTitles(QStringList titles)
{
    setColumnCount(titles.count());
    enumerateStringList(titles);
    setHorizontalHeaderLabels(titles);
}

void TableWidget::setRows(QList<QStringList> rows)
{
    setRowCount(0);
    for(int rowIndex = 0; rowIndex < rows.size(); rowIndex++)
    {
        setRowCount(rowCount() + 1);
        QStringList row = rows[rowIndex];
        for(int columnIndex = 0; columnIndex < row.size(); columnIndex++)
            setItem(rowIndex, columnIndex, row[columnIndex]);
    }
}

void TableWidget::setItem(int rowIndex, int columnIndex, QString textCell)
{
    if(columnIndex >= columnCount() || columnIndex < 0)
        return;
    if(rowIndex >= rowCount() || rowIndex < 0)
        return;
    QTableWidget::setItem(rowIndex, columnIndex, new QTableWidgetItem(textCell));
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

QList<QModelIndex> TableWidget::getSelectedIndexes()
{
    return selectedIndexes();
}

void TableWidget::renameColumn()
{
    QString newColumnName = getColumnName(QString("Переименование столбца %1").arg(currentColumn() + 1),
                                          QString("Новое название столбца %1").arg(currentColumn() + 1),
                                          getTitles()[currentColumn()]);
    if(!newColumnName.isEmpty())
        setTitle(currentColumn(), newColumnName);
}

void TableWidget::setTitle(int columnIndex, QString title)
{
    if(columnIndex >= columnCount() || columnIndex < 0)
        return;
    setHorizontalHeaderItem(currentColumn(), new QTableWidgetItem(enumerateString(columnIndex, title)));
}
