#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QLabel>
#include <QScopedPointer>
#include <QtWidgets/QMainWindow>
#include "cdatastate.h"
#include "ui_filebrowser.h"

// ����� ����� - ����� ������ � ����������� ��� ���������
class FileBrowser : public QMainWindow
{
	Q_OBJECT

public:
	// ����������� � ����������
	FileBrowser(QWidget *parent = 0);
	~FileBrowser();

signals:
	// ������ ���������� ���� �����������
	bool isComparisonFieldActive();
	// �������� ������� ���������
	void ChangeStatesFB(const cDataState& State1, const cDataState& State2);
	// �������� ����
	void closing();

protected:
	void closeEvent(QCloseEvent *) override;

private slots:
	// ����� ������ ����� ������������
	void on_File1Button_clicked();
	void on_File2Button_clicked();

	// ������ ���������
	void on_Compare_clicked();

	// ���������� ����� ������ �������
	void on_File1lineEdit_editingFinished();
	void on_File1lineEdit_textEdited(QString);
	void on_File2lineEdit_editingFinished();
	void on_File2lineEdit_textEdited(QString);

private:
	//����
	Ui::FileBrowserClass ui;
	// �������������� Label ��� ������-����
	QScopedPointer<QLabel> statusLabel;
	//������
	// ��������� ������������ ������� ������ (��� ������-����)
	void CheckingFileNames(void); 
};

// ��������/���������� ����� �� ��������� ��� �������� FileDialog
QString defaultPath();
void setDefaultPath(const QString &NewDefaultPath);
void setDefaultPath(QString &&NewDefaultPath);

// ��������/���������� ��� ����� � ������ ����������
QString State1FileName();
void setState1FileName(const QString &NewState1FileName);
void setState1FileName(QString &&NewState1FileName);

// ��������/���������� ��� ����� �� ������ ����������
QString State2FileName();
void setState2FileName(const QString &NewState2FileName);
void setState2FileName(QString &&NewState2FileName);

#endif // FILEBROWSER_H
