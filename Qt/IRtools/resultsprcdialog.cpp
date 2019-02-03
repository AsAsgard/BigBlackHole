#include "resultsprcdialog.h"
#include "ui_resultsprcdialog.h"
#include "processid.h"


ResultsPrcDialog::ResultsPrcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsPrcDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

ResultsPrcDialog::~ResultsPrcDialog()
{
    delete ui;
}

void ResultsPrcDialog::on_StateComparison_clicked()
{
    if (startNewProcess(ProcessID[Programs::StateComparison], this)) this->accept();
}