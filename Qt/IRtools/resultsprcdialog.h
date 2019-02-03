#ifndef RESULTSPRCDIALOG_H
#define RESULTSPRCDIALOG_H

#include <QDialog>

namespace Ui {
class ResultsPrcDialog;
}

class ResultsPrcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsPrcDialog(QWidget *parent = nullptr);
    ~ResultsPrcDialog();

private slots:

    void on_StateComparison_clicked();

private:
    Ui::ResultsPrcDialog *ui;
};

#endif // RESULTSPRCDIALOG_H
