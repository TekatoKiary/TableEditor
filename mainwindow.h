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
private:
    Ui::MainWindow *ui;
    QString filePath = "newFile.csv";
    QSize sizeWindow = QSize(800, 700);

    void loadTable(QStringList titles, QList<QStringList> rows);
    void setCurrentFileName(QString nameCurrentFile);
    void setSectionResizeModeInTitles();
    void setFilePath(QString newFilePath);
    void setEnabledAction(QAction* action, bool isEnabled);
};
#endif // MAINWINDOW_H
