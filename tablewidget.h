#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QObject>
#include <QMessageBox>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget *parent = nullptr);
    QList<QStringList> getRows();
    QStringList getRow(int rowIndex);
    QStringList getTitles();
    void setTitles(QStringList titles);
    void setItem(int rowIndex, int columnIndex, QString textCell);
    void setRows(QList<QStringList> rows);
    void addRow(QStringList row);
    QList<QModelIndex> getSelectedIndexes();
public slots:
    void removeSelectedColumns();
    void removeSelectedRows();
    void addRow();
    void addColumn();
    void renameColumn();
signals:
    void cellHighlighted(bool);
private:
    bool getPermission(QString title, QString text);
    QString getColumnName(QString title, QString label, QString text);
};

#endif // TABLEWIDGET_H
