#ifndef INITIALWINDOW_H
#define INITIALWINDOW_H

/*
 * Class InitialWindow
 *
 * Version 1.3
 *
 * Writed by Brylkin Dmitry. 30.01.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QMainWindow>
#include <QScopedPointer>
#include <QLabel>

namespace Ui {
class InitialWindow;
}

/*
 * Класс InitialWindow
 *
 * Используется как первоначальное окно программы IR-tools
 * Содержит в себе возможности запуска:
 *  1) программы "Имитатор Реактора",
 *  2) программ подготовки входных файлов для выполнения расчетов
 *     по программе "Имитатор Реактора",
 *  3) программ обработки результатов выполнененных расчетов
 *     по программе "Имитатор Реактора"
 *
 * Реализует опции выбора языка для комплекса программ,
 * открытие необходимой загрузки (точка входа для других программ).
 *
 * */
class InitialWindow : public QMainWindow
{
    Q_OBJECT

public:
    // конструктор и деструктор
    explicit InitialWindow(QWidget *parent = nullptr);
    ~InitialWindow();

protected:
    // событие изменения чего-либо (используется для изменения языка окна)
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    // запуск программы "Имитатор Реактора"
    void on_IRButton_clicked();
    // вызов диалога запуска программ подготовки файлов
    void on_PrepFilesButton_clicked();
    // вызов диалога запуска программ обработки результатов
    void on_ResultsPrcButton_clicked();
    // сворачивание окна в маленькую панель внизу экрана
    void on_openHangingWindow_triggered();
    // смена языка на английский
    void on_English_triggered();
    // смена языка на русский
    void on_Russian_triggered();
    // открытие загрузки
    void on_openFuelLoad_triggered();

private:
    // ПОЛЯ
    // форма
    Ui::InitialWindow *ui;
    // дополнительный Label для статус-бара
    QScopedPointer<QLabel> statusLabel;

    // МЕТОДЫ
    // приватный метод, выполняющий свертку в панель
    void to_hanging(void);
    // обновить статус бар
    void updateStatusBar(void);
};

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// позиция панели на мониторе
extern QPoint hangwindPos;

#endif // INITIALWINDOW_H
