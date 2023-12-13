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
    file << joinElement(element) << endl;
}

void CsvFileWriter::write(QList<QList<QString>> elements)
{
    for (int index = 0; index < elements.size(); index++)
        file << joinElement(elements[index]) << endl;
}

string CsvFileWriter::joinElement(QStringList element)
{
    string joinedElement;
    for (int index = 0; index < element.size() - 1; index++)
        joinedElement += element[index].toStdString() + string(1, delimiter);
    joinedElement += element[element.size() - 1].toStdString();
    return joinedElement;
}
