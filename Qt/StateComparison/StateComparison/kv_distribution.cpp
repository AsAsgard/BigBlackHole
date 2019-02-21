#include "kv_distribution.h"
#include "filebrowser.h"
#include "fa_box.h"

Kv_Distribution::Kv_Distribution(const QPen& oldState1Pen, const QPen& oldState2Pen, const AutoAxis::AutoAxisEnum& oldAutoAxisSetting, QWidget *parent)
	: QMainWindow(parent),
	  // стандартные параметры отрисовки
	  defaultState1Pen(Qt::red, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
	  defaultState2Pen(Qt::darkGreen, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin),
	  State1Pen(oldState1Pen),
	  State2Pen(oldState2Pen),
	  AutoAxisSetting(oldAutoAxisSetting), AxisSettingNeeded(true)
{
	ui.setupUi(this);
	// задаем трехзначное значение номера ТВС - для задания минимального размера надписи
	CurrentFA = 163;

	// задаем стандартные параметры, если цвета белые (при создании объектов)
	if (State1Pen.color() == Qt::white && State2Pen.color() == Qt::white)
	{
		State1Pen = defaultState1Pen;
		State2Pen = defaultState2Pen;
		AutoAxisSetting = AutoAxis::NoAuto;
	}
	
	// задаем номер ТВС, шрифты и минимальный размер для него
	FA_Num.reset(new QCPTextElement(ui.plotArea));
	QFont font = ui.plotArea->xAxis->labelFont();
	font.setPointSize(font.pointSize() + 1);
	FA_Num->setFont(font);
    FA_Num->setText(tr("FA      %1").arg(CurrentFA));
	FA_Num->setMinimumSize(FA_Num->rect().width(), FA_Num->rect().height());

	// выравнивание легенды графика и номера ТВС
	// создаем новый лэйаут
	LegendLayout.reset(new QCPLayoutGrid);
	// добавляем этот лэйаут второй строкой - под самим графиком
	ui.plotArea->plotLayout()->addElement(1, 0, LegendLayout.data());
	// поля лэйаута
	LegendLayout->setMargins(QMargins(5, 0, 5, 5));
	// добавляем номер ТВС в первую колонку
	LegendLayout->addElement(0, 0, FA_Num.data());
	// минимальный размер номера ТВС
	LegendLayout->element(0, 0)->setMinimumSize(FA_Num->rect().width(), FA_Num->minimumSize().height());
	// добавляем легенду во вторую колонку
	LegendLayout->addElement(0, 1, ui.plotArea->legend);
	// левая часть - 1% площади, но есть минимальный размер
	LegendLayout->setColumnStretchFactor(0, 1);
	// правая - 99 % площади
    LegendLayout->setColumnStretchFactor(1, 99);
	// чтобы минимальный размер по высоте был
	ui.plotArea->plotLayout()->setRowStretchFactor(1, 0.001);

	// слежение за мышью
	ui.plotArea->setMouseTracking(true);
	// связываем сигналы движения мыши в QCustomPlot и в основном окне - менять будем отсюда
	connect(ui.plotArea, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(MouseOnPlot(QMouseEvent *)));
	
	// задаем нулевой указатель настройщику гистограммы
	Setter.reset(nullptr);
	// указатель на поле картограммы уже нулевой

	// очищаем все графики
	ui.plotArea->clearGraphs();
	// задаем названия осей
	ui.plotArea->xAxis->setLabel("Kv");
    ui.plotArea->yAxis->setLabel("H, %Hcore");

	// выставляем пределы оси Y
	ui.plotArea->yAxis->setRange(0, 100);
		
	// создаем новый тикер и  применяем его к оси Y
	QSharedPointer<QCPAxisTickerFixed> yfixedTicker(new QCPAxisTickerFixed);
	ui.plotArea->yAxis->setTicker(yfixedTicker);
	// задаем шаг основных меток - возле которых будут стоять значения
	yfixedTicker->setTickStep(10);
	// задаем количество дополнительных меток - между основными метками
	yfixedTicker->setTickCount(4);
	// без авторасширения
	yfixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);

	// рисуем легенду
	ui.plotArea->legend->setVisible(true);
	ui.plotArea->legend->setBrush(QBrush(QColor(255,255,255,150)));
	ui.plotArea->legend->setFont(ui.plotArea->xAxis->labelFont());
		
	// выбор ТВС с максимальной разницей Kv
	FA_selected(cFA_Box::FAwithMaxDeltaKv.FA);
}

// деструктор
Kv_Distribution::~Kv_Distribution() {}

void Kv_Distribution::MouseOnPlot(QMouseEvent * mouse)
{
	// 3 и 4 графики - вертикальная и горизонтальная линии через позицию мыши
	// удаляем данные для 3 и 4 графиков 
	ui.plotArea->graph(3)->data()->clear();
	ui.plotArea->graph(4)->data()->clear();
	
	// получаем позицию мыши в координатах графика
	double xPos = ui.plotArea->xAxis->pixelToCoord(mouse->pos().x());
	double yPos = ui.plotArea->yAxis->pixelToCoord(mouse->pos().y());
	// если позиция внутри границ осей, то отрисовываем линии
	if ( ui.plotArea->xAxis->range().upper >= xPos &&
		 ui.plotArea->xAxis->range().lower <= xPos &&
		 ui.plotArea->yAxis->range().upper >= yPos &&
		 ui.plotArea->xAxis->range().lower <= yPos )
	{
		QVector<double> xValues(2);
		QVector<double> yValues(2);

		// вертикальная линия - задаем данные
		xValues[0] = xPos;
		xValues[1] = xPos;
		yValues[0] = ui.plotArea->yAxis->range().lower;
		yValues[1] = ui.plotArea->yAxis->range().upper;
		ui.plotArea->graph(3)->setData(xValues,yValues,true);

		// горизонтальная линия - задаем данные
		xValues[0] = ui.plotArea->xAxis->range().lower;
		xValues[1] = ui.plotArea->xAxis->range().upper;
		yValues[0] = yPos;
		yValues[1] = yPos;
		ui.plotArea->graph(4)->setData(xValues,yValues,true);
	}
	// перестроить график
	ui.plotArea->replot();
}

// скриншот гистограммы
void Kv_Distribution::on_Screenshot_triggered()
{
	// вызываем диалог сохранения файла
	QString ScreenFileName = QFileDialog::getSaveFileName(this,
                                                          tr("Save as..."),
														  defaultPath(),
														  tr("JPG Files (*.jpg);;PNG Files (*.png);;BMP Files (*.bmp);;All Files (*.*)")
														  );
	// если имя файла выбрано
	if (!ScreenFileName.isEmpty()) 
	{
		// сохраняем стандартный путь
		setDefaultPath(ScreenFileName.left(ScreenFileName.lastIndexOf('/')+1));
		// сохраняем изображение
		QPixmap screen = QPixmap::grabWidget(ui.centralWidget);
		// пытаемся улучшить качество - увеличиаем размеры без потери качества
		screen = screen.scaled(ui.centralWidget->width()*4,ui.centralWidget->height()*4,Qt::KeepAspectRatio,Qt::SmoothTransformation);
		// сохраняем каринку в файл
		screen.save(ScreenFileName, 0, 100);
	}
}

// изменение размера картограммы - меняем размер области отрисовки графика
void Kv_Distribution::resizeEvent(QResizeEvent *)
{
	ui.plotArea->resize(ui.centralWidget->width(), ui.centralWidget->height());
}

// оповещаем картограмму о закрытии гистограммы с отправкой текущих значений параметров отрисовки и автонастройки осей
void Kv_Distribution::closeEvent(QCloseEvent *)
{
	emit closing(State1Pen, State2Pen, AutoAxisSetting);
}


// открытие настройщика гистограммы
void Kv_Distribution::on_Settings_triggered()
{
	// если указатель нулевой
	if (!Setter)
	{
		// создаем новое окно и присоединяем сигналы
		Setter.reset(new KV_Settings(State1Pen, State2Pen, AutoAxisSetting));
		// закрытие настройщика
		connect(Setter.data(), SIGNAL(closing()), this, SLOT(SetterClosed()));
		// отправка текущих настроек отрисовки
		connect(Setter.data(), SIGNAL(getCurrentPens()), this, SLOT(sendCurrentPens()));
		// отправка стандартных настроек отрисовки
		connect(Setter.data(), SIGNAL(getDefaultPens()), this, SLOT(sendDefaultPens()));
		// отправка текущего значения автонастройки осей
		connect(Setter.data(), SIGNAL(getCurrentAutoAxisSetting()), this, SLOT(sendCurrentAutoAxisSetting()));
		// принятие новых параметров отрисовки
		connect(Setter.data(), SIGNAL(sendNewSettings(const QPen&, const QPen&, AutoAxis::AutoAxisEnum)), this, SLOT(getNewSettings(const QPen&, const QPen&, AutoAxis::AutoAxisEnum)));
		// показываем настройщик
		Setter->show();
	}
}


// закрытие настройщика гистограммы
void Kv_Distribution::SetterClosed()
{
	// освобождаем память у указателя
	Setter.reset(nullptr);
}

// отправка текущих настроек отрисовки
QPair<QPen, QPen> Kv_Distribution::sendCurrentPens()
{
	return qMakePair(State1Pen, State2Pen);
}

// отправка стандартных настроек отрисовки
QPair<QPen, QPen> Kv_Distribution::sendDefaultPens()
{
	return qMakePair(defaultState1Pen, defaultState2Pen);
}

// отправка текущего значения автонастройки осей
AutoAxis::AutoAxisEnum Kv_Distribution::sendCurrentAutoAxisSetting()
{
	return AutoAxisSetting;
}

// принятие новых значений настроек отрисовки и автонастройки оси
void Kv_Distribution::getNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting)
{
	State1Pen = newState1Pen;
	State2Pen = newState2Pen;
	AutoAxisSetting = newAutoAxisSetting;
	// необходима перенастройка оси
	AxisSettingNeeded = true;
	// перерисовка гистограммы
	FA_selected(CurrentFA);
}

// получение указателя на поле рисования
void Kv_Distribution::getRenderAreaPointer(QWeakPointer<RenderArea> RA)
{
	// приравниваем указатели
	renderArea = RA;
	// перерисовка гистограммы
	FA_selected(CurrentFA);
}

// перенастройка оси X
void Kv_Distribution::XAxisSetting()
{
	// делается, если она необходима
	if (AxisSettingNeeded)
	{
		//задаются новые максимум и минимум оси
		double xMax = 0;
		double xMin = 0;

		// если нет автонастройки
		if (AutoAxisSetting == AutoAxis::NoAuto)
		{
			// то задается новый максимум исходя из глобального максимума Kv состояний
			while(xMax < renderArea.data()->maxKv()) 
			{
				// делается это с определенным шагом - для дальнейшей разметки шкалы
				// вычитается малая константа из-за неточности типа double
                if (xMax >= 3 - DBL_CALIBRATION) xMax += 1;
                if (xMax >= 2 - DBL_CALIBRATION && xMax < 3 - DBL_CALIBRATION) xMax +=0.5;
                if (xMax >= 1 - DBL_CALIBRATION && xMax < 2 - DBL_CALIBRATION) xMax += 0.25;
                if (xMax >= 0.6 - DBL_CALIBRATION && xMax < 1 - DBL_CALIBRATION) xMax += 0.2;
                if (xMax < 0.6 - DBL_CALIBRATION) xMax += 0.1;
			}
		// если есть автонастройка 
		} else if (AutoAxisSetting == AutoAxis::HighBorderAuto || AutoAxisSetting == AutoAxis::AllAuto) {
			// то копируются вектора Kv для состояний
			std::vector<double> State1Kv = renderArea.data()->state1()[CurrentFA].GetKv();
			std::vector<double> State2Kv = renderArea.data()->state2()[CurrentFA].GetKv();
			// из них исключаются элементы NaN - переносятся в конец вектора и возвращается итератор на новый его конец
			auto it1 = std::remove_if(State1Kv.begin(), State1Kv.end(), [] (const double& elem) { return qIsNaN(elem);});
			auto it2 = std::remove_if(State2Kv.begin(), State2Kv.end(), [] (const double& elem) { return qIsNaN(elem);});
			// происходит удаление этих элементов
			State1Kv.erase(it1, State1Kv.end());
			State2Kv.erase(it2, State2Kv.end());

			// если настройка и по нижней границе, и по верхней 
			if (AutoAxisSetting == AutoAxis::AllAuto)
			{
				// находятся минимальные элементы этих векторов - на них возвращается итератор
				auto State1minKv = std::min_element(State1Kv.begin(), State1Kv.end());
				auto State2minKv = std::min_element(State2Kv.begin(), State2Kv.end());
				double minKv = 0;
				/*
					если итераторы валидны - находится минимальное из их значений 
					если какой-то итератор не валиден - берется значение из другого
					если оба итератора не валидны - остается значение 0
				*/
				if (State1minKv != State1Kv.end() && State2minKv != State2Kv.end()) {
					minKv = std::min(*State1minKv, *State2minKv);
				} else if (State1minKv != State1Kv.end()) {
					minKv = *State1minKv;
				} else if (State2minKv != State2Kv.end()) {
					minKv = *State2minKv;
				}
				// считаем новый минимум оси с шагом 0.1
				while(xMin <= minKv)
				{
					xMin += 0.1;
				}
				xMin -= 0.1;
                if (xMin <= DBL_CALIBRATION) xMin = 0;
			}

			// находятся максимальные элементы этих векторов - на них возвращается итератор
			auto State1maxKv = std::max_element(State1Kv.begin(), State1Kv.end());
			auto State2maxKv = std::max_element(State2Kv.begin(), State2Kv.end());
			double maxKv = 0;
			/*
				если итераторы валидны - находится максимальное из их значений 
				если какой-то итератор не валиден - берется значение из другого
				если оба итератора не валидны - остается значение 0
			*/
			if (State1maxKv != State1Kv.end() && State2maxKv != State2Kv.end()) {
				maxKv = std::max(*State1maxKv, *State2maxKv);
			} else if (State1maxKv != State1Kv.end()) {
				maxKv = *State1maxKv;
			} else if (State2maxKv != State2Kv.end()) {
				maxKv = *State2maxKv;
			}
			// максимум в начале счета = минимуму
			xMax = xMin;
			// если он сразу >= максимальному значению Kv - просто прибавляется 0.1
			if (xMax >= maxKv) xMax += 0.1;
			// ищем новый максимум оси с определенным шагом
			while(xMax < maxKv)
			{
                if (xMax >= 3 + xMin - DBL_CALIBRATION) xMax += 1;
                if (xMax >= 2 + xMin - DBL_CALIBRATION && xMax < 3 + xMin - DBL_CALIBRATION) xMax +=0.5;
                if (xMax >= 1 + xMin - DBL_CALIBRATION && xMax < 2 + xMin - DBL_CALIBRATION) xMax += 0.25;
                if (xMax >= 0.6 + xMin - DBL_CALIBRATION && xMax < 1 + xMin - DBL_CALIBRATION) xMax += 0.2;
                if (xMax < 0.6 + xMin - DBL_CALIBRATION) xMax += 0.1;
			}
		}

		// устанавливаются новые пределы осей (константы из-за неточности double)
        ui.plotArea->xAxis->setRange(xMin - DBL_CALIBRATION, xMax + DBL_CALIBRATION);

		// создаем новый тикер и  применяем его к оси X
		QSharedPointer<QCPAxisTickerFixed> xfixedTicker(new QCPAxisTickerFixed);
		ui.plotArea->xAxis->setTicker(xfixedTicker);
		// без авторасширения
		xfixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
		// начало отсчета - минимум
		xfixedTicker->setTickOrigin(xMin);
		// задание шагов сетки в зависимости от разности максимума и минимума
        if (xMax - xMin <= 0.6 - DBL_CALIBRATION)
		{
			xfixedTicker->setTickStep(0.1);
        } else if (xMax - xMin > 0.6 - DBL_CALIBRATION && xMax - xMin <= 1 - DBL_CALIBRATION)
		{
			xfixedTicker->setTickStep(0.2);
        } else if (xMax - xMin > 1 - DBL_CALIBRATION && xMax - xMin <= 2 - DBL_CALIBRATION)
		{
			xfixedTicker->setTickStep(0.25);
        } else if (xMax - xMin > 2 - DBL_CALIBRATION && xMax - xMin <= 3 - DBL_CALIBRATION)
		{
			xfixedTicker->setTickStep(0.5);
        } else if (xMax - xMin > 3 - DBL_CALIBRATION)
		{
			xfixedTicker->setTickStep(1);
		}

		// настройка закончена
		AxisSettingNeeded = false;
	}
}

// максрос получения значения текущего высотного слоя
#define GET_SPINBOX_VALUE emit getSpinBoxValue()

// выбор ТВС
void Kv_Distribution::FA_selected(int FA_Number)
{
	// очищаем имеющиеся данные
	ui.plotArea->clearGraphs();

	// установка нового номера ТВС
	SetFAnum(FA_Number);
	
	// работаем с графиком, только если есть указатель на поле рисования
	if (renderArea)
	{
		// если автонастройка оси включена, то перерасчет оси нужен при каждом выборе ТВС
		if (AutoAxisSetting == AutoAxis::HighBorderAuto || AutoAxisSetting == AutoAxis::AllAuto) AxisSettingNeeded = true;
		// перенастрйка оси
		XAxisSetting();

		// вектор слоев
		QVector<double> LayersVector(cDataState::GetNumLayers());
		// заполняем его от 0 до 100 - первый слой в нуле, последний - в 100
		for (int i = 0; i < cDataState::GetNumLayers(); ++i) { LayersVector[i] = static_cast<double>(i) * 100 / ( cDataState::GetNumLayers() - 1 );}
		// добавляем график так, чтобы осью абсцисс была ось Y, а ординат - ось X
		ui.plotArea->addGraph(ui.plotArea->axisRect()->axis(QCPAxis::atLeft),ui.plotArea->axisRect()->axis(QCPAxis::atBottom));
		// добавляем в него данные - вектор слоев и вектор значений Kv для состояния 1
		ui.plotArea->graph(0)->addData(LayersVector, QVector<double>::fromStdVector(renderArea.data()->state1()[FA_Number].GetKv()), true);
		// задаем параметры отрисовки и имя графика
		ui.plotArea->graph(0)->setPen(State1Pen);
		ui.plotArea->graph(0)->setName(State1FileName());

		// добавляем график так, чтобы осью абсцисс была ось Y, а ординат - ось X
		ui.plotArea->addGraph(ui.plotArea->axisRect()->axis(QCPAxis::atLeft),ui.plotArea->axisRect()->axis(QCPAxis::atBottom));
		// добавляем в него данные - вектор слоев и вектор значений Kv для состояния 2
		ui.plotArea->graph(1)->addData(LayersVector, QVector<double>::fromStdVector(renderArea.data()->state2()[FA_Number].GetKv()), true);
		// задаем параметры отрисовки и имя графика
		ui.plotArea->graph(1)->setPen(State2Pen);
		ui.plotArea->graph(1)->setName(State2FileName());

		// добавляем новый график - линию слоя
		ui.plotArea->addGraph();
		// она отрисовывается только если активный параметр - Kv
		if (cFA_Box::ActiveMode() == Parameters::Kv)
		{
			// два вектора - координаты двух точек по X и по Y
			QVector<double> xValues(2);
			QVector<double> yValues(2);
			// по оси X - это координаты пределов оси
			xValues[0] = ui.plotArea->xAxis->range().lower;
			xValues[1] = ui.plotArea->xAxis->range().upper;
			// по оси Y - это координаты положения слоя (аналогично вектору слоев при заполнении графиков состояний)
			yValues[0] = static_cast<double>(GET_SPINBOX_VALUE - 1) * 100 / ( cDataState::GetNumLayers() - 1 );
			// если близко к нулю и вершине - плохо видно из-за оси - поэтому чуть-чуть двигаем в сторону центра графика
			if (yValues[0] <= 0.7) yValues[0] = 0.7;
			if (yValues[0] >= 99.8) yValues[0] = 99.8;
			// это горизонтальная линия - у нее Y равны
			yValues[1] = yValues[0];
			// задаем данные для графика и параметры отрисовки
			ui.plotArea->graph(2)->setData(xValues,yValues,true);
			ui.plotArea->graph(2)->setPen(QPen(Qt::darkGray,1.5));
		}
		// удаляем его из легенды
		ui.plotArea->graph(2)->removeFromLegend();

		// добавляем ещё 2 графика - вертикальная и горизонтальная линии через позицию мыши
		ui.plotArea->addGraph();
		ui.plotArea->addGraph();
		// задаем им параметры отрисовки и удаляем из легенды
		ui.plotArea->graph(3)->setPen(QPen(Qt::gray,0.8));
		ui.plotArea->graph(3)->removeFromLegend();
		ui.plotArea->graph(4)->setPen(QPen(Qt::gray,0.8));
		ui.plotArea->graph(4)->removeFromLegend();
	}

	// перерисовываем график
	ui.plotArea->replot();
}

void Kv_Distribution::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        SetFAnum(currentFA());
        ui.retranslateUi(this);
        ui.plotArea->replot();
    }
    QMainWindow::changeEvent(event);
}
