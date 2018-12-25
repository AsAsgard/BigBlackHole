#include "kv_distribution.h"
#include "filebrowser.h"
#include "fa_box.h"

Kv_Distribution::Kv_Distribution(const QPen& oldState1Pen, const QPen& oldState2Pen, const AutoAxis::AutoAxisEnum& oldAutoAxisSetting, QWidget *parent)
	: QMainWindow(parent),
	  // ����������� ��������� ���������
	  defaultState1Pen(Qt::red, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
	  defaultState2Pen(Qt::darkGreen, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
	  State1Pen(oldState1Pen),
	  State2Pen(oldState2Pen),
	  AutoAxisSetting(oldAutoAxisSetting), AxisSettingNeeded(true)
{
	ui.setupUi(this);
	// ������ ����������� �������� ������ ��� - ��� ������� ������������ ������� �������
	CurrentFA = 163;

	// ������ ����������� ���������, ���� ����� ����� (��� �������� ��������)
	if (State1Pen.color() == Qt::white && State2Pen.color() == Qt::white)
	{
		State1Pen = defaultState1Pen;
		State2Pen = defaultState2Pen;
		AutoAxisSetting = AutoAxis::NoAuto;
	}
	
	// ������ ����� ���, ������ � ����������� ������ ��� ����
	FA_Num.reset(new QCPTextElement(ui.plotArea));
	QFont font = ui.plotArea->xAxis->labelFont();
	font.setPointSize(font.pointSize() + 1);
	FA_Num->setFont(font);
	FA_Num->setText(QString::fromStdWString(L"���      " + ExtFunctions::to_wstring(CurrentFA)));
	FA_Num->setMinimumSize(FA_Num->rect().width(), FA_Num->rect().height());

	// ������������ ������� ������� � ������ ���
	// ������� ����� ������
	LegendLayout.reset(new QCPLayoutGrid);
	// ��������� ���� ������ ������ ������� - ��� ����� ��������
	ui.plotArea->plotLayout()->addElement(1, 0, LegendLayout.data());
	// ���� �������
	LegendLayout->setMargins(QMargins(5, 0, 5, 5));
	// ��������� ����� ��� � ������ �������
	LegendLayout->addElement(0, 0, FA_Num.data());
	// ����������� ������ ������ ���
	LegendLayout->element(0, 0)->setMinimumSize(FA_Num->rect().width(), FA_Num->minimumSize().height());
	// ��������� ������� �� ������ �������
	LegendLayout->addElement(0, 1, ui.plotArea->legend);
	// ����� ����� - 1% �������, �� ���� ����������� ������
	LegendLayout->setColumnStretchFactor(0, 1);
	// ������ - 99 % �������
    LegendLayout->setColumnStretchFactor(1, 99);
	// ����� ����������� ������ �� ������ ���
	ui.plotArea->plotLayout()->setRowStretchFactor(1, 0.001);

	// �������� �� �����
	ui.plotArea->setMouseTracking(true);
	// ��������� ������� �������� ���� � QCustomPlot � � �������� ���� - ������ ����� ������
	connect(ui.plotArea, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(MouseOnPlot(QMouseEvent *)));
	
	// ������ ������� ��������� ����������� �����������
	Setter.reset(nullptr);
	// ������ ������� ��������� ���� �����������
	renderArea = nullptr;

	// ������� ��� �������
	ui.plotArea->clearGraphs();
	// ������ �������� ����
	ui.plotArea->xAxis->setLabel("Kv");
	ui.plotArea->yAxis->setLabel(QString::fromWCharArray(L"H, %H��"));

	// ���������� ������� ��� Y
	ui.plotArea->yAxis->setRange(0, 100);
		
	// ������� ����� ����� �  ��������� ��� � ��� Y
	QSharedPointer<QCPAxisTickerFixed> yfixedTicker(new QCPAxisTickerFixed);
	ui.plotArea->yAxis->setTicker(yfixedTicker);
	// ������ ��� �������� ����� - ����� ������� ����� ������ ��������
	yfixedTicker->setTickStep(10);
	// ������ ���������� �������������� ����� - ����� ��������� �������
	yfixedTicker->setTickCount(4);
	// ��� ��������������
	yfixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);

	// ������ �������
	ui.plotArea->legend->setVisible(true);
	ui.plotArea->legend->setBrush(QBrush(QColor(255,255,255,150)));
	ui.plotArea->legend->setFont(ui.plotArea->xAxis->labelFont());
		
	// ����� ��� � ������������ �������� Kv
	FA_selected(cFA_Box::FAwithMaxDeltaKv.FA);
}

// ����������
Kv_Distribution::~Kv_Distribution() 
{
	// ������ � renderArea �� �����������! ��� �������� � ������� QScopedPointer � �������� ���� �����������!
	renderArea = nullptr;
	// ������������� - ������������ ������ (��-�� ����, ��� ���� �������� � ������� ��� ���������� ����������)
	FA_Num.reset(nullptr);
	Setter.reset(nullptr);
}

void Kv_Distribution::MouseOnPlot(QMouseEvent * mouse)
{
	// 3 � 4 ������� - ������������ � �������������� ����� ����� ������� ����
	// ������� ������ ��� 3 � 4 �������� 
	ui.plotArea->graph(3)->data()->clear();
	ui.plotArea->graph(4)->data()->clear();
	
	// �������� ������� ���� � ����������� �������
	double xPos = ui.plotArea->xAxis->pixelToCoord(mouse->pos().x());
	double yPos = ui.plotArea->yAxis->pixelToCoord(mouse->pos().y());
	// ���� ������� ������ ������ ����, �� ������������ �����
	if ( ui.plotArea->xAxis->range().upper >= xPos &&
		 ui.plotArea->xAxis->range().lower <= xPos &&
		 ui.plotArea->yAxis->range().upper >= yPos &&
		 ui.plotArea->xAxis->range().lower <= yPos )
	{
		QVector<double> xValues(2);
		QVector<double> yValues(2);

		// ������������ ����� - ������ ������
		xValues[0] = xPos;
		xValues[1] = xPos;
		yValues[0] = ui.plotArea->yAxis->range().lower;
		yValues[1] = ui.plotArea->yAxis->range().upper;
		ui.plotArea->graph(3)->setData(xValues,yValues,true);

		// �������������� ����� - ������ ������
		xValues[0] = ui.plotArea->xAxis->range().lower;
		xValues[1] = ui.plotArea->xAxis->range().upper;
		yValues[0] = yPos;
		yValues[1] = yPos;
		ui.plotArea->graph(4)->setData(xValues,yValues,true);
	}
	// ����������� ������
	ui.plotArea->replot();
}

// �������� �����������
void Kv_Distribution::on_Screenshot_triggered()
{
	// �������� ������ ���������� �����
	QString ScreenFileName = QFileDialog::getSaveFileName(this,
														  QString::fromStdWString(L"��������� ���..."),
														  defaultPath(),
														  tr("JPG Files (*.jpg);;PNG Files (*.png);;BMP Files (*.bmp);;All Files (*.*)")
														  );
	// ���� ��� ����� �������
	if (!ScreenFileName.isEmpty()) 
	{
		// ��������� ����������� ����
		setDefaultPath(ScreenFileName.left(ScreenFileName.lastIndexOf('/')+1));
		// ��������� �����������
		QPixmap screen = QPixmap::grabWidget(ui.centralWidget);
		// �������� �������� �������� - ���������� ������� ��� ������ ��������
		screen = screen.scaled(ui.centralWidget->width()*4,ui.centralWidget->height()*4,Qt::KeepAspectRatio,Qt::SmoothTransformation);
		// ��������� ������� � ����
		screen.save(ScreenFileName, 0, 100);
	}
}

// ��������� ������� ����������� - ������ ������ ������� ��������� �������
void Kv_Distribution::resizeEvent(QResizeEvent * res)
{
	ui.plotArea->resize(ui.centralWidget->width(), ui.centralWidget->height());
}

// ��������� ����������� � �������� ����������� � ��������� ������� �������� ���������� ��������� � ������������� ����
void Kv_Distribution::closeEvent(QCloseEvent *)
{
	emit closing(State1Pen, State2Pen, AutoAxisSetting);
}


// �������� ����������� �����������
void Kv_Distribution::on_Settings_triggered()
{
	// ���� ��������� �������
	if (!Setter)
	{
		// ������� ����� ���� � ������������ �������
		Setter.reset(new KV_Settings(State1Pen, State2Pen, AutoAxisSetting));
		// �������� �����������
		connect(Setter.data(), SIGNAL(closing()), this, SLOT(SetterClosed()));
		// �������� ������� �������� ���������
		connect(Setter.data(), SIGNAL(getCurrentPens()), this, SLOT(sendCurrentPens()));
		// �������� ����������� �������� ���������
		connect(Setter.data(), SIGNAL(getDefaultPens()), this, SLOT(sendDefaultPens()));
		// �������� �������� �������� ������������� ����
		connect(Setter.data(), SIGNAL(getCurrentAutoAxisSetting()), this, SLOT(sendCurrentAutoAxisSetting()));
		// �������� ����� ���������� ���������
		connect(Setter.data(), SIGNAL(sendNewSettings(const QPen&, const QPen&, AutoAxis::AutoAxisEnum)), this, SLOT(getNewSettings(const QPen&, const QPen&, AutoAxis::AutoAxisEnum)));
		// ���������� ����������
		Setter->show();
	}
}


// �������� ����������� �����������
void Kv_Distribution::SetterClosed()
{
	// ����������� ������ � ���������
	Setter.reset(nullptr);
}

// �������� ������� �������� ���������
QPair<QPen, QPen> Kv_Distribution::sendCurrentPens()
{
	return qMakePair(State1Pen, State2Pen);
}

// �������� ����������� �������� ���������
QPair<QPen, QPen> Kv_Distribution::sendDefaultPens()
{
	return qMakePair(defaultState1Pen, defaultState2Pen);
}

// �������� �������� �������� ������������� ����
AutoAxis::AutoAxisEnum Kv_Distribution::sendCurrentAutoAxisSetting()
{
	return AutoAxisSetting;
}

// �������� ����� �������� �������� ��������� � ������������� ���
void Kv_Distribution::getNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting)
{
	State1Pen = newState1Pen;
	State2Pen = newState2Pen;
	AutoAxisSetting = newAutoAxisSetting;
	// ���������� ������������� ���
	AxisSettingNeeded = true;
	// ����������� �����������
	FA_selected(CurrentFA);
}

// ��������� ��������� �� ���� ���������
void Kv_Distribution::getRenderAreaPointer(RenderArea * RA)
{
	// ������������ ���������
	renderArea = RA;
	// ����������� �����������
	FA_selected(CurrentFA);
}

// ������������� ��� X
void Kv_Distribution::XAxisSetting()
{
	// ��������, ���� ��� ����������
	if (AxisSettingNeeded)
	{
		//�������� ����� �������� � ������� ���
		double xMax = 0;
		double xMin = 0;

		// ���� ��� �������������
		if (AutoAxisSetting == AutoAxis::NoAuto)
		{
			// �� �������� ����� �������� ������ �� ����������� ��������� Kv ���������
			while(xMax < renderArea->maxKv()) 
			{
				// �������� ��� � ������������ ����� - ��� ���������� �������� �����
				// ���������� ����� ��������� ��-�� ���������� ���� double
				if (xMax >= 3 - 0.0000001) xMax += 1;
				if (xMax >= 2 - 0.0000001 && xMax < 3 - 0.0000001) xMax +=0.5;
				if (xMax >= 1 - 0.0000001 && xMax < 2 - 0.0000001) xMax += 0.25;
				if (xMax >= 0.6 - 0.0000001 && xMax < 1 - 0.0000001) xMax += 0.2;
				if (xMax < 0.6 - 0.0000001) xMax += 0.1;
			}
		// ���� ���� ������������� 
		} else if (AutoAxisSetting == AutoAxis::HighBorderAuto || AutoAxisSetting == AutoAxis::AllAuto) {
			// �� ���������� ������� Kv ��� ���������
			std::vector<double> State1Kv = renderArea->state1()[CurrentFA].GetKv();
			std::vector<double> State2Kv = renderArea->state2()[CurrentFA].GetKv();
			// �� ��� ����������� �������� NaN - ����������� � ����� ������� � ������������ �������� �� ����� ��� �����
			auto it1 = std::remove_if(State1Kv.begin(), State1Kv.end(), [] (const double& elem) { return qIsNaN(elem);});
			auto it2 = std::remove_if(State2Kv.begin(), State2Kv.end(), [] (const double& elem) { return qIsNaN(elem);});
			// ���������� �������� ���� ���������
			State1Kv.erase(it1, State1Kv.end());
			State2Kv.erase(it2, State2Kv.end());

			// ���� ��������� � �� ������ �������, � �� ������� 
			if (AutoAxisSetting == AutoAxis::AllAuto)
			{
				// ��������� ����������� �������� ���� �������� - �� ��� ������������ ��������
				auto State1minKv = std::min_element(State1Kv.begin(), State1Kv.end());
				auto State2minKv = std::min_element(State2Kv.begin(), State2Kv.end());
				double minKv = 0;
				/*
					���� ��������� ������� - ��������� ����������� �� �� �������� 
					���� �����-�� �������� �� ������� - ������� �������� �� �������
					���� ��� ��������� �� ������� - �������� �������� 0
				*/
				if (State1minKv != State1Kv.end() && State2minKv != State2Kv.end()) {
					minKv = std::min(*State1minKv, *State2minKv);
				} else if (State1minKv != State1Kv.end()) {
					minKv = *State1minKv;
				} else if (State2minKv != State2Kv.end()) {
					minKv = *State2minKv;
				}
				// ������� ����� ������� ��� � ����� 0.1
				while(xMin <= minKv)
				{
					xMin += 0.1;
				}
				xMin -= 0.1;
				if (xMin <= 0.0000001) xMin = 0;
			}

			// ��������� ������������ �������� ���� �������� - �� ��� ������������ ��������
			auto State1maxKv = std::max_element(State1Kv.begin(), State1Kv.end());
			auto State2maxKv = std::max_element(State2Kv.begin(), State2Kv.end());
			double maxKv = 0;
			/*
				���� ��������� ������� - ��������� ������������ �� �� �������� 
				���� �����-�� �������� �� ������� - ������� �������� �� �������
				���� ��� ��������� �� ������� - �������� �������� 0
			*/
			if (State1maxKv != State1Kv.end() && State2maxKv != State2Kv.end()) {
				maxKv = std::max(*State1maxKv, *State2maxKv);
			} else if (State1maxKv != State1Kv.end()) {
				maxKv = *State1maxKv;
			} else if (State2maxKv != State2Kv.end()) {
				maxKv = *State2maxKv;
			}
			// �������� � ������ ����� = ��������
			xMax = xMin;
			// ���� �� ����� >= ������������� �������� Kv - ������ ������������ 0.1
			if (xMax >= maxKv) xMax += 0.1;
			// ���� ����� �������� ��� � ������������ �����
			while(xMax < maxKv)
			{
				if (xMax >= 3 + xMin - 0.0000001) xMax += 1;
				if (xMax >= 2 + xMin - 0.0000001 && xMax < 3 + xMin - 0.0000001) xMax +=0.5;
				if (xMax >= 1 + xMin - 0.0000001 && xMax < 2 + xMin - 0.0000001) xMax += 0.25;
				if (xMax >= 0.6 + xMin - 0.0000001 && xMax < 1 + xMin - 0.0000001) xMax += 0.2;
				if (xMax < 0.6 + xMin - 0.0000001) xMax += 0.1;
			}
		}

		// ��������������� ����� ������� ���� (��������� ��-�� ���������� double)
		ui.plotArea->xAxis->setRange(xMin - 0.0000001, xMax + 0.0000001);

		// ������� ����� ����� �  ��������� ��� � ��� X
		QSharedPointer<QCPAxisTickerFixed> xfixedTicker(new QCPAxisTickerFixed);
		ui.plotArea->xAxis->setTicker(xfixedTicker);
		// ��� ��������������
		xfixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
		// ������ ������� - �������
		xfixedTicker->setTickOrigin(xMin);
		// ������� ����� ����� � ����������� �� �������� ��������� � ��������
		if (xMax - xMin <= 0.6 - 0.0000001)
		{
			xfixedTicker->setTickStep(0.1);
		} else if (xMax - xMin > 0.6 - 0.0000001 && xMax - xMin <= 1 - 0.0000001) 
		{
			xfixedTicker->setTickStep(0.2);
		} else if (xMax - xMin > 1 - 0.0000001 && xMax - xMin <= 2 - 0.0000001) 
		{
			xfixedTicker->setTickStep(0.25);
		} else if (xMax - xMin > 2 - 0.0000001 && xMax - xMin <= 3 - 0.0000001) 
		{
			xfixedTicker->setTickStep(0.5);
		} else if (xMax - xMin > 3 - 0.0000001) 
		{
			xfixedTicker->setTickStep(1);
		}

		// ��������� ���������
		AxisSettingNeeded = false;
	}
}

// ������� ��������� �������� �������� ��������� ����
#define GET_SPINBOX_VALUE emit getSpinBoxValue()

// ����� ���
void Kv_Distribution::FA_selected(int FA_Number)
{
	// ������� ��������� ������
	ui.plotArea->clearGraphs();

	// ��������� ������ ������ ���
	SetFAnum(FA_Number);
	
	// �������� � ��������, ������ ���� ���� ��������� �� ���� ���������
	if (renderArea)
	{
		// ���� ������������� ��� ��������, �� ���������� ��� ����� ��� ������ ������ ���
		if (AutoAxisSetting == AutoAxis::HighBorderAuto || AutoAxisSetting == AutoAxis::AllAuto) AxisSettingNeeded = true;
		// ������������ ���
		XAxisSetting();

		// ������ �����
		QVector<double> LayersVector(cDataState::GetNumLayers());
		// ��������� ��� �� 0 �� 100 - ������ ���� � ����, ��������� - � 100
		for (int i = 0; i < cDataState::GetNumLayers(); ++i) { LayersVector[i] = static_cast<double>(i) * 100 / ( cDataState::GetNumLayers() - 1 );}
		// ��������� ������ ���, ����� ���� ������� ���� ��� Y, � ������� - ��� X
		ui.plotArea->addGraph(ui.plotArea->axisRect()->axis(QCPAxis::atLeft),ui.plotArea->axisRect()->axis(QCPAxis::atBottom));
		// ��������� � ���� ������ - ������ ����� � ������ �������� Kv ��� ��������� 1
		ui.plotArea->graph(0)->addData(LayersVector, QVector<double>::fromStdVector(renderArea->state1()[FA_Number].GetKv()), true);
		// ������ ��������� ��������� � ��� �������
		ui.plotArea->graph(0)->setPen(State1Pen);
		ui.plotArea->graph(0)->setName(State1FileName());

		// ��������� ������ ���, ����� ���� ������� ���� ��� Y, � ������� - ��� X
		ui.plotArea->addGraph(ui.plotArea->axisRect()->axis(QCPAxis::atLeft),ui.plotArea->axisRect()->axis(QCPAxis::atBottom));
		// ��������� � ���� ������ - ������ ����� � ������ �������� Kv ��� ��������� 2
		ui.plotArea->graph(1)->addData(LayersVector, QVector<double>::fromStdVector(renderArea->state2()[FA_Number].GetKv()), true);
		// ������ ��������� ��������� � ��� �������
		ui.plotArea->graph(1)->setPen(State2Pen);
		ui.plotArea->graph(1)->setName(State2FileName());

		// ��������� ����� ������ - ����� ����
		ui.plotArea->addGraph();
		// ��� �������������� ������ ���� �������� �������� - Kv
		if (cFA_Box::ActiveMode() == Parameters::Kv)
		{
			// ��� ������� - ���������� ���� ����� �� X � �� Y
			QVector<double> xValues(2);
			QVector<double> yValues(2);
			// �� ��� X - ��� ���������� �������� ���
			xValues[0] = ui.plotArea->xAxis->range().lower;
			xValues[1] = ui.plotArea->xAxis->range().upper;
			// �� ��� Y - ��� ���������� ��������� ���� (���������� ������� ����� ��� ���������� �������� ���������)
			yValues[0] = static_cast<double>(GET_SPINBOX_VALUE - 1) * 100 / ( cDataState::GetNumLayers() - 1 );
			// ���� ������ � ���� � ������� - ����� ����� ��-�� ��� - ������� ����-���� ������� � ������� ������ �������
			if (yValues[0] <= 0.7) yValues[0] = 0.7;
			if (yValues[0] >= 99.8) yValues[0] = 99.8;
			// ��� �������������� ����� - � ��� Y �����
			yValues[1] = yValues[0];
			// ������ ������ ��� ������� � ��������� ���������
			ui.plotArea->graph(2)->setData(xValues,yValues,true);
			ui.plotArea->graph(2)->setPen(QPen(Qt::darkGray,1.5));
		}
		// ������� ��� �� �������
		ui.plotArea->graph(2)->removeFromLegend();

		// ��������� ��� 2 ������� - ������������ � �������������� ����� ����� ������� ����
		ui.plotArea->addGraph();
		ui.plotArea->addGraph();
		// ������ �� ��������� ��������� � ������� �� �������
		ui.plotArea->graph(3)->setPen(QPen(Qt::gray,0.8));
		ui.plotArea->graph(3)->removeFromLegend();
		ui.plotArea->graph(4)->setPen(QPen(Qt::gray,0.8));
		ui.plotArea->graph(4)->removeFromLegend();
	}

	// �������������� ������
	ui.plotArea->replot();
}
