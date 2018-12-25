#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include "cdatastate.h"
#include "fa_box.h"

// ����� ������� ��������� �� �������
class RenderArea : public QWidget
{
    Q_OBJECT

public:
	// ����������� � ����������
	RenderArea(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QSize &rSize);
	~RenderArea();

	// ������ ������ �� ����������
	const cDataState& deltaState() const { return DeltaState;}
	const cDataState& state1() const { return State1;}
	const cDataState& state2() const { return State2;}
	// ������ �������� Kv
	const double& maxKv() const {return MaxKv;}
	// �������� ������������ ������
	void ChangeText(Parameters::ParametersEnum newActiveMode, int LayerValue);

signals:
	// ������� ��� ��� �����������
	void select_FA(int FA_Number);
	// ������ ���������� ����������� Kv
	bool isKvDiagramActive();

protected:
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent * res) override;
	// ������� ���� �� ���� - ���������� ��� ������ ��� �� ����������� Kv
	void mousePressEvent(QMouseEvent * mouse) override;

private slots:
	// ��������� ������ ����������� (�������� ��������� ��� ��� ���������)
	void ViewModeChanged(bool DeltaChecked);
	// �������� ����� ���������
	void StatesBinding(const cDataState &rState1, const cDataState &rState2);

private:
	// ����
	// ��������� � �� ��������
	cDataState State1;
	cDataState State2;
	cDataState DeltaState;
	// ������������ �������� Kv
	double MaxKv;
	// ��� ��� �� �����������
	std::vector<cFA_Box> FA;
	// ��� � ��������
	cFA_Box titleFA;

	// ��������������� ���������
	struct Geom {
		double x_center;
		double y_center;
		double width;
		double height;
	};
	// ��������� ���
	std::vector<Geom> FA_geometry;
	//������
	// ����������� ��������� ���
	void RecalculateGeometry();
};


#endif // RENDERAREA_H