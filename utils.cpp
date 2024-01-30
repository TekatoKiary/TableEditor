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

void setClipboard(QString text)
{
    QClipboard* board = QGuiApplication::clipboard();
    board->setText(text);
}

CsvFileReader* getOpenFileReader(QWidget* parent)
{
    QString filePath = getOpenFilePath(parent);
    if(filePath.isEmpty() || filePath.isNull())
        throw domain_error("Передаваемый параметр пуст или равен Null");
    return new CsvFileReader(filePath);
}

QString getOpenFilePath(QWidget* parent)
{
    return QFileDialog::getOpenFileName(
                parent,
                "Открыть файл",
                QDir::homePath(),
                "Csv files (*.csv)"
                );
}

QString getSaveFilePath(QWidget* parent)
{
    return QFileDialog::getSaveFileName(
                parent,
                "Сохранить файл",
                QDir::homePath(),
                "Csv files (*.csv)"
                );
}
