#include "csvfilewriter.h"

#include <fstream>

using namespace std;

CsvFileWriter::CsvFileWriter(string fileName)
{
    this->fileName = fileName;
    file.open(fileName);
}

CsvFileWriter::~CsvFileWriter()
{
    file.close();
}

void CsvFileWriter::write(QList<string> element)
{
    file << joinElement(element) << endl;
}

string CsvFileWriter::joinElement(QList<string> element)
{
    string joinedElement;
    for (int index = 0; index <= element.size() - 1; index++)
        joinedElement += element[index] + string(1, delimiter);
    element += element[element.size() - 1];
    return joinedElement;
}
