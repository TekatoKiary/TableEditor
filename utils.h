#ifndef UTILS_H
#define UTILS_H

#include <filesystem>
#include <QString>


QString getFileNameFromAbsolutePath(QString filePath)
{
    std::filesystem::path path(filePath.toStdString());
    return QString::fromStdString(path.filename().string());
}

#endif // UTILS_H
