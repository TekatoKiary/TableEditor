#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FileManager/csvfilereader.h>

#include <QFileDialog>
#include <QInputDialog>
#include <QList>
#include <QMessageBox>
#include <QMainWindow>

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
    void addColumn();

    void addElement();
    void saveFile();
    void deleteColumn();
    void deleteElement();
    void saveAsFile();
    void rebaseTable();
private:
    Ui::MainWindow *ui;
    QString filePath = "newFile.csv";
    QSize sizeWindow = QSize(800, 700);

    void loadTable(QList<QString> titles, QList<QList<QString> > elements);
    void setTitles(QList<QString> titles);
    void setElements(QList<QList<QString> > elements);
    QList<QString> getTitles();
    void setCurrentFileName(QString nameCurrentFile);
    QList<QList<QString> > getElements();
    QString getCell(int rowIndex, int columnIndex);
    void setSectionResizeModeInTitles();
    bool getPermission(QString title, QString text);
    void setFilePath(QString newFilePath);
    QString getSaveFilePath();
    QString getOpenFilePath();
    void addElement(QStringList element);
    CsvFileReader getRebasingFileReader();
    void addElementsFromRebasingFile(CsvFileReader rebasingFileReader);
    QStringList createNewElementBasedOnRebasingOne(QStringList oldElement, QStringList rebasingTitles);
};
#endif // MAINWINDOW_H
