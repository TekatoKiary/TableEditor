#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <FileManager/csvfilereader.h>
#include <TableWidget/table.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QObject>
#include <QTableWidget>

class TableWidget: public QTableWidget
{
    Q_OBJECT
public:
    QChar cellDelimiter = ';';
    QChar rowDelimiter = '\t';

    TableWidget(QWidget *parent = nullptr);

    QList<QStringList> getRows();
    void setRow(QStringList row, int rowIndex);
    void setRows(QList<QStringList> rows);

    QStringList getTitles();
    void setTitles(QStringList titles);

    void setItem(int rowIndex, int columnIndex, QString textCell);

    QList<QModelIndex> getSelectedIndexes();
public slots:
    void addColumn();
    void addRow();
//    void combineSelectedColumns();
    void copy();
    void cut();
    void paste();
    void rebase();
    void removeSelectedColumns();
    void removeSelectedRows();
    void renameColumn();
    void tableChanged(QTableWidgetItem *item);
private:
    Table table;

    QString getUserTitleWithDialog(QString title, QString label, QString text);
    bool getPermission(QString title, QString text);
    void pasteRowPart(QString row, int startColumnIndex, int rowIndex);
    QString takeRowPart(int rowIndex, int startColumnIndex, int endColumnIndex, bool is_removed=false);
    QString takePart(QTableWidgetSelectionRange range, bool is_removed=false);
    void setViewTitles(QStringList titles);
};

#endif // TABLEWIDGET_H
