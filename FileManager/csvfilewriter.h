#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QList>
#include <fstream>

using namespace std;


class CsvFileWriter
{

public:
    CsvFileWriter(QString fileName);
    ~CsvFileWriter();

    void write(QStringList element);
    void write(QList<QList<QString> > elements);

private:
    QString fileName;
    char delimiter =',';
    ofstream file;

    string joinElement(QStringList element);
};

#endif // CSVFILEWRITER_H
