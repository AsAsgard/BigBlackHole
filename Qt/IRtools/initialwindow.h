#ifndef INITIALWINDOW_H
#define INITIALWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QLabel>

namespace Ui {
class InitialWindow;
}

class InitialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InitialWindow(QWidget *parent = nullptr);
    ~InitialWindow();

private slots:
    void on_IRButton_clicked();

    void on_PrepFiles_button_clicked();

    void on_ResultsPrc_button_clicked();

    void on_openHangingWindow_triggered();

    void on_English_triggered();

    void on_Russian_triggered();

    void on_openFuelLoad_triggered();

private:
    Ui::InitialWindow *ui;
    // дополнительный Label для статус-бара
    QScopedPointer<QLabel> statusLabel;

    void to_hanging(void);
};


extern QPoint hangwindPos;

#endif // INITIALWINDOW_H
