#ifndef PREPFILESDIALOG_H
#define PREPFILESDIALOG_H

/*
 * Class PrepFilesDialog
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 31.01.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QDialog>

namespace Ui {
class PrepFilesDialog;
}

/*
 * Класс PrepFilesDialog
 *
 * Используется для вывода диалога выбора программ подготовки
 * входных файлов для выполнения расчетов по программе
 * "Имитатор Реактора"
 *
 * */
class PrepFilesDialog : public QDialog
{
    Q_OBJECT

public:
    // конструктор и деструктор
    explicit PrepFilesDialog(QWidget *parent = nullptr);
    ~PrepFilesDialog();

private slots:
    // запуск программы Конвертер шлаков
    void on_SlagsConverter_clicked();
    // запуск программы подготовки файлов master.set и user.set
    void on_SetFilesMaker_clicked();

private:
    // форма
    Ui::PrepFilesDialog *ui;
};

#endif // PREPFILESDIALOG_H
