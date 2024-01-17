#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FileManager/csvfilereader.h>

#include <QClipboard>
#include <QFileDialog>
#include <QList>
#include <QMainWindow>
#include <QTableWidgetItem>
#include <fstream>

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();
    void saveAsFile();
    void rebaseTable();
    void copyTablePart();
    void pasteTablePart();
    void cutTablePart();
private:
    Ui::MainWindow *ui;
    QString filePath = "newFile.csv";
    QSize sizeWindow = QSize(800, 700);
    QChar cellDelimiter = ';';
    QChar rowDelimiter = '\t';

    void loadTable(QStringList titles, QList<QStringList> rows);
    void setCurrentFileName(QString nameCurrentFile);
    void setSectionResizeModeInTitles();
    void setFilePath(QString newFilePath);
    QString getSaveFilePath();
    QString getOpenFilePath();
    CsvFileReader getRebasingFileReader();
    void addRowsFromRebasingFile(CsvFileReader rebasingFileReader);
    QStringList createNewRowBasedOnRebasingOne(QStringList oldElement, QStringList rebasingTitles);
    void setClipboard(QString text);
    QString takeTablePart(QTableWidgetSelectionRange range, bool is_removed=false);
    QString takeRow(int rowIndex, int startColumnIndex, int endColumnIndex, bool is_removed=false);
};
#endif // MAINWINDOW_H
