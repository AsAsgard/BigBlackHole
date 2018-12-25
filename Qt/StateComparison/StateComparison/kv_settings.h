#ifndef KV_SETTINGS_H
#define KV_SETTINGS_H

#include <QPair>
#include <QWidget>
#include <QPen>
#include "ui_kv_settings.h"

// ������������ ��������� �������������� ��������� ��� X
namespace AutoAxis {
	enum AutoAxisEnum {
		NoAuto,			// ��� �������������
		HighBorderAuto,	// ������������� ������ �� ������� �������
		AllAuto			// ������������� �� ������� � ������ ��������
	};
}

// ����� ����� - ��������� ���������� ����������� Kv
class KV_Settings : public QWidget
{
	Q_OBJECT

public:
	// ����������� � ����������
	KV_Settings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting, QWidget *parent = 0);
	~KV_Settings();

signals:
	// �������� ����
	void closing();
	// �������� ������� ��������� ���������
	QPair<QPen, QPen> getCurrentPens();
	// �������� ��������� ��������� �� ���������
	QPair<QPen, QPen> getDefaultPens();
	// �������� ������� ����� ������������� ���
	AutoAxis::AutoAxisEnum getCurrentAutoAxisSetting();
	// ��������� ����� ��������� ���������
	void sendNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting);

protected:
	void closeEvent(QCloseEvent *) override;

private slots:
	// ������� �� ������ ����� �����
	void on_buttonBox_clicked(QAbstractButton * button);
	// ����� ������ �����
	void on_State1lineStyleBox_currentIndexChanged(int index);
	void on_State2lineStyleBox_currentIndexChanged(int index);
	// ����� ������� �����
	void on_State1WidthBox_valueChanged(double value);
	void on_State2WidthBox_valueChanged(double value);
	// ����� ����� �����
	void on_State1ColorChange_clicked();
	void on_State2ColorChange_clicked();
	// ����� ������ ������������� ��� X �����������
	void on_NoAuto_toggled(bool checked);
	void on_HighBorderAuto_toggled(bool checked);
	void on_AllAuto_toggled(bool checked);

private:
	// ����
	Ui::KV_Settings ui;
	// ��������� ��������� �����������
	QPen newState1Pen;
	QPen newState2Pen;
	AutoAxis::AutoAxisEnum newAutoAxisSetting;

	// ������
	// ��������� ���������
	void applySettings();
	// ���������� ������ ��������� �� ��������
	void showParameters(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting);
};

#endif // KV_SETTINGS_H
