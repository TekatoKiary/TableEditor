#include "table.h"

Table::Table()
{

}

void Table::addRow(QStringList row)
{
    for(int index = 0; index < titles.count(); index++)
        row.append("");
    rows.append(row);
}

QStringList Table::getRow(int index)
{
    return rows[index];
}

QList<QStringList> Table::getRows()
{
    return rows;
}

void Table::setRow(QStringList row, int rowIndex)
{
    rows[rowIndex] = row;
}

void Table::setRows(QList<QStringList> rows)
{
    this->rows = rows;
}

void Table::addTitle(QString title)
{
    titles.append(title);
    for(int index = 0; index < rows.count(); index++)
        rows[index].append("");
}

QString Table::getTitle(int index)
{
    return titles[index];
}

QStringList Table::getTitles()
{
    return titles;
}

void Table::setTitle(int index, QString title)
{
    if(index >= titles.count() || index < 0)
        return;
    titles.append(title);
}

void Table::setTitles(QStringList titles)
{
    this->titles = titles;
}

void Table::setItem(int rowIndex, int columnIndex, QString text)
{
    if((columnIndex >= titles.count() || columnIndex < 0) ||
            (rowIndex >= rows.count() || rowIndex < 0))
        return;
    rows[rowIndex][columnIndex] = text;
}

void Table::rebase(CsvFileReader* rebasingFileReader)
{
    QStringList combiningTitles = getTitles() + rebasingFileReader->getTitles();
    combiningTitles.removeDuplicates();
    setTitles(combiningTitles);
    addRowsFromRebasingFile(rebasingFileReader);
}

void Table::addRowsFromRebasingFile(CsvFileReader* rebasingFileReader)
{
    for(QStringList row: rebasingFileReader->getRows())
        addRow(createNewRowBasedOnRebasingOne(row, rebasingFileReader->getTitles()));
}

QStringList Table::createNewRowBasedOnRebasingOne(QStringList rebasingRow, QStringList rebasingTitles)
{
    QStringList newRow;
    for(QString title : titles)
        if(rebasingTitles.contains(title))
            newRow.append(rebasingRow[rebasingTitles.indexOf(title)]);
        else
            newRow.append("");
    return newRow;
}
