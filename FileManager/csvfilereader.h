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

    int getElementsCount();

    QList<QList<QString>> getElements();

private:
    QStringList titles;
    QString fileName;
    QList<QList<QString>> elements;
    char delimiter = ',';

    void readFile();

    void readTitles(ifstream *file);

    void readElements(ifstream *file);

    QStringList splitElement(QString element);

};

#endif // CSVFILEREADER_H
