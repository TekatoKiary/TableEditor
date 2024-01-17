#ifndef UTILS_H
#define UTILS_H


#include <filesystem>
#include <QString>
#include <QStringList>

QString getFileNameFromAbsolutePath(QString filePath);
QStringList enumerateStringList(QStringList &list);
QString enumerateString(int index, QString &string);

#endif // UTILS_H
