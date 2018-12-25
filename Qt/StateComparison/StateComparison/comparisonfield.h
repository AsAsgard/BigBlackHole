#ifndef COMPARISONFIELD_H
#define COMPARISONFIELD_H

#include <QLabel>
#include <QScopedPointer>
#include <QMainWindow>
#include "ui_comparisonfield.h"
#include "kv_distribution.h"
#include "renderarea.h"
#include "filebrowser.h"
#include "cf_colorchanger.h"

// ����� ����� - �������� ���� � ������������
class ComparisonField : public QMainWindow
{
	Q_OBJECT

public:
	// ����������� � ����������
	explicit ComparisonField(QWidget *parent, const cDataState &rState1, const cDataState &rState2,const QRect &ScreenParameters);
	~ComparisonField();

signals:
	// �������� ����� ���������
	void ViewModeChange(bool);
	// �������� ���������
	void ChangeStatesCF(const cDataState& State1, const cDataState& State2);
	// ����� ����� ��� ��� ����������� Kv
	void select_FA(int);
	// ���������� ���������� ��������� ��� X ����������� Kv
	void resetAxisNeeded();
	// ���������� �������� ����� ���� �� ����������� Kv
	void hideLayerLine();
	// ��������� ��������� �� ���� ���������
	void sendRenderAreaPointer(RenderArea *);

protected:
	void resizeEvent(QResizeEvent * res) override;

private slots:
	// ������� ������ ���������
	void on_otherStates_triggered();
	// ������������ ����������� Kv
	void on_Kv_diagramActivator_triggered();
	// ������� ����� �����������
	void on_Screenshot_triggered();
	// ����� ������ ��������� ���������
	void on_activateKq_triggered();
	void on_activateBurn_triggered();
	void on_activateKv_triggered();
	// �������� ������������ ����������
	void on_MaxShow_triggered();
	// ������� ���� ��������� �����������
	void on_ChangeColors_triggered();
	// �������� ����� ���������
	void on_ChangeViewMode_toggled(bool checked);
	// ��������/��������� �����
	void on_ColorFont_toggled(bool checked);
	// ��������� �������� ���� Kv
	void on_spinBox_valueChanged(int newValue);
	// ����� �� ������ ���������� ����������� Kv
	bool KvDiagramAvaliability() const;
	// ����������� Kv ���� �������
	void KvDiagramClosed(const QPen& Kv1PenToSave, const QPen& Kv2PenToSave, const AutoAxis::AutoAxisEnum& KvAutoAxisToSave);
	// ���� ��������� ����������� ���� �������
	void ColorChangerClosed();
	// ���� ������ ������ ��������� ���� �������
	void FileBrowserClosed();
	// ����� �� ������ ���������� ���� � ������������
	bool ComparisonFieldAvaliability() const;
	// ������ ������ � ���������� ���������
	void StatesChanged(const cDataState &rState1, const cDataState &rState2);
	// ����������� ���� ���������
	void RecolorAll();

private:
	// ����
	Ui::ComparisonFieldClass ui;
	// ����� ����������� Kv
	QScopedPointer<Kv_Distribution> KvDistrib;
	// ������� ���������
	QScopedPointer<RenderArea> renderArea;
	// ������������� Label � ������-���
	QScopedPointer<QLabel> statusLabel;
	// ����� ��������� ���������� �����������
	QScopedPointer<CF_ColorChanger> ColorChanger;
	// ����� ������ ���������
	QScopedPointer<FileBrowser> fileBrowser;

	// ����������� ��������� ��������� ��� ����������� Kv
	QPen Kv1SavedPen;
	QPen Kv2SavedPen;
	AutoAxis::AutoAxisEnum KvSavedAutoAxis;

	// ������
	// �������� ������-���
	void StatusBarUpdate(void);
};

#endif // COMPARISONFIELD_H
