#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QLabel>
#include <QScopedPointer>
#include <QtWidgets/QMainWindow>
#include "cdatastate.h"
#include "ui_filebrowser.h"

// класс формы - выбор файлов с состояниями для сравнения
class FileBrowser : public QMainWindow
{
	Q_OBJECT

public:
	// конструктор и деструктор
	FileBrowser(QWidget *parent = 0);
	~FileBrowser();

signals:
	// запрос активности поля картограммы
	bool isComparisonFieldActive();
	// изменить текущие состояния
	void ChangeStatesFB(const cDataState& State1, const cDataState& State2);
	// закрытие окна
	void closing();

protected:
	void closeEvent(QCloseEvent *) override;

private slots:
	// выбор файлов через обозреватель
	void on_File1Button_clicked();
	void on_File2Button_clicked();

	// кнопка сравнения
	void on_Compare_clicked();

	// добавление имени файлов вручную
	void on_File1lineEdit_editingFinished();
	void on_File1lineEdit_textEdited(QString);
	void on_File2lineEdit_editingFinished();
	void on_File2lineEdit_textEdited(QString);

private:
	//ПОЛЯ
	Ui::FileBrowserClass ui;
	// дополнительный Label для статус-бара
	QScopedPointer<QLabel> statusLabel;
	//МЕТОДЫ
	// проверить правильность входных данных (для статус-бара)
	void CheckingFileNames(void); 
};

#endif // FILEBROWSER_H
