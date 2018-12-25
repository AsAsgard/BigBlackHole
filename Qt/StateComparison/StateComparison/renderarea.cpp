#include <algorithm>

#include "renderarea.h"
#include "comparisonfield.h"

using namespace std;

// конструктор
RenderArea::RenderArea(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QSize &rSize) 
	: QWidget(parent), FA(cDataState::GetNumberFAs()), FA_geometry(cDataState::GetNumberFAs()),
	  titleFA(cFA_Box(true))
{
	// белый фон
	setBackgroundRole(QPalette::Base);
    // автозаполнение
	setAutoFillBackground(true);
	// слежение за мышью
	setMouseTracking(true);
	// задаем состояния и цвета
	StatesBinding(rState1, rState2);
	// задаем геометрию ТВС
	RecalculateGeometry();
	// задаем бызовый текст - Kv, первый слой
	ChangeText(Parameters::Kv, 1);
	// для каждой ТВС задаем основные параметры
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		FA[i].SetParent(this);
		FA[i].SetVisible(true);
		FA[i].SetFA_Number(i+1);
		FA[i].SetGeometry(FA_geometry[i].x_center, FA_geometry[i].y_center, FA_geometry[i].width, FA_geometry[i].height);
	}
	// задаем параметры ТВС-легенды
	titleFA.SetParent(this);
	titleFA.SetVisible(true);
}
 
// деструктор
RenderArea::~RenderArea() {}

// изменение размера поля для рисования
void RenderArea::resizeEvent(QResizeEvent * res)
{
	// пересчитываем геометрию ТВС
	RecalculateGeometry();
	// задаем новую геометрию для всех ТВС
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		FA[i].SetGeometry(FA_geometry[i].x_center, FA_geometry[i].y_center, FA_geometry[i].width, FA_geometry[i].height);
	}
}

// перерисовка поля
void RenderArea::paintEvent(QPaintEvent *)
{
	// создаем объект рисования
	QPainter painter(this);
	// задаем стандартные параметры рисования ТВС
	painter.setPen(QPen(Qt::black, 1));
	// рисуем каждую ТВС
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		// если она видна
		if (FA[i].isVisible()) 
		{
			// задаем цвет ТВС и рисуем ее
			painter.setBrush(FA[i].GetColor());
			painter.drawPath(FA[i].GetPainterPath());
		}
	}
	// если видна ТВС-легенда - рисуем ее
	if (titleFA.isVisible())
	{
		painter.setBrush(titleFA.GetColor());
		painter.drawPath(titleFA.GetPainterPath());
	}
}

// изменился режим показа
void RenderArea::ViewModeChanged(bool DeltaChecked)
{
	/* 
		если нажата кнопка показа разности - показываем ее,
		иначе показывам два состояния
	*/
	if (DeltaChecked) 
	{
		cFA_Box::SetViewMode(View::DeltaView);
	} else {
		cFA_Box::SetViewMode(View::TwoStatesView);
	}
	// применяем изменение вида к каждой ТВС
	for_each(FA.begin(), FA.end(), [] (cFA_Box& obj)
	{
		obj.ChangeView();
	});
	// и к ТВС-легенде
	titleFA.ChangeView();
}

// макрос вычисления линии между двумя точками
#define LINE_TWO_POINTS(x, x1, y1, x2, y2) (y1) + ((y2)-(y1))*((x)-(x1))/((x2)-(x1)) 

// нажатие мыши на поле 
void RenderArea::mousePressEvent(QMouseEvent * mouse)
{
	// если есть гистограмма Kv
	if (emit isKvDiagramActive()) 
	{
		// считываем координаты мыши
		int x = mouse->x();
		int y = mouse->y();
		// ищем ТВС в которую попала мышь
		std::vector<cFA_Box>::iterator it = std::find_if(FA.begin(),FA.end(), [&] (const cFA_Box& fa)
		{
			return ( (x > (fa.x_center() - fa.width() / 2.0)) && 
					 (x < (fa.x_center() + fa.width() / 2.0)) && 
					 (y > LINE_TWO_POINTS(x, fa.x_center() - fa.width() / 2.0, fa.y_center() - fa.height() / 4.0, fa.x_center(), fa.y_center() - fa.height() / 2.0)) &&
					 (y > LINE_TWO_POINTS(x, fa.x_center(), fa.y_center() - fa.height() / 2.0, fa.x_center() + fa.width() / 2.0, fa.y_center() - fa.height() / 4.0)) &&
					 (y < LINE_TWO_POINTS(x, fa.x_center() + fa.width() / 2.0, fa.y_center() + fa.height() / 4.0, fa.x_center(), fa.y_center() + fa.height() / 2.0)) &&
					 (y < LINE_TWO_POINTS(x, fa.x_center(), fa.y_center() + fa.height() / 2.0, fa.x_center() - fa.width() / 2.0, fa.y_center() + fa.height() / 4.0)) );
		});
		// если мышь попала в какую-то ТВС - меняем значение ТВС у гистограммы
		if (it != FA.end()) emit select_FA(it->GetFA_Number());
	}
}

/*
	макрос задания геометрии ТВСок
	Высота, ширина заданы
	Положение по высоте ниже предыдущего ряда на константу
	Находится положение конкретной ТВС в ряду и задается геометрия ТВС
*/
#define LINE_GEOMETRY(FAsInRow, FirstFAinRow) {								\
	FAy_center = FAy_center - 3 * FAheight / 4;								\
	int FAx_center = this->width() / 2.0 - (FAsInRow - 1) / 2.0 * FAwidth;	\
	for (int i = 0; i < FAsInRow; ++i)										\
	{																		\
		FA_geometry[FirstFAinRow-1+i].width = FAwidth + 1;					\
		FA_geometry[FirstFAinRow-1+i].height = FAheight + 1;				\
		FA_geometry[FirstFAinRow-1+i].x_center = FAx_center + i * FAwidth;	\
		FA_geometry[FirstFAinRow-1+i].y_center = FAy_center;				\
	}																		\
} 

// пересчет геометрии ТВС
void RenderArea::RecalculateGeometry()
{
	// ширина и высота - экспериментально
	int FAwidth = (this->width() - 2*this->width()/50.0) / 14.0;
	int FAheight = (this->height() - 2*this->height()/30.0) / (8.0 + 7*0.5);

	// положение по высоте - экспериментально + начальное отклонение для правильности работы макроса
	int FAy_center = this->height() - (this->height()/30.0 + FAheight / 2.0) + 3 * FAheight / 4;

	// геометрия рядов
	LINE_GEOMETRY( 6, 1);
	LINE_GEOMETRY( 9, 7);
	LINE_GEOMETRY( 10, 16);
	LINE_GEOMETRY( 11, 26);
	LINE_GEOMETRY( 12, 37);
	LINE_GEOMETRY( 13, 49);
	LINE_GEOMETRY( 14, 62);
	LINE_GEOMETRY( 13, 76);
	LINE_GEOMETRY( 14, 89);
	LINE_GEOMETRY( 13, 103);
	LINE_GEOMETRY( 12, 116);
	LINE_GEOMETRY( 11, 128);
	LINE_GEOMETRY( 10, 139);
	LINE_GEOMETRY( 9, 149);
	LINE_GEOMETRY( 6, 158);

	// геометрия ТВС-легенды
	titleFA.SetGeometry(0.03 * this->width() + 0.9 * FAwidth, this->height() - 0.03 * this->height() - 0.9 * FAheight, 1.8 * FAwidth, 1.8 * FAheight);
}

// изменение текста
void RenderArea::ChangeText(Parameters::ParametersEnum newActiveMode, int LayerValue)
{
	// задание нового активного параметр
	cFA_Box::SetActiveMode(newActiveMode);
	// в зависимоти от параметра выводится соответствующий текст
	// если значение NaN - выводится текст NaN
	switch(newActiveMode) 
	{
		case(Parameters::Kq) :
			for (int i = 0; i < cDataState::GetNumberFAs(); ++i) 
			{
				if (!qIsNaN(DeltaState[i+1].GetKq())) { FA[i].SetDelta(DeltaState[i+1].GetKq());}
				else { FA[i].SetDelta("NaN");}
				if (!qIsNaN(State1[i+1].GetKq())) { FA[i].SetState1(State1[i+1].GetKq());}
				else { FA[i].SetState1("NaN");}
				if (!qIsNaN(State2[i+1].GetKq())) { FA[i].SetState2(State2[i+1].GetKq());}
				else { FA[i].SetState2("NaN");}
			}
			break;
		case(Parameters::Burn) :
			for (int i = 0; i < cDataState::GetNumberFAs(); ++i) 
			{
				if (!qIsNaN(DeltaState[i+1].GetBurn())) { FA[i].SetDelta(DeltaState[i+1].GetBurn());}
				else { FA[i].SetDelta("NaN");}
				if (!qIsNaN(State1[i+1].GetBurn())) { FA[i].SetState1(State1[i+1].GetBurn());}
				else { FA[i].SetState1("NaN");}
				if (!qIsNaN(State2[i+1].GetBurn())) { FA[i].SetState2(State2[i+1].GetBurn());}
				else { FA[i].SetState2("NaN");}
			}
			break;
		case(Parameters::Kv) :
			for (int i = 0; i < cDataState::GetNumberFAs(); ++i) 
			{
				if (!qIsNaN(DeltaState[i+1].GetKv(LayerValue))) { FA[i].SetDelta(DeltaState[i+1].GetKv(LayerValue));}
				else { FA[i].SetDelta("NaN");}
				if (!qIsNaN(State1[i+1].GetKv(LayerValue))) { FA[i].SetState1(State1[i+1].GetKv(LayerValue));}
				else { FA[i].SetState1("NaN");}
				if (!qIsNaN(State2[i+1].GetKv(LayerValue))) { FA[i].SetState2(State2[i+1].GetKv(LayerValue));}
				else { FA[i].SetState2("NaN");}
			}
			break;
	}
}

// задание состояний
void RenderArea::StatesBinding(const cDataState& rState1, const cDataState& rState2)
{
	// копируем состояния в местные переменные
	State1 = rState1;
	State2 = rState2;
	// сравниваем состояния
	std::tie(DeltaState,MaxKv) = CompareStates(State1, State2);
	double maxKq = 0;
	double maxBurn = 0;
	double maxKv = 0;
	// ищем максимальное по модулю отклонение значений состояний - от этого зависит окраска ТВС
	for (int i = 1; i <= cDataState::GetNumberFAs(); ++i)
	{
		if (!qIsNaN(DeltaState[i].GetKq()) && abs(DeltaState[i].GetKq()) > maxKq)
		{
			maxKq = abs(DeltaState[i].GetKq());
		}
		if (!qIsNaN(DeltaState[i].GetBurn()) && abs(DeltaState[i].GetBurn()) > maxBurn)
		{
			maxBurn = abs(DeltaState[i].GetBurn());
		}
		for (int j = 1; j <= cDataState::GetNumLayers(); ++j)
		{
			if (!qIsNaN(DeltaState[i].GetKv(j)) && abs(DeltaState[i].GetKv(j)) > maxKv)
			{
				maxKv = abs(DeltaState[i].GetKv(j));
				cFA_Box::FAwithMaxDeltaKv.FA = i;
				cFA_Box::FAwithMaxDeltaKv.Layer = j;
			}
		}
	}
	// задаем новые максимумы, если они не 0
	if(maxKq!=0)	cFA_Box::SetLimiters(Parameters::Kq, maxKq);
	if(maxBurn!=0)	cFA_Box::SetLimiters(Parameters::Burn, maxBurn);
	if(maxKv!=0)	cFA_Box::SetLimiters(Parameters::Kv, maxKv);
}