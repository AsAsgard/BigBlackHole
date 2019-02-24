/*
 * Class PrepFilesDialog implementation
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 31.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include "prepfilesdialog.h"
#include "ui_prepfilesdialog.h"
#include "processid.h"

// конструктор
PrepFilesDialog::PrepFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrepFilesDialog)
{
    // установка формы
    ui->setupUi(this);
    // фиксация размера формы
    this->setFixedSize(this->size());

    // параметры шрифтов для совместимости с high dpi
    QFont font(ui->Choice_label->font());
    font.setPixelSize(16);
    ui->Choice_label->setFont(font);

    font = ui->SlagsConverter->font();
    font.setPixelSize(12);
    ui->SlagsConverter->setFont(font);

    font = ui->SetFilesMaker->font();
    font.setPixelSize(12);
    ui->SetFilesMaker->setFont(font);
}

// деструктор
PrepFilesDialog::~PrepFilesDialog()
{
    delete ui;
}

// запуск программы Конвертер шлаков
void PrepFilesDialog::on_SlagsConverter_clicked()
{
    // старт нового процесса выполнения программы конвертирования шлаков
    if (startNewProcess(ProcessID[Programs::SlagsConverter], this)) this->accept();
}

// запуск программы подготовки файлов master.set и user.set
void PrepFilesDialog::on_SetFilesMaker_clicked()
{
    // старт нового процесса выполнения программы создания файлов master.set и user.set
    if (startNewProcess(ProcessID[Programs::SetFilesMaker], this)) this->accept();
}
