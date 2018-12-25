#include <QDesktopWidget>
#include <QFileDialog>
#include <QString>
#include <QFont>
#include "filebrowser.h"
#include "comparisonfield.h"

using namespace std;

// реализация стандартного пути - его получение и задание
static QString _defaultPath = "./";
QString defaultPath() {return _defaultPath;}
void setDefaultPath(const QString &NewDefaultPath) { _defaultPath = NewDefaultPath;}
void setDefaultPath(QString &&NewDefaultPath) { if (&_defaultPath != &NewDefaultPath) _defaultPath = NewDefaultPath;}

// реализация имен файлов - их получение и задание
static QString _State1FileName = "", _State2FileName = "";
QString State1FileName() { return _State1FileName;}
void setState1FileName(const QString &NewState1FileName) { _State1FileName = NewState1FileName;}
void setState1FileName(QString &&NewState1FileName) { if (&_State1FileName != &NewState1FileName) _State1FileName = NewState1FileName;}
QString State2FileName() { return _State2FileName;}
void setState2FileName(const QString &NewState2FileName) { _State2FileName = NewState2FileName;}
void setState2FileName(QString &&NewState2FileName) { if (&_State2FileName != &NewState2FileName) _State2FileName = NewState2FileName;}


FileBrowser::FileBrowser(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// делаем окно модальным и фиксированного размера
	FileBrowser::setFixedSize(FileBrowser::size());
	FileBrowser::setWindowModality(Qt::ApplicationModal);

	// изменяем размеры статус-бара, создаем на нем Label, задаем его геометрию и выравнивание и вписываем текст
	ui.statusBar->resize(this->width(), 20);
	statusLabel.reset(new QLabel(ui.statusBar));
	statusLabel->setGeometry(4, 0, this->width(), ui.statusBar->height());
	statusLabel->setAlignment(Qt::AlignVCenter);
	statusLabel->setText(QString::fromStdWString(L"Введите названия файлов с состояниями, которые необходимо сравнить."));
	
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
FileBrowser::~FileBrowser() 
{
	// перестраховка - освобождение памяти (из-за того, что были проблемы с памятью при завершении приложения)
	statusLabel.reset(nullptr);
}

// обозреватель первого состояния
void FileBrowser::on_File1Button_clicked() 
{
	// обновляем статус-бар
	statusLabel->setText(QString::fromStdWString(L"Выберете файл с первым состоянием."));
	// вызываем диалог открытия файла
	QString FileName = QFileDialog::getOpenFileName(
		this,
		QString::fromStdWString(L"Выберете файл со первым состоянием."),
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
		ui.textBrowser->append(QString::fromStdWString(L"В графу \"Первое состояние\" помещен файл "+ FileName.toStdWString() + L"."));
	}
	// обновляем статус-бар
	CheckingFileNames();
}

// обозреватель второго состояния
void FileBrowser::on_File2Button_clicked() 
{
	// обновляем статус-бар
	statusLabel->setText(QString::fromStdWString(L"Выберете файл со вторым состоянием."));
	// вызываем диалог открытия файла
	QString FileName = QFileDialog::getOpenFileName(
		this,
		QString::fromStdWString(L"Выберете файл со вторым состоянием."),
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
		ui.textBrowser->append(QString::fromStdWString(L"В графу \"Второе состояние\" помещен файл "+ FileName.toStdWString() + L"."));
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
			ui.textBrowser->append(QString::fromStdWString(L"В графу \"Первое состояние\" помещен файл "+ FileName.toStdWString() + L"."));
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
			ui.textBrowser->append(QString::fromStdWString(L"В графу \"Второе состояние\" помещен файл "+ FileName.toStdWString() + L"."));
		}
		// обновляем статус-бар
		CheckingFileNames();
	}
}

// при редактировании имен файлов - происходит автоматическое обновление статус-бара
void FileBrowser::on_File2lineEdit_textEdited(QString) { CheckingFileNames();}


// нажата кнопка "Сравнить"
void FileBrowser::on_Compare_clicked()
{
	// начальная проверка - как при обновлении статус-бара, только с занесением данных также и в TextBrowser
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"Введите названия файлов с состояниями в графы \"Первое состояние\" и \"Второе состояние\"."));
		statusLabel->setText(QString::fromStdWString(L"Введите названия файлов с состояниями, которые необходимо сравнить."));
		return;
	}
	if (ui.File1lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"Введите название файла с первым состоянием в графу \"Первое состояние\"."));
		statusLabel->setText(QString::fromStdWString(L"Введите название файла с первым состоянием в графу \"Первое состояние\"."));
		return;
	}
	if (ui.File2lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"Введите название файла со вторым состоянием в графу \"Второе состояние\"."));
		statusLabel->setText(QString::fromStdWString(L"Введите название файла со вторым состоянием в графу \"Второе состояние\"."));
		return;
	}
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
		ui.textBrowser->append(QString::fromStdWString(L"Выберете для сравнения различные файлы."));
		statusLabel->setText(QString::fromStdWString(L"Необходимо выбрать различные файлы для сравнения."));
		return;
	}
	// создаем состояния, в которые будем считывать, а также переменные с именами файлов
	cDataState State1, State2;
	QString FileName = "", FileName1 = "", FileName2 = "";
	try {
		// запоминаем текущее число высотных слоев
		cDataState::rememberNumLayers();
		// ставим число слоев = 1
		cDataState::reduceNumLayers();
		// считываем первое имя файла
		FileName = FileName1 = ui.File1lineEdit->text();
		// пытаемся считать первое состояние
		State1.ReadDataFromFile(FileName.toStdWString());
		// считываем второе имя файла
		FileName = FileName2 = ui.File2lineEdit->text();
		// пытаемся считать второе состояние
		State2.ReadDataFromFile(FileName.toStdWString());
	} catch(const ios_base::failure &ex) {
		// выводим сообщение об ошибке
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"ОШИБКА!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"Проблема с файлом ")+FileName+QString::fromWCharArray(L". Проверьте правильность указанного пути."));
		statusLabel->setText(QString::fromWCharArray(L"Проверьте правильность пути файла с проблемой."));
		// возвращаем предыдущее число слоев и выходим из функции
		cDataState::setRememberedNumLayers();
		return;
	} catch(const invalid_argument &ex) {
		// выводим сообщение об ошибке
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"ОШИБКА!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"Проблема с файлом ")+FileName+QString::fromWCharArray(L". Проверьте правильность формата файла."));
		statusLabel->setText(QString::fromWCharArray(L"Проверьте правильность формата файла с проблемой."));
		// возвращаем предыдущее число слоев и выходим из функции
		cDataState::setRememberedNumLayers();
		return;
	} catch(const exception &ex) {
		// выводим сообщение об ошибке
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"НЕПРЕДВИДЕННАЯ ОШИБКА!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"Проблема с файлом ")+FileName);
		ui.textBrowser->append(QString::fromWCharArray(L"Попробуйте изменить файл и перезапустить программу. Если проблема останется, свяжитесь с разработчиком."));
		statusLabel->setText(QString::fromWCharArray(L"Попробуйте изменить файл с проблемой и перезапустить программу."));
		// возвращаем предыдущее число слоев и выходим из функции
		cDataState::setRememberedNumLayers();
		return;
	} catch(...) {
		// выводим сообщение об ошибке
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"НЕПРЕДВИДЕННАЯ ОШИБКА!"));
		ui.textBrowser->append(QString::fromWCharArray(L"Проблема с файлом ")+FileName);
		ui.textBrowser->append(QString::fromWCharArray(L"Попробуйте изменить файл и перезапустить программу. Если проблема останется, свяжитесь с разработчиком."));
		statusLabel->setText(QString::fromWCharArray(L"Попробуйте изменить файл с проблемой и перезапустить программу."));
		// возвращаем предыдущее число слоев и выходим из функции
		cDataState::setRememberedNumLayers();
		return;
	}
	// если все хорошо - запоминаем имена файлов со считанными состояниями
	setState1FileName(FileName1.mid(FileName.lastIndexOf('/')+1));
	setState2FileName(FileName2.mid(FileName.lastIndexOf('/')+1));
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
		ComparisonField *w = new ComparisonField(0,State1,State2,Desktop.screenGeometry(this));
		w->setAttribute(Qt::WA_DeleteOnClose);	
		w->show();
	}
	// закрываем обозреватель
	FileBrowser::close();
}

// обновление статус-бара
void FileBrowser::CheckingFileNames(void) 
{
	// если оба LineEdit-а пусты
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"Введите названия файлов с состояниями, которые необходимо сравнить."));
		return;
	}
	// если пуст только первый LineEdit
	if (ui.File1lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"Введите название файла с первым состоянием в графу \"Первое состояние\"."));
		return;
	}
	// если пуст только второй LineEdit
	if (ui.File2lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"Введите название файла со вторым состоянием в графу \"Второе состояние\"."));
		return;
	}
	// если в LineEdit-ах одинаковые названия файлов
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
		statusLabel->setText(QString::fromStdWString(L"Необходимо выбрать различные файлы для сравнения."));
		return;
	}
	// ничего из вышеперечисленного
	statusLabel->setText(QString::fromStdWString(L"Нажмите кнопку \"Сравнить\" для сравнения состояний."));
}

// оповещение картограммы о закрытии обозревателя файлов
void FileBrowser::closeEvent(QCloseEvent *)
{
	emit closing();
}