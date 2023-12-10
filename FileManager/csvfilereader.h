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

    QList<QString> getTitles();

    int getColumnCount();

    int getElementsCount();

    QList<QList<QString>> getElements();

private:
    QList<QString> titles;
    QString fileName;
    QList<QList<QString>> elements;
    char delimiter = ',';

    void readFile();

    void readTitles(ifstream *file);

    void readElements(ifstream *file);

    QList<QString> splitElement(QString element);

};

#endif // CSVFILEREADER_H
