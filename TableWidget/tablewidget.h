#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <FileManager/csvfilereader.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QObject>
#include <QTableWidget>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    QChar cellDelimiter = ';';
    QChar rowDelimiter = '\t';

    TableWidget(QWidget *parent = nullptr);

    void addRow(QStringList row);
    QStringList getRow(int rowIndex);
    QList<QStringList> getRows();
    void setRow(QStringList row, int rowIndex);
    void setRows(QList<QStringList> rows);

    void addTitle(QString columnName);
    QString getTitle(int index);
    QStringList getTitles();
    void setTitle(int columnIndex, QString title);
    void setTitles(QStringList titles);

    void setItem(int rowIndex, int columnIndex, QString textCell);

    QList<QModelIndex> getSelectedIndexes();
public slots:
    void addColumn();
    void addRow();
    void copy();
    void cut();
    void paste();
    void rebase();
    void removeSelectedColumns();
    void removeSelectedRows();
    void renameColumn();
    void moveColumn();
private:
    void addRowsFromRebasingFile(CsvFileReader* rebasingFileReader);
    QStringList createNewRowBasedOnRebasingOne(QStringList oldRow, QStringList rebasingTitles);
    QString getColumnNameWithDialog(QString title, QString label, QString text);
    bool getPermission(QString title, QString text);
    void pasteRowPart(QString row, int startColumnIndex, int rowIndex);
    QString takeRowPart(int rowIndex, int startColumnIndex, int endColumnIndex, bool is_removed=false);
    QString takePart(QTableWidgetSelectionRange range, bool is_removed=false);
    void swapColumns(int firstColumnIndex, int secondColumnIndex);
    void swapTitles(int firstColumnIndex, int secondColumnIndex);
    void swapItems(int firstColumnIndex, int secondColumnIndex);
};

#endif // TABLEWIDGET_H
