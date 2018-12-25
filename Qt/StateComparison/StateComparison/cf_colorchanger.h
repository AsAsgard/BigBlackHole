#ifndef CF_COLORCHANGER_H
#define CF_COLORCHANGER_H

#include <QWidget>
#include "fa_box.h"
#include "ui_cf_colorchanger.h"

// ����� ����� - ���� ��������� ���� �����������
class CF_ColorChanger : public QWidget
{
	Q_OBJECT

public:
	// ����������� � ����������
	CF_ColorChanger(QWidget *parent = 0);
	~CF_ColorChanger();

signals:
	// �������� ����
	void closing();
	// �������� ����� ���� �����������
	void RecolorComparisonField();
	// �������� ��������������� ��������
	void changeActiveMode(const Parameters::ParametersEnum &);
	// ������������ ���������������� ���
	void sendDemoStates(const QPair<double,double>& demoStates);

protected:
	void closeEvent(QCloseEvent *) override;

private slots:
	// ����� ColorDialog-�� � ���������� � ��� ������
	void on_ChangeMaxColor_clicked();
	void on_ChangeMinColor_clicked();

	// ������� �� ������ �����
	void on_buttonBox_clicked(QAbstractButton * button);
	
	// ����� ������
	void on_KqButton_clicked();
	void on_BurnButton_clicked();
	void on_KvButton_clicked();

private:
	// ����
	Ui::CF_ColorChanger ui;
	// �������� ��������
	Parameters::ParametersEnum activeMode;

	// ��������������� ���������
	struct LimColors {
		QColor maxColor;
		QColor minColor;
	};

	// ����� �����
	std::map<Parameters::ParametersEnum, LimColors> newLimitColors;

	// ������
	// ��������� ����� � �����������
	void applyColors();
	// ��������� ����� � ����������������� ����
	void changeColorField();
};

#endif // CF_COLORCHANGER_H
