#ifndef UTILS_H
#define UTILS_H

#include <FileManager/csvfilereader.h>

#include <filesystem>
#include <QClipboard>
#include <QFileDialog>
#include <QGuiApplication>
#include <QString>
#include <QStringList>

QString getFileNameFromAbsolutePath(QString filePath);
QStringList enumerateStringList(QStringList &list);
QString enumerateString(int index, QString &string);
void setClipboard(QString text);
CsvFileReader* getOpenFileReader(QWidget* parent);
QString getOpenFilePath(QWidget* parent);
QString getSaveFilePath(QWidget* parent);

#endif // UTILS_H
