#ifndef DEMOAREACF_H
#define DEMOAREACF_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QPair>
#include "fa_box.h"

// ���������������� ���� ��� ��������� ����������� ���
class demoAreaCF : public QFrame
{
	Q_OBJECT

	
public:
	// ������������
	demoAreaCF(QWidget *parent);
	~demoAreaCF();

signals:
	// �������� ����, � ������� ��������� ���
	QColor getColor();

protected:
	// ������������ ����
	void paintEvent(QPaintEvent *) override;

private slots:
	// �������� ��������
	void getStates(const QPair<double, double>& States);

private:
	// ������� ����
	QColor color;
	// ���������������� ���
	cFA_Box demoFA;
};

#endif // DEMOAREACF_H
