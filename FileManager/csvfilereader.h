#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <fstream>
#include <sstream>
#include <QList>

using namespace std;


class CsvFileReader
{

public:
    CsvFileReader(QString fileName);

    QString getFileName();

    QStringList getTitles();

    int getColumnCount();

    int getRowsCount();

    QList<QList<QString>> getRows();

private:
    QStringList titles;
    QString fileName;
    QList<QStringList> rows;
    QChar delimiter = ',';

    void readFile();

    void readTitles(ifstream *file);

    void readRows(ifstream *file);

};

#endif // CSVFILEREADER_H
