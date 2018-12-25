#ifndef DEMOAREAKV_H
#define DEMOAREAKV_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>

// ���������������� ���� ��� ��������� ����������� Kv
class demoAreaKv : public QFrame
{
	Q_OBJECT

public:
	// ������������
	demoAreaKv(QWidget *parent);
	~demoAreaKv();

signals:
	// �������� ��������� ���������
	QPen getPen();

protected:
	// ������������ ����
	void paintEvent(QPaintEvent *) override;

private:
	// ��������� ���������
	QPen pen;
};

#endif // DEMOAREAKV_H
