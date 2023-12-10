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

QList<QString> CsvFileReader::getTitles()
{
    return titles;
}

int CsvFileReader::getColumnCount()
{
    return titles.count();
}

int CsvFileReader::getElementsCount()
{
    return elements.count();
}

QList<QList<QString>> CsvFileReader::getElements()
{
    return elements;
}

void CsvFileReader::readFile()
{
    ifstream file(fileName.toStdString());
    readTitles(&file);
    readElements(&file);
    file.close();
}

void CsvFileReader::readTitles(ifstream *file)
{
    string line, nameTitle;
    getline(*file, line);
    stringstream stream(line);
    while(getline(stream, nameTitle, delimiter))
        titles.append(QString::fromStdString(nameTitle));
}

void CsvFileReader::readElements(ifstream *file)
{
    string element;
    while(getline(*file, element))
        elements.append(splitElement(QString::fromStdString(element)));
}

QList<QString> CsvFileReader::splitElement(QString element)
{
    QList<QString> dividedElement;
    string cell;
    stringstream stream(element.toStdString());
    while(getline(stream, cell, delimiter))
        dividedElement.append(QString::fromStdString(cell));
    return dividedElement;
}
