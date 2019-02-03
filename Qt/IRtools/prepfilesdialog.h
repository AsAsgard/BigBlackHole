#ifndef PREPFILESDIALOG_H
#define PREPFILESDIALOG_H

#include <QDialog>

namespace Ui {
class PrepFilesDialog;
}

class PrepFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrepFilesDialog(QWidget *parent = nullptr);
    ~PrepFilesDialog();

private slots:
    void on_SlagsConverter_clicked();

    void on_SetFilesMaker_clicked();

private:
    Ui::PrepFilesDialog *ui;
};

#endif // PREPFILESDIALOG_H
