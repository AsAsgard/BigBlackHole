#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include "cdatastate.h"
#include "fa_box.h"

// класс области рисования на виджете
class RenderArea : public QWidget
{
    Q_OBJECT

public:
	// конструктор и деструктор
	RenderArea(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QSize &rSize);
	~RenderArea();

	// отдать данные по состояниям
	const cDataState& deltaState() const { return DeltaState;}
	const cDataState& state1() const { return State1;}
	const cDataState& state2() const { return State2;}
	// отдать максимум Kv
	const double& maxKv() const {return MaxKv;}
	// изменить отображаемые данные
	void ChangeText(Parameters::ParametersEnum newActiveMode, int LayerValue);

signals:
	// выбрать ТВС для гистограммы
	void select_FA(int FA_Number);
	// запрос активности гистограммы Kv
	bool isKvDiagramActive();

protected:
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent * res) override;
	// нажатие мыши на поле - необходимо для выбора ТВС на гистограмме Kv
	void mousePressEvent(QMouseEvent * mouse) override;

private slots:
	// изменение режима отображения (разность состояний или два состояния)
	void ViewModeChanged(bool DeltaChecked);
	// привязка новых состояний
	void StatesBinding(const cDataState &rState1, const cDataState &rState2);

private:
	// ПОЛЯ
	// состояния и их разность
	cDataState State1;
	cDataState State2;
	cDataState DeltaState;
	// максимальное значение Kv
	double MaxKv;
	// все ТВС на картограмме
	std::vector<cFA_Box> FA;
	// ТВС с легендой
	cFA_Box titleFA;

	// вспомогательная структура
	struct Geom {
		double x_center;
		double y_center;
		double width;
		double height;
	};
	// геометрия ТВС
	std::vector<Geom> FA_geometry;
	//МЕТОДЫ
	// пересчитать геометрию ТВС
	void RecalculateGeometry();
};


#endif // RENDERAREA_H