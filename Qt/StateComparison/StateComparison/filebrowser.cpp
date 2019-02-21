#include <QDesktopWidget>
#include <QFileDialog>
#include <QString>
#include <QFont>
#include "filebrowser.h"
#include "comparisonfield.h"

using namespace std;


FileBrowser::FileBrowser(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// делаем окно модальным и фиксированного размера
	FileBrowser::setFixedSize(FileBrowser::size());
	FileBrowser::setWindowModality(Qt::ApplicationModal);

    // установка начального значения языка
    if (langData.Lang == Lang::RU) ui.Russian->trigger();
    if (langData.Lang == Lang::EN) ui.English->trigger();

	// изменяем размеры статус-бара, создаем на нем Label, задаем его геометрию и выравнивание и вписываем текст
	ui.statusBar->resize(this->width(), 20);
	statusLabel.reset(new QLabel(ui.statusBar));
	statusLabel->setGeometry(4, 0, this->width(), ui.statusBar->height());
	statusLabel->setAlignment(Qt::AlignVCenter);
    CheckingFileNames();
	
	// параметры шрифтов для совместимости с high dpi
	QFont font(ui.File1Label->font());
	font.setPixelSize(15);
	ui.File1Label->setFont(font);
	ui.File2Label->setFont(font);
	font = ui.CommonLabel->font();
	font.setPixelSize(16);
	ui.CommonLabel->setFont(font);
	font = ui.File1lineEdit->font();
	font.setPixelSize(16);
	ui.File1lineEdit->setFont(font);
	ui.File2lineEdit->setFont(font);
}

// деструктор
FileBrowser::~FileBrowser() {}

// обозреватель первого состояния
void FileBrowser::on_File1Button_clicked() 
{
	// обновляем статус-бар
    statusLabel->setText(tr("Select file with the first state."));
	// вызываем диалог открытия файла
	QString FileName = QFileDialog::getOpenFileName(
		this,
        tr("Select file with the first state."),
		defaultPath(),
		"All files (*.*);;Dat file (*.dat);;Text file (*.txt);;Exp files (*.exp)"
		);
	// если файл выбран
	if (!FileName.isEmpty()) 
	{
		// задаем новый стандартный путь
		setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
		// заносим имя файла в LineEdit
		ui.File1lineEdit->setText(FileName);
		// делаем запись в TextBrowser-е
        ui.textBrowser->append(tr("File %1 is placed to the line \"First state\".")
                               .arg(FileName));
	}
	// обновляем статус-бар
	CheckingFileNames();
}

// обозреватель второго состояния
void FileBrowser::on_File2Button_clicked() 
{
	// обновляем статус-бар
    statusLabel->setText(tr("Select file with the second state."));
	// вызываем диалог открытия файла
	QString FileName = QFileDialog::getOpenFileName(
		this,
        tr("Select file with the second state."),
		defaultPath(),
		"All files (*.*);;Dat file (*.dat);;Text file (*.txt);;Exp files (*.exp)"
		);
	// если файл выбран
	if (!FileName.isEmpty()) 
	{
		// задаем новый стандартный путь
		setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
		// заносим имя файла в LineEdit
		ui.File2lineEdit->setText(FileName);
		// делаем запись в TextBrowser-е
        ui.textBrowser->append(tr("File %1 is placed to the line \"Second state\".")
                               .arg(FileName));
	}
	// обновляем статус-бар
	CheckingFileNames();
}

// окончание модифицирования текста в первом LineEdit - фиксирование нового имени файла
void FileBrowser::on_File1lineEdit_editingFinished()
{
	// если имя файла было модифицировано
	if (ui.File1lineEdit->isModified())
	{
		// фиксируем имя файла
		ui.File1lineEdit->setModified(false);
		QString FileName = ui.File1lineEdit->text();
		// если имя файла не пустое
		if (!FileName.isEmpty()) 
		{
			// задаем новый стандартный путь
			setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
			// делаем запись в TextBrowser-е
            ui.textBrowser->append(tr("File %1 is placed to the line \"First state\".")
                                   .arg(FileName));
		}
		// обновляем статус-бар
		CheckingFileNames();
	}
}

// при редактировании имен файлов - происходит автоматическое обновление статус-бара
void FileBrowser::on_File1lineEdit_textEdited(QString) { CheckingFileNames();}

// окончание модифицирования текста во втором LineEdit - фиксирование нового имени файла
void FileBrowser::on_File2lineEdit_editingFinished()
{
	// если имя файла было модифицировано
	if (ui.File2lineEdit->isModified()) 
	{
		// фиксируем имя файла
		ui.File2lineEdit->setModified(false);
		QString FileName = ui.File2lineEdit->text();
		// если имя файла не пустое
		if (!FileName.isEmpty()) 
		{
			// задаем новый стандартный путь
			setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
			// делаем запись в TextBrowser-е
            ui.textBrowser->append(tr("File %1 is placed to the line \"Second state\".")
                                   .arg(FileName));
		}
		// обновляем статус-бар
		CheckingFileNames();
	}
}

// при редактировании имен файлов - происходит автоматическое обновление статус-бара
void FileBrowser::on_File2lineEdit_textEdited(QString) { CheckingFileNames();}


// макрос проверки ошибок при считывании файлов
bool FileBrowser::errCheck(ErrCode ErrFlag, QString FileName)
{
    /* если функция завершилась неудачно	*/
    if (ErrFlag != 0) {
        /* выводим сообщение об ошибке		*/
        ui.textBrowser->append("");
        ui.textBrowser->append(tr("Error!"));
        if (ErrFlag == 1) {
            /* выводим сообщение об ошибке в пути к файлу		*/
            ui.textBrowser->append(tr("Error during opening the file! This file doesn't exist!"));
            ui.textBrowser->append(tr("Problem with the file %1. Check the correctness of the path.").arg(FileName));
            statusLabel->setText(tr("Check the correctness of the path to the file with problem."));
        } else if (ErrFlag == 2) {
            /* выводим сообщение об ошибке в формате файла		*/
            ui.textBrowser->append(tr("Error during reading the file! Wrong data format! The data wasn't read!"));
            ui.textBrowser->append(tr("Problem with the file %1. Check the correctness of the data format.").arg(FileName));
            statusLabel->setText(tr("Check the data format of the file with problem."));
        } else {
            /* выводим сообщение о непредвиденной ошибке		*/
            ui.textBrowser->append(tr("Problem with the file %1.").arg(FileName));
            ui.textBrowser->append(tr("Try to change the file with problem and restart the program. If the problem persists, contact the developer."));
            statusLabel->setText(tr("Try to change the file with problem and restart the program."));
        }
        /* возвращаем предыдущее число слоев и выходим из функции	*/
        cDataState::setRememberedNumLayers();
        return false;
    } else {
        return true;
    }
}


// нажата кнопка "Сравнить"
void FileBrowser::on_Compare_clicked()
{
	// начальная проверка - как при обновлении статус-бара, только с занесением данных также и в TextBrowser
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
        ui.textBrowser->append(tr("Enter the names of the files with reactor states to the lines \"First state\" and \"Second state\"."));
        statusLabel->setText(tr("Enter the names of the files with reactor states you want to compare."));
		return;
	}
	if (ui.File1lineEdit->text().isEmpty())
	{
        ui.textBrowser->append(tr("Enter the name of the file with the first state into the line \"First state\"."));
        statusLabel->setText(tr("Enter the name of the file with the first state into the line \"First state\"."));
		return;
	}
	if (ui.File2lineEdit->text().isEmpty())
	{
        ui.textBrowser->append(tr("Enter the name of the file with the second state into the line \"Second state\"."));
        statusLabel->setText(tr("Enter the name of the file with the second state into the line \"Second state\"."));
		return;
	}
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
        ui.textBrowser->append(tr("Select different files to compare."));
        statusLabel->setText(tr("Select different files to compare."));
		return;
	}
	// создаем состояния, в которые будем считывать, а также переменные с именами файлов и флаг ошибки
	cDataState State1, State2;
	QString FileName1 = "", FileName2 = "";
	ErrCode ErrFlag;

	// запоминаем текущее число высотных слоев
	cDataState::rememberNumLayers();
	// ставим число слоев = 1
	cDataState::reduceNumLayers();

	// считываем первое имя файла
	FileName1 = ui.File1lineEdit->text();
	// пытаемся считать первое состояние
    ErrFlag = State1.ReadDataFromFile(FileName1);
	//проверка на отсутствие ошибок
    if (!errCheck(ErrFlag, FileName1)) return;

	// считываем второе имя файла
	FileName2 = ui.File2lineEdit->text();
	// пытаемся считать второе состояние
    ErrFlag = State2.ReadDataFromFile(FileName2);
	//проверка на отсутствие ошибок
    if (!errCheck(ErrFlag, FileName2)) return;
	
	// если все хорошо - запоминаем имена файлов со считанными состояниями
	setState1FileName(FileName1.mid(FileName1.lastIndexOf('/')+1));
	setState2FileName(FileName2.mid(FileName2.lastIndexOf('/')+1));
	/*
		если картограмма активна - меняем состояния
		если картограмма не активна - создаем ее, ставим аттрибут - удалить при закрытии - и выводим на экран
		при создании картограммы, передаем её в конструктор параметры экрана
	*/
	if (emit isComparisonFieldActive())
	{
		emit ChangeStatesFB(State1, State2);
	} else {
		QDesktopWidget Desktop;
        ComparisonField *w = new ComparisonField(NULL,State1,State2,Desktop.screenGeometry(this));
		w->setAttribute(Qt::WA_DeleteOnClose);	
		w->show();
	}
	// закрываем обозреватель
	this->close();
}

// обновление статус-бара
void FileBrowser::CheckingFileNames(void) 
{
	// если оба LineEdit-а пусты
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
        statusLabel->setText(tr("Enter the names of the files with reactor states you want to compare."));
		return;
	}
	// если пуст только первый LineEdit
	if (ui.File1lineEdit->text().isEmpty())
	{
        statusLabel->setText(tr("Enter the name of the file with the first state into the line \"First state\"."));
		return;
	}
	// если пуст только второй LineEdit
	if (ui.File2lineEdit->text().isEmpty())
	{
        statusLabel->setText(tr("Enter the name of the file with the second state into the line \"Second state\"."));
		return;
	}
	// если в LineEdit-ах одинаковые названия файлов
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
        statusLabel->setText(tr("Select different files to compare."));
		return;
	}
	// ничего из вышеперечисленного
    statusLabel->setText(tr("Click the button \"Compare\" to compare states."));
}

// оповещение картограммы о закрытии обозревателя файлов
void FileBrowser::closeEvent(QCloseEvent *)
{
	emit closing();
}


void FileBrowser::on_English_triggered()
{
    ui.English->setChecked(true);
    ui.Russian->setChecked(false);
    changeAppLanguage(Lang::EN);
}

void FileBrowser::on_Russian_triggered()
{
    ui.English->setChecked(false);
    ui.Russian->setChecked(true);
    changeAppLanguage(Lang::RU);
}

void FileBrowser::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui.textBrowser->append(tr("The language is changed to %1. Subsequent messages will be displayed using this language.")
                               .arg(LangToQString(langData.Lang)));
        CheckingFileNames();
        ui.retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}
