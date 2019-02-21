#include "prepfilesdialog.h"
#include "ui_prepfilesdialog.h"
#include "processid.h"

PrepFilesDialog::PrepFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepFilesDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // Fonts
    QFont font(ui->Choice_label->font());
    font.setPixelSize(16);
    ui->Choice_label->setFont(font);

    font = ui->SlagsConverter->font();
    font.setPixelSize(12);
    ui->SlagsConverter->setFont(font);

    font = ui->SetFilesMaker->font();
    font.setPixelSize(12);
    ui->SetFilesMaker->setFont(font);

    ui->retranslateUi(this);
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
