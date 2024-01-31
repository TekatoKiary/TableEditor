#ifndef PREVIEWNEWTABLECOLUMNDIALOG_H
#define PREVIEWNEWTABLECOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
class PreviewNewTableColumnDialog;
}

class PreviewNewTableColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewNewTableColumnDialog(QWidget *parent = nullptr, bool* ok = nullptr);
    ~PreviewNewTableColumnDialog();

    void setTitle(QString *title);
private:
    bool* ok;
    Ui::PreviewNewTableColumnDialog *ui;
    static bool execute(QWidget *parent, bool *ok, QString *titleColumn, QStringList *rowsColumn);
    void accepted();
    void rejected();
};

#endif // PREVIEWNEWTABLECOLUMNDIALOG_H
