#ifndef HANGINGWINDOW_H
#define HANGINGWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class HangingWindow;
}

class HangingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HangingWindow(QWidget *parent = nullptr);
    ~HangingWindow();

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private slots:
    void on_IRButton_clicked();

    void on_PrepFilesButton_clicked();

    void on_ResultsPrcButton_clicked();

    void on_openInitialWindow_triggered();

    void on_English_triggered();

    void on_Russian_triggered();

    void on_openFuelLoad_triggered();

private:
    Ui::HangingWindow *ui;
};

#endif // HANGINGWINDOW_H
