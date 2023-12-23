#include "csvfilewriter.h"


CsvFileWriter::CsvFileWriter(QString fileName)
{
    this->fileName = fileName;
    file.open(fileName.toStdString());
}

CsvFileWriter::~CsvFileWriter()
{
    file.close();
}

void CsvFileWriter::write(QStringList row)
{
    file << row.join(delimiter).toStdString() << endl;
}

void CsvFileWriter::write(QList<QStringList> rows)
{
    for (int index = 0; index < rows.size(); index++)
        file << rows[index].join(delimiter).toStdString() << endl;
}
