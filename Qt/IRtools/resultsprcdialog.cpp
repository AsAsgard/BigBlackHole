/*
 * Class ResultsPrcDialog implementation
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 31.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include "resultsprcdialog.h"
#include "ui_resultsprcdialog.h"
#include "processid.h"

// конструктор
ResultsPrcDialog::ResultsPrcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsPrcDialog)
{
    // установка формы
    ui->setupUi(this);
    // фиксация размера формы
    this->setFixedSize(this->size());

    // параметры шрифтов для совместимости с high dpi
    QFont font(ui->Choice_label->font());
    font.setPixelSize(16);
    ui->Choice_label->setFont(font);

    font = ui->StateComparison->font();
    font.setPixelSize(12);
    ui->StateComparison->setFont(font);
}

// деструктор
ResultsPrcDialog::~ResultsPrcDialog()
{
    delete ui;
}

// запуск программы сравнения состояний
void ResultsPrcDialog::on_StateComparison_clicked()
{
    // старт нового процесса выполнения программы сравнения состояний
    if (startNewProcess(ProcessID[Programs::StateComparison], this)) this->accept();
}
