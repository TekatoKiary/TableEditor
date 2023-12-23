#include "csvfilereader.h"


CsvFileReader::CsvFileReader(QString fileName)
{
    this->fileName = fileName;
    readFile();
}

QString CsvFileReader::getFileName()
{
    return fileName;
}

QStringList CsvFileReader::getTitles()
{
    return titles;
}

int CsvFileReader::getColumnCount()
{
    return titles.count();
}

int CsvFileReader::getRowsCount()
{
    return rows.count();
}

QList<QStringList> CsvFileReader::getRows()
{
    return rows;
}

void CsvFileReader::readFile()
{
    ifstream file(fileName.toStdString());
    readTitles(&file);
    readRows(&file);
    file.close();
}

void CsvFileReader::readTitles(ifstream *file)
{
    string line, nameTitle;
    getline(*file, line);
    stringstream stream(line);
    while(getline(stream, nameTitle, delimiter.toLatin1()))
        titles.append(QString::fromStdString(nameTitle));
}

void CsvFileReader::readRows(ifstream *file)
{
    string row;
    while(getline(*file, row))
        rows.append(QString::fromStdString(row).split(delimiter));
}
