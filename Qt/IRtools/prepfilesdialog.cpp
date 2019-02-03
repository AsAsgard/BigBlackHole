#include "prepfilesdialog.h"
#include "ui_prepfilesdialog.h"
#include "processid.h"

PrepFilesDialog::PrepFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepFilesDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

PrepFilesDialog::~PrepFilesDialog()
{
    delete ui;
}

void PrepFilesDialog::on_SlagsConverter_clicked()
{
    if (startNewProcess(ProcessID[Programs::SlagsConverter], this)) this->accept();
}

void PrepFilesDialog::on_SetFilesMaker_clicked()
{
    if (startNewProcess(ProcessID[Programs::SetFilesMaker], this)) this->accept();
}
