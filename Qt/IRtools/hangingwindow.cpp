#include <QFileDialog>
#include "hangingwindow.h"
#include "ui_hangingwindow.h"
#include "initialwindow.h"
#include "prepfilesdialog.h"
#include "resultsprcdialog.h"
#include "processid.h"
#include "inidata.h"

HangingWindow::HangingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HangingWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    if (IniData.TrueLang) {
        if (IniData.Lang == Lang::RU) ui->Russian->trigger();
        if (IniData.Lang == Lang::EN) ui->English->trigger();
    } else {
        ui->English->trigger();
    }
}

HangingWindow::~HangingWindow()
{
    delete ui;
}

void HangingWindow::on_IRButton_clicked()
{
    startNewProcess(ProcessID[Programs::IR], this);
}

void HangingWindow::on_PrepFilesButton_clicked()
{
    PrepFilesDialog prepfilesdialog(this);
    ALIGNMENT(prepfilesdialog, this);
    prepfilesdialog.exec();
}

void HangingWindow::on_ResultsPrcButton_clicked()
{
    ResultsPrcDialog resultprcdialog(this);
    ALIGNMENT(resultprcdialog, this);
    resultprcdialog.exec();
}

void HangingWindow::on_openInitialWindow_triggered()
{
    InitialWindow * initwind = new InitialWindow();
    initwind->setAttribute(Qt::WA_DeleteOnClose);
    initwind->show();
    this->close();
}

void HangingWindow::closeEvent(QCloseEvent *)
{
    hangwindPos.setX(this->x());
    hangwindPos.setY(this->y());
}

void HangingWindow::on_English_triggered()
{
    ui->English->setChecked(true);
    ui->Russian->setChecked(false);
    IniData.TrueLang = true;
    IniData.Lang = Lang::EN;
}

void HangingWindow::on_Russian_triggered()
{
    ui->English->setChecked(false);
    ui->Russian->setChecked(true);
    IniData.TrueLang = true;
    IniData.Lang = Lang::RU;
}

void HangingWindow::on_openFuelLoad_triggered()
{
    QString Path = QFileDialog::getExistingDirectory(
                        this,
                        InitialWindow::tr("Open Fuel Load"),
                        IniData.TruePath ? IniData.Path : "./",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                        );
    if (!Path.isEmpty())
    {
        IniData.TruePath = true;
        IniData.Path = Path;
        QMessageBox::information(this, QMessageBox::tr("Information"),
                                 QMessageBox::tr("Fuel Load was changed to: %1")
                                 .arg(IniData.Path),
                                 QMessageBox::Ok);
    }
}
