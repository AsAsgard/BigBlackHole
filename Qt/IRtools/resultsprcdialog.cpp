#include "resultsprcdialog.h"
#include "ui_resultsprcdialog.h"
#include "processid.h"


ResultsPrcDialog::ResultsPrcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsPrcDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // Fonts
    QFont font(ui->Choice_label->font());
    font.setPixelSize(16);
    ui->Choice_label->setFont(font);

    font = ui->StateComparison->font();
    font.setPixelSize(12);
    ui->StateComparison->setFont(font);
}

ResultsPrcDialog::~ResultsPrcDialog()
{
    delete ui;
}

void ResultsPrcDialog::on_StateComparison_clicked()
{
    if (startNewProcess(ProcessID[Programs::StateComparison], this)) this->accept();
}
