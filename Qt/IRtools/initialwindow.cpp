#include <QDesktopWidget>
#include <QFileDialog>
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
    if (IniData.Lang == Lang::RU) ui->Russian->trigger();
    if (IniData.Lang == Lang::EN) ui->English->trigger();
    updateStatusBar();

    // Fonts
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

void InitialWindow::updateStatusBar()
{
    if (IniData.TruePath) {
        statusLabel->setText(InitialWindow::tr("Fuel load: \"%1\"").arg(IniData.Path));
    } else {
        statusLabel->setText(InitialWindow::tr("Bad fuel load is opened. Open fuel load in the menu bar."));
    }
}

void InitialWindow::on_IRButton_clicked()
{
    if (startNewProcess(ProcessID[Programs::IR], this)) to_hanging();
}

void InitialWindow::on_PrepFilesButton_clicked()
{
    PrepFilesDialog prepfilesdialog(this);
    ALIGNMENT(prepfilesdialog, this);
    int dial_result = prepfilesdialog.exec();
    if (dial_result) to_hanging();
}

void InitialWindow::on_ResultsPrcButton_clicked()
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
    changeAppLanguage(Lang::EN);
}

void InitialWindow::on_Russian_triggered()
{
    ui->English->setChecked(false);
    ui->Russian->setChecked(true);
    changeAppLanguage(Lang::RU);
}

void InitialWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        updateStatusBar();
    }
    QMainWindow::changeEvent(event);
}

void InitialWindow::on_openFuelLoad_triggered()
{
    QString Path = QFileDialog::getExistingDirectory(
                        this,
                        InitialWindow::tr("Open fuel load"),
                        IniData.TruePath ? IniData.Path : "./",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                        );
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
