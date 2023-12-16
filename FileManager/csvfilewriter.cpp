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

void CsvFileWriter::write(QStringList element)
{
    file << element.join(delimiter).toStdString() << endl;
}

void CsvFileWriter::write(QList<QList<QString>> elements)
{
    for (int index = 0; index < elements.size(); index++)
        file << elements[index].join(delimiter).toStdString() << endl;
}
