#include <algorithm>

#include "renderarea.h"
#include "comparisonfield.h"

using namespace std;

// �����������
RenderArea::RenderArea(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QSize &rSize) 
	: QWidget(parent), FA(cDataState::GetNumberFAs()), FA_geometry(cDataState::GetNumberFAs()),
	  titleFA(cFA_Box(true))
{
	// ����� ���
	setBackgroundRole(QPalette::Base);
    // ��������������
	setAutoFillBackground(true);
	// �������� �� �����
	setMouseTracking(true);
	// ������ ��������� � �����
	StatesBinding(rState1, rState2);
	// ������ ��������� ���
	RecalculateGeometry();
	// ������ ������� ����� - Kv, ������ ����
	ChangeText(Parameters::Kv, 1);
	// ��� ������ ��� ������ �������� ���������
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		FA[i].SetParent(this);
		FA[i].SetVisible(true);
		FA[i].SetFA_Number(i+1);
		FA[i].SetGeometry(FA_geometry[i].x_center, FA_geometry[i].y_center, FA_geometry[i].width, FA_geometry[i].height);
	}
	// ������ ��������� ���-�������
	titleFA.SetParent(this);
	titleFA.SetVisible(true);
}
 
// ����������
RenderArea::~RenderArea() {}

// ��������� ������� ���� ��� ���������
void RenderArea::resizeEvent(QResizeEvent * res)
{
	// ������������� ��������� ���
	RecalculateGeometry();
	// ������ ����� ��������� ��� ���� ���
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		FA[i].SetGeometry(FA_geometry[i].x_center, FA_geometry[i].y_center, FA_geometry[i].width, FA_geometry[i].height);
	}
}

// ����������� ����
void RenderArea::paintEvent(QPaintEvent *)
{
	// ������� ������ ���������
	QPainter painter(this);
	// ������ ����������� ��������� ��������� ���
	painter.setPen(QPen(Qt::black, 1));
	// ������ ������ ���
	for (int i = 0; i < cDataState::GetNumberFAs(); ++i)
	{
		// ���� ��� �����
		if (FA[i].isVisible()) 
		{
			// ������ ���� ��� � ������ ��
			painter.setBrush(FA[i].GetColor());
			painter.drawPath(FA[i].GetPainterPath());
		}
	}
	// ���� ����� ���-������� - ������ ��
	if (titleFA.isVisible())
	{
		painter.setBrush(titleFA.GetColor());
		painter.drawPath(titleFA.GetPainterPath());
	}
}

// ��������� ����� ������
void RenderArea::ViewModeChanged(bool DeltaChecked)
{
	/* 
		���� ������ ������ ������ �������� - ���������� ��,
		����� ��������� ��� ���������
	*/
	if (DeltaChecked) 
	{
		cFA_Box::SetViewMode(View::DeltaView);
	} else {
		cFA_Box::SetViewMode(View::TwoStatesView);
	}
	// ��������� ��������� ���� � ������ ���
	for_each(FA.begin(), FA.end(), [] (cFA_Box& obj)
	{
		obj.ChangeView();
	});
	// � � ���-�������
	titleFA.ChangeView();
}

// ������ ���������� ����� ����� ����� �������
#define LINE_TWO_POINTS(x, x1, y1, x2, y2) (y1) + ((y2)-(y1))*((x)-(x1))/((x2)-(x1)) 

// ������� ���� �� ���� 
void RenderArea::mousePressEvent(QMouseEvent * mouse)
{
	// ���� ���� ����������� Kv
	if (emit isKvDiagramActive()) 
	{
		// ��������� ���������� ����
		int x = mouse->x();
		int y = mouse->y();
		// ���� ��� � ������� ������ ����
		std::vector<cFA_Box>::iterator it = std::find_if(FA.begin(),FA.end(), [&] (const cFA_Box& fa)
		{
			return ( (x > (fa.x_center() - fa.width() / 2.0)) && 
					 (x < (fa.x_center() + fa.width() / 2.0)) && 
					 (y > LINE_TWO_POINTS(x, fa.x_center() - fa.width() / 2.0, fa.y_center() - fa.height() / 4.0, fa.x_center(), fa.y_center() - fa.height() / 2.0)) &&
					 (y > LINE_TWO_POINTS(x, fa.x_center(), fa.y_center() - fa.height() / 2.0, fa.x_center() + fa.width() / 2.0, fa.y_center() - fa.height() / 4.0)) &&
					 (y < LINE_TWO_POINTS(x, fa.x_center() + fa.width() / 2.0, fa.y_center() + fa.height() / 4.0, fa.x_center(), fa.y_center() + fa.height() / 2.0)) &&
					 (y < LINE_TWO_POINTS(x, fa.x_center(), fa.y_center() + fa.height() / 2.0, fa.x_center() - fa.width() / 2.0, fa.y_center() + fa.height() / 4.0)) );
		});
		// ���� ���� ������ � �����-�� ��� - ������ �������� ��� � �����������
		if (it != FA.end()) emit select_FA(it->GetFA_Number());
	}
}

/*
	������ ������� ��������� �����
	������, ������ ������
	��������� �� ������ ���� ����������� ���� �� ���������
	��������� ��������� ���������� ��� � ���� � �������� ��������� ���
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

// �������� ��������� ���
void RenderArea::RecalculateGeometry()
{
	// ������ � ������ - ����������������
	int FAwidth = (this->width() - 2*this->width()/50.0) / 14.0;
	int FAheight = (this->height() - 2*this->height()/30.0) / (8.0 + 7*0.5);

	// ��������� �� ������ - ���������������� + ��������� ���������� ��� ������������ ������ �������
	int FAy_center = this->height() - (this->height()/30.0 + FAheight / 2.0) + 3 * FAheight / 4;

	// ��������� �����
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

	// ��������� ���-�������
	titleFA.SetGeometry(0.03 * this->width() + 0.9 * FAwidth, this->height() - 0.03 * this->height() - 0.9 * FAheight, 1.8 * FAwidth, 1.8 * FAheight);
}

// ��������� ������
void RenderArea::ChangeText(Parameters::ParametersEnum newActiveMode, int LayerValue)
{
	// ������� ������ ��������� ��������
	cFA_Box::SetActiveMode(newActiveMode);
	// � ���������� �� ��������� ��������� ��������������� �����
	// ���� �������� NaN - ��������� ����� NaN
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

// ������� ���������
void RenderArea::StatesBinding(const cDataState& rState1, const cDataState& rState2)
{
	// �������� ��������� � ������� ����������
	State1 = rState1;
	State2 = rState2;
	// ���������� ���������
	std::tie(DeltaState,MaxKv) = CompareStates(State1, State2);
	double maxKq = 0;
	double maxBurn = 0;
	double maxKv = 0;
	// ���� ������������ �� ������ ���������� �������� ��������� - �� ����� ������� ������� ���
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
	// ������ ����� ���������, ���� ��� �� 0
	if(maxKq!=0)	cFA_Box::SetLimiters(Parameters::Kq, maxKq);
	if(maxBurn!=0)	cFA_Box::SetLimiters(Parameters::Burn, maxBurn);
	if(maxKv!=0)	cFA_Box::SetLimiters(Parameters::Kv, maxKv);
}