#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QInputDialog>
#include <QList>
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
private:
    Ui::MainWindow *ui;
    QString fileNamePath = "newFile.csv";
    QSize sizeWindow = QSize(800, 700);

    void loadTable(QList<QString> titles, QList<QList<QString> > elements);

    void setTitles(QList<QString> titles);
    void setElements(QList<QList<QString> > elements);
    QList<QString> getTitles();
    void setCurrentFileName(QString nameCurrentFile);
    QList<QList<QString> > getElements();
    QString getCell(int rowIndex, int columnIndex);
};
#endif // MAINWINDOW_H
