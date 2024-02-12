#ifndef TABLE_H
#define TABLE_H

#include <FileManager/csvfilereader.h>

#include <QStringList>

class Table
{
public:
    Table();

    void addRow(QStringList row);
    QStringList getRow(int rowIndex);
    QList<QStringList> getRows();
    void setRow(QStringList row, int rowIndex);
    void setRows(QList<QStringList> rows);

    void addTitle(QString title);
    QString getTitle(int index);
    QStringList getTitles();
    void setTitle(int index, QString title);
    void setTitles(QStringList titles);

    void setItem(int rowIndex, int columnIndex, QString text);

    void rebase(CsvFileReader *rebasingFileReader);
private:
    QStringList titles;
    QList<QStringList> rows;

    void addRowsFromRebasingFile(CsvFileReader *rebasingFileReader);
    QStringList createNewRowBasedOnRebasingOne(QStringList rebasingRow, QStringList rebasingTitles);
};

#endif // TABLE_H
