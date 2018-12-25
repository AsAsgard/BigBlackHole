#ifndef KV_DISTRIBUTION_H
#define KV_DISTRIBUTION_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPair>
#include <qcustomplot.h>
#include "extfunctions.h"
#include "ui_kv_distribution.h"
#include "kv_settings.h"
#include "renderarea.h"

//����� ����� - ����������� ������������� Kv
class Kv_Distribution : public QMainWindow
{
	Q_OBJECT


public:
	// ����������� � ����������
	Kv_Distribution(const QPen& oldState1Pen, const QPen& oldState2Pen, const AutoAxis::AutoAxisEnum& oldAutoAxisSetting, QWidget *parent = 0);
	~Kv_Distribution();

	// �������� �������� ������ ���, ��������� �� �����������
	int currentFA() const { return CurrentFA;}

signals:
	// �������� ����������� � ��������� ������� ���������� ���������
	void closing(const QPen& State1Pen, const QPen& State2Pen, const AutoAxis::AutoAxisEnum& AutoAxisSetting);\
	// �������� �������� �������� ��������� ����
	int getSpinBoxValue();

protected:
	void closeEvent(QCloseEvent *) override;
	void resizeEvent(QResizeEvent * res) override;

private slots:
	//����������� ��������� ���� �� ������� - ��������� ��� �����
	void MouseOnPlot(QMouseEvent * mouse);
	// ������� �������� ����������
	void on_Screenshot_triggered();
	// ���� �������� �����������
	void on_Settings_triggered();
	// ���� �������� ����������� ���� ������� 
	void SetterClosed();
	// ��������� ������� ��������� ���������
	QPair<QPen, QPen> sendCurrentPens();
	// ��������� ��������� ��������� �� ���������
	QPair<QPen, QPen> sendDefaultPens();
	// ��������� ������� ������������� ���
	AutoAxis::AutoAxisEnum sendCurrentAutoAxisSetting();
	// ��������� ����� ���������� ��������� � �������������
	void getNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting);
	// ����� ��� ������� � ���� �����������
	void FA_selected(int FA_Number);
	// ���������� ������ ����� ����
	void LayerLineHiding() {ui.plotArea->graph(2)->data()->clear(); ui.plotArea->replot();}
	// ���������� ����������� ��������� ���
	void ResetAxisNeededTrue() { AxisSettingNeeded = true;}
	// �������� ��������� �� ���� ���������
	void getRenderAreaPointer(RenderArea *);

private:
	// ����
	Ui::Kv_DistributionClass ui;
	// ������� ���
	int CurrentFA;
	// Layout ������� �� �������
	QScopedPointer<QCPLayoutGrid> LegendLayout;
	// ��������� ������� �� �������
	QScopedPointer<QCPTextElement> FA_Num;
	// ���� ���������� ��������� �����������
	QScopedPointer<KV_Settings> Setter;
	// ��������� �� ���� ���������
	RenderArea * renderArea;
	// ��������� ��������� �� ���������
	QPen defaultState1Pen;
	QPen defaultState2Pen;
	// ������� ��������� ���������
	QPen State1Pen;
	QPen State2Pen;
	// ������� �������� ������������� ����
	AutoAxis::AutoAxisEnum AutoAxisSetting;
	// ������������� �����������
	bool AxisSettingNeeded;
	// ������
	// ���������� ����� �������� ������ ���
	void SetFAnum(const int& newFA_num) { CurrentFA = newFA_num; FA_Num->setText(QString::fromStdWString(L"��� " + ExtFunctions::to_wstring(newFA_num)));}
	// ������� ���������� ��� X
	void XAxisSetting(void);
};

#endif // KV_DISTRIBUTION_H
