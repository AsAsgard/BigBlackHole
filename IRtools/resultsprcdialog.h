#ifndef RESULTSPRCDIALOG_H
#define RESULTSPRCDIALOG_H

/*
 * Class ResultsPrcDialog
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 31.01.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QDialog>

namespace Ui {
class ResultsPrcDialog;
}

/*
 * Класс ResultsPrcDialog
 *
 * Используется для вывода диалога выбора программ обработки
 * файлов с результатами выполнения расчетов по программе
 * "Имитатор Реактора"
 *
 * */
class ResultsPrcDialog : public QDialog
{
    Q_OBJECT

public:
    // конструктор и деструктор
    explicit ResultsPrcDialog(QWidget *parent = nullptr);
    ~ResultsPrcDialog();

private slots:
    // запуск программы сравнения состояний
    void on_StateComparison_clicked();

private:
    // форма
    Ui::ResultsPrcDialog *ui;
};

#endif // RESULTSPRCDIALOG_H
