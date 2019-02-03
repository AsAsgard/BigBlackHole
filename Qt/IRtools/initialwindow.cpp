#include <QDesktopWidget>
#include <QFileDialog>
#include <QDebug>
#include "initialwindow.h"
#include "ui_initialwindow.h"
#include "prepfilesdialog.h"
#include "resultsprcdialog.h"
#include "hangingwindow.h"
#include "processid.h"
#include "inidata.h"

QPoint hangwindPos(0,0);

InitialWindow::InitialWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InitialWindow)
{
    ui->setupUi(this);
    ui->IRButton->setDefault(true);
    this->setFixedSize(this->size());
    ui->statusBar->resize(this->width(), 20);
    statusLabel.reset(new QLabel(ui->statusBar));
    statusLabel->setGeometry(4, 0, this->width(), ui->statusBar->height());
    statusLabel->setAlignment(Qt::AlignVCenter);
    if (IniData.TrueLang) {
        if (IniData.Lang == Lang::RU) ui->Russian->trigger();
        if (IniData.Lang == Lang::EN) ui->English->trigger();
    } else {
        ui->English->trigger();
    }
    if (IniData.TruePath) {
        statusLabel->setText(InitialWindow::tr("Fuel load: \"%1\"").arg(IniData.Path));
    } else {
        statusLabel->setText(InitialWindow::tr("Bad fuel load is opened. Open fuel load in the menu bar."));
    }
}

InitialWindow::~InitialWindow()
{
    delete ui;
}

void InitialWindow::to_hanging(void)
{
    HangingWindow * hangwind = new HangingWindow();
    hangwind->setAttribute(Qt::WA_DeleteOnClose);
    if (!hangwindPos.isNull()) {
        // remembered position
        hangwind->move(hangwindPos.x(), hangwindPos.y());
    } else {
        // default position
        QDesktopWidget Desktop;
        hangwind->move(Desktop.screenGeometry(this).x() + Desktop.screenGeometry(this).width() - 0.030 * Desktop.screenGeometry(this).width() - hangwind->width(),
                       Desktop.screenGeometry(this).y() + Desktop.screenGeometry(this).height() - 0.095 * Desktop.screenGeometry(this).height() - hangwind->height());
    }
    hangwind->show();
    this->close();
}

void InitialWindow::on_IRButton_clicked()
{
    startNewProcess(ProcessID[Programs::IR], this);
}

void InitialWindow::on_PrepFiles_button_clicked()
{
    PrepFilesDialog prepfilesdialog(this);
    ALIGNMENT(prepfilesdialog, this);
    int dial_result = prepfilesdialog.exec();
    if (dial_result) to_hanging();
}

void InitialWindow::on_ResultsPrc_button_clicked()
{
    ResultsPrcDialog resultprcdialog(this);
    ALIGNMENT(resultprcdialog, this);
    int dial_result = resultprcdialog.exec();
    if (dial_result) to_hanging();
}

void InitialWindow::on_openHangingWindow_triggered()
{
    to_hanging();
}

void InitialWindow::on_English_triggered()
{
    ui->English->setChecked(true);
    ui->Russian->setChecked(false);
    IniData.TrueLang = true;
    IniData.Lang = Lang::EN;
}

void InitialWindow::on_Russian_triggered()
{
    ui->English->setChecked(false);
    ui->Russian->setChecked(true);
    IniData.TrueLang = true;
    IniData.Lang = Lang::RU;
}

void InitialWindow::on_openFuelLoad_triggered()
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
        statusLabel->setText(InitialWindow::tr("Fuel load: \"%1\"").arg(IniData.Path));
        QMessageBox::information(this, QMessageBox::tr("Information"),
                                 QMessageBox::tr("Fuel Load was changed to: \"%1\"")
                                 .arg(IniData.Path),
                                 QMessageBox::Ok);
    }
}
