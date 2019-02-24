/*
 * Class InitialWindow implementation
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 30.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QDesktopWidget>
#include <QFileDialog>
#include "initialwindow.h"
#include "ui_initialwindow.h"
#include "prepfilesdialog.h"
#include "resultsprcdialog.h"
#include "hangingwindow.h"
#include "processid.h"
#include "inidata.h"

// создание глобальной переменной позиции панели
QPoint hangwindPos(0,0);

// конструктор
InitialWindow::InitialWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InitialWindow)
{
    // установка формы
    ui->setupUi(this);
    // делаем кнопку запуска "Имитатора Реактора" - кнопкой по умолчанию
    ui->IRButton->setDefault(true);
    // фиксация размера формы
    this->setFixedSize(this->size());
    // задаем параметры текста в статус баре
    ui->statusBar->resize(this->width(), 20);
    statusLabel.reset(new QLabel(ui->statusBar));
    statusLabel->setGeometry(4, 0, this->width(), ui->statusBar->height());
    statusLabel->setAlignment(Qt::AlignVCenter);
    // задание значений checkable action-ов меню языка на текущее значение
    if (IniData.Lang == Lang::RU) ui->Russian->trigger();
    if (IniData.Lang == Lang::EN) ui->English->trigger();
    // обновляем статус бар
    updateStatusBar();

    // параметры шрифтов для совместимости с high dpi
    QFont font(ui->IRtoolsLabel->font());
    font.setPixelSize(64);
    ui->IRtoolsLabel->setFont(font);

    font = ui->ChooseOptionLabel->font();
    font.setPixelSize(16);
    ui->ChooseOptionLabel->setFont(font);

    font = ui->GreetingsLabel->font();
    font.setPixelSize(16);
    ui->GreetingsLabel->setFont(font);

    font = ui->HappyUsingLabel->font();
    font.setPixelSize(13);
    ui->HappyUsingLabel->setFont(font);

    font = statusLabel->font();
    font.setPixelSize(13);
    statusLabel->setFont(font);

    font = ui->statusBar->font();
    font.setPixelSize(13);
    ui->statusBar->setFont(font);

    font = ui->IRButton->font();
    font.setPixelSize(13);
    ui->IRButton->setFont(font);

    font = ui->PrepFilesButton->font();
    font.setPixelSize(13);
    ui->PrepFilesButton->setFont(font);

    font = ui->ResultsPrcButton->font();
    font.setPixelSize(13);
    ui->ResultsPrcButton->setFont(font);
}

// деструктор
InitialWindow::~InitialWindow()
{
    delete ui;
}

// приватный метод, выполняющий свертку в панель
void InitialWindow::to_hanging(void)
{
    // создаем панель
    HangingWindow * hangwind = new HangingWindow();
    hangwind->setAttribute(Qt::WA_DeleteOnClose);
    // проверяем позицию панели на нулевые значения
    if (!hangwindPos.isNull()) {
        // если не нулевые значения, то выставляем записанную позицию
        hangwind->move(hangwindPos.x(), hangwindPos.y());
    } else {
        // иначе выставляем на начальную позицию
        QDesktopWidget Desktop;
        hangwind->move(Desktop.screenGeometry(this).x() + Desktop.screenGeometry(this).width() - 0.030 * Desktop.screenGeometry(this).width() - hangwind->width(),
                       Desktop.screenGeometry(this).y() + Desktop.screenGeometry(this).height() - 0.095 * Desktop.screenGeometry(this).height() - hangwind->height());
    }
    // показываем панель
    hangwind->show();
    // закрываем главное окно
    this->close();
}

// обновить статус бар
void InitialWindow::updateStatusBar()
{
    // проверям правильность загрузки
    if (IniData.TruePath) {
        // если путь верный - выводим путь к загрузке
        statusLabel->setText(InitialWindow::tr("Fuel load: \"%1\"").arg(IniData.Path));
    } else {
        // иначе выводим сообщение о неправильной загрузке
        statusLabel->setText(InitialWindow::tr("Bad fuel load is opened. Open fuel load in the menu bar."));
    }
}

// запуск программы "Имитатор Реактора"
void InitialWindow::on_IRButton_clicked()
{
    // старт нового процесса выполнения программы "Имитатор Реактора"
    if (startNewProcess(ProcessID[Programs::IR], this)) to_hanging();
}

// вызов диалога запуска программ подготовки файлов
void InitialWindow::on_PrepFilesButton_clicked()
{
    // создание диалога
    PrepFilesDialog prepfilesdialog(this);
    // выравнивание диалога относительно текущего окна и границ экрана
    ALIGNMENT(prepfilesdialog, this);
    // вызов диалога
    int dial_result = prepfilesdialog.exec();
    // если диалог принят - сворачиваем приложение в панель
    if (dial_result) to_hanging();
}

// вызов диалога запуска программ обработки результатов
void InitialWindow::on_ResultsPrcButton_clicked()
{
    // создание диалога
    ResultsPrcDialog resultprcdialog(this);
    // выравнивание диалога относительно текущего окна и границ экрана
    ALIGNMENT(resultprcdialog, this);
    // вызов диалога
    int dial_result = resultprcdialog.exec();
    // если диалог принят - сворачиваем приложение в панель
    if (dial_result) to_hanging();
}

// сворачивание окна в маленькую панель внизу экрана
void InitialWindow::on_openHangingWindow_triggered()
{
    // переадресация на приватную функцию сворачивания в панель
    to_hanging();
}

// выбран английский язык
void InitialWindow::on_English_triggered()
{
    // установка значений для checkable action-ов меню языка
    ui->English->setChecked(true);
    ui->Russian->setChecked(false);
    // изменение языка приложения
    changeAppLanguage(Lang::EN);
}

// выбран русский язык
void InitialWindow::on_Russian_triggered()
{
    // установка значений для checkable action-ов меню языка
    ui->English->setChecked(false);
    ui->Russian->setChecked(true);
    // изменение языка приложения
    changeAppLanguage(Lang::RU);
}

// событие изменения чего-либо (используется для изменения языка окна)
void InitialWindow::changeEvent(QEvent *event)
{
    // если событие - изменение языка, то делаем перевод формы
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        updateStatusBar();
    }
    // отправка сигнала классу родителю
    QMainWindow::changeEvent(event);
}

// открытие загрузки
void InitialWindow::on_openFuelLoad_triggered()
{
    // выбор директории загрузки
    QString Path = QFileDialog::getExistingDirectory(
                        this,
                        InitialWindow::tr("Open fuel load"),
                        IniData.TruePath ? IniData.Path : "./",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                        );
    // если директория выбрана - то меняем ее и выводим сообщение об успешной смене загрузки
    if (!Path.isEmpty())
    {
        IniData.TruePath = true;
        IniData.Path = Path;
        updateStatusBar();
        QMessageBox::information(this, QMessageBox::tr("Information"),
                                 QMessageBox::tr("Fuel Load was changed to: \"%1\"")
                                 .arg(IniData.Path),
                                 QMessageBox::Ok);
    }
}
