/*
 * Class HangingWindow implementation
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 30.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QFileDialog>
#include "hangingwindow.h"
#include "ui_hangingwindow.h"
#include "initialwindow.h"
#include "prepfilesdialog.h"
#include "resultsprcdialog.h"
#include "processid.h"
#include "inidata.h"

// конструктор
HangingWindow::HangingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HangingWindow)
{
    // установка формы
    ui->setupUi(this);
    // фиксация размера формы
    this->setFixedSize(this->size());
    // задание значений checkable action-ов меню языка на текущее значение
    if (IniData.Lang == Lang::RU) ui->Russian->trigger();
    if (IniData.Lang == Lang::EN) ui->English->trigger();
}

// деструктор
HangingWindow::~HangingWindow()
{
    delete ui;
}

// запуск программы "Имитатор Реактора"
void HangingWindow::on_IRButton_clicked()
{
    // старт нового процесса выполнения программы "Имитатор Реактора"
    startNewProcess(ProcessID[Programs::IR], this);
}

// вызов диалога запуска программ подготовки файлов
void HangingWindow::on_PrepFilesButton_clicked()
{
    // создание диалога
    PrepFilesDialog prepfilesdialog(this);
    // выравнивание диалога относительно текущего окна и границ экрана
    ALIGNMENT(prepfilesdialog, this);
    // вызов диалога
    prepfilesdialog.exec();
}

// вызов диалога запуска программ обработки результатов
void HangingWindow::on_ResultsPrcButton_clicked()
{
    // создание диалога
    ResultsPrcDialog resultprcdialog(this);
    // выравнивание диалога относительно текущего окна и границ экрана
    ALIGNMENT(resultprcdialog, this);
    // вызов диалога
    resultprcdialog.exec();
}

// сворачивание окна в маленькую панель внизу экрана
void HangingWindow::on_openInitialWindow_triggered()
{
    // создание главного окна и его вывод
    InitialWindow * initwind = new InitialWindow();
    initwind->setAttribute(Qt::WA_DeleteOnClose);
    initwind->show();
    // закрытие текущего окна
    this->close();
}

// событие закрытия окна
void HangingWindow::closeEvent(QCloseEvent *)
{
    // сохранение текущих позиций панели
    hangwindPos.setX(this->x());
    hangwindPos.setY(this->y());
}

// смена языка на английский
void HangingWindow::on_English_triggered()
{
    // установка значений для checkable action-ов меню языка
    ui->English->setChecked(true);
    ui->Russian->setChecked(false);
    // изменение языка приложения
    changeAppLanguage(Lang::EN);
}

// смена языка на русский
void HangingWindow::on_Russian_triggered()
{
    // установка значений для checkable action-ов меню языка
    ui->English->setChecked(false);
    ui->Russian->setChecked(true);
    // изменение языка приложения
    changeAppLanguage(Lang::RU);
}

// событие изменения чего-либо (используется для изменения языка окна)
void HangingWindow::changeEvent(QEvent *event)
{
    // если событие - изменение языка, то делаем перевод формы
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    // отправка сигнала классу родителю
    QMainWindow::changeEvent(event);
}

// открытие загрузки
void HangingWindow::on_openFuelLoad_triggered()
{
    // выбор директории загрузки
    QString Path = QFileDialog::getExistingDirectory(
                        this,
                        HangingWindow::tr("Open fuel load"),
                        IniData.TruePath ? IniData.Path : "./",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                        );
    // если директория выбрана - то меняем ее и выводим сообщение об успешной смене загрузки
    if (!Path.isEmpty())
    {
        IniData.TruePath = true;
        IniData.Path = Path;
        QMessageBox::information(this, QMessageBox::tr("Information"),
                                 QMessageBox::tr("Fuel Load was changed to: \"%1\"")
                                 .arg(IniData.Path),
                                 QMessageBox::Ok);
    }
}
