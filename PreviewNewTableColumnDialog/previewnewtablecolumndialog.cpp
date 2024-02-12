#include "previewnewtablecolumndialog.h"
#include "ui_previewnewtablecolumndialog.h"

PreviewNewTableColumnDialog::PreviewNewTableColumnDialog(QWidget *parent, bool* ok) :
    QDialog(parent),
    ui(new Ui::PreviewNewTableColumnDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(1);
    this->ok = ok;
}

PreviewNewTableColumnDialog::~PreviewNewTableColumnDialog()
{
    delete ui;
}

void PreviewNewTableColumnDialog::setTitle(QString* title)
{
//    ui->tableWidget->addTitle(new QTableWidgetItem(*title));
}

void PreviewNewTableColumnDialog::accepted()
{
}

void PreviewNewTableColumnDialog::rejected()
{
}


bool PreviewNewTableColumnDialog::execute(QWidget *parent, bool* ok, QString* titleColumn, QStringList* rowsColumn)
{
    PreviewNewTableColumnDialog dialog(parent, ok);
}
