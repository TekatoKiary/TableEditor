#include "csvfilereader.h"

using namespace std;


CsvFileReader::CsvFileReader(string fileName)
{
    this->fileName = fileName;
    readFile();
}

string CsvFileReader::getFileName()
{
    return fileName;
}

QList<string> CsvFileReader::getTitles()
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

QList<QList<string>> CsvFileReader::getElements()
{
    return elements;
}

void CsvFileReader::readFile()
{
    ifstream file(fileName);
    readTitles(&file);
    readElements(&file);
    file.close();
}

void CsvFileReader::readTitles(ifstream *file)
{
    std::string line, nameTitle;
    getline(*file, line);
    stringstream stream(line);
    while(getline(stream, nameTitle, delimiter))
        titles.append(nameTitle);
}

void CsvFileReader::readElements(ifstream *file)
{
    string element;
    while(getline(*file, element))
        elements.append(splitElement(element));
}

QList<string> CsvFileReader::splitElement(string element)
{
    QList<string> dividedElement;
    string cell;
    stringstream stream(element);
    while(getline(stream, cell, delimiter))
        dividedElement.append(cell);
    return dividedElement;
}
