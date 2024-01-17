#include "utils.h"


QString getFileNameFromAbsolutePath(QString filePath)
{
    std::filesystem::path path(filePath.toStdString());
    return QString::fromStdString(path.filename().string());
}

QStringList enumerateStringList(QStringList &list)
{
    for(int index = 0; index < list.count(); index++)
        list[index] = QString("%1. %2").arg(index + 1).arg(list[index]);
    return list;
}

QString enumerateString(int index, QString &string)
{
    return QString("%1. %2").arg(index + 1).arg(string);
}
