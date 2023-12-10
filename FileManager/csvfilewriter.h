#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QList>
#include <fstream>

using namespace std;

class CsvFileWriter
{

public:
    CsvFileWriter(string fileName);
    ~CsvFileWriter();

    void write(QList<string> element);

private:
    string fileName;
    char delimiter =',';
    ofstream file;

    string joinElement(QList<string> element);
};

#endif // CSVFILEWRITER_H
