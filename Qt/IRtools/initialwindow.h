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

protected:
    void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_IRButton_clicked();

    void on_PrepFilesButton_clicked();

    void on_ResultsPrcButton_clicked();

    void on_openHangingWindow_triggered();

    void on_English_triggered();

    void on_Russian_triggered();

    void on_openFuelLoad_triggered();

private:
    Ui::InitialWindow *ui;
    // дополнительный Label для статус-бара
    QScopedPointer<QLabel> statusLabel;

    void to_hanging(void);
    void updateStatusBar(void);
};


extern QPoint hangwindPos;

#endif // INITIALWINDOW_H
