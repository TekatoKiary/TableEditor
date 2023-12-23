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

    void write(QStringList row);
    void write(QList<QStringList> rows);

private:
    QString fileName;
    QChar delimiter =',';
    ofstream file;

    string joinRow(QStringList row);
};

#endif // CSVFILEWRITER_H
