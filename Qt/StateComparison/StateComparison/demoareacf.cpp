#include "demoareacf.h"

// ����������� - ���� �� ������, ������ �������� ��������� ���������������� ���
demoAreaCF::demoAreaCF(QWidget *parent)
	: QFrame(parent), color(), demoFA() 
{
	demoFA.SetParent(this);
	demoFA.SetVisible(true);
	demoFA.SetFA_Number(135);
}

// ����������
demoAreaCF::~demoAreaCF() {}

// �������� ����� ��������
void demoAreaCF::getStates(const QPair<double, double>& States)
{
	// ������ �������� 1
	demoFA.SetState1(States.first);
	// ������ �������� 2
	demoFA.SetState2(States.second);
	// ������ ������� ��������
	demoFA.SetDelta(States.first - States.second);
}

// �������������� �������
void demoAreaCF::paintEvent(QPaintEvent *)
{
	// ������ ��������� ���
	demoFA.SetGeometry(this->width() / 2, this->height() / 2, 9 * this->height() / 10, 9 * this->height() / 10);
	// �������� ���� ���������
	color = emit getColor();
	// �������� ������ ���������
	QPainter painter(this);
	// ������ ����������� ��� ���������
	painter.setPen(QPen(Qt::black, 1));
	// ���������� ���������� ����
	painter.setBrush(color);
	// ������ ���
	painter.drawPath(demoFA.GetPainterPath());
}
