#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <fstream>
#include <sstream>
#include <QList>

using namespace std;

class CsvFileReader
{

public:
    CsvFileReader(string fileName);

    string getFileName();

    QList<string> getTitles();

    int getColumnCount();

    int getElementsCount();

    QList<QList<string>> getElements();

private:
    QList<string> titles;
    string fileName;
    QList<QList<string>> elements;
    char delimiter = ',';

    void readFile();

    void readTitles(ifstream *file);

    void readElements(ifstream *file);

    QList<string> splitElement(string element);

};

#endif // CSVFILEREADER_H
