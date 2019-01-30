#include <sstream>
#include <iomanip>
#include <QFileDialog>
#include <QDesktopWidget>
#include "comparisonfield.h"
#include "kv_distribution.h"
#include "extfunctions.h"
#include "pathandfiles.h"

// конструктор
ComparisonField::ComparisonField(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QRect &ScreenParameters)
	: QMainWindow(parent), Kv1SavedPen(Qt::white), Kv2SavedPen(Qt::white), KvSavedAutoAxis(AutoAxis::NoAuto)
{
	ui.setupUi(this);
	// инициализируем указатели на зависимые формы нулевыми указателями
	KvDistrib.reset(nullptr);
	ColorChanger.reset(nullptr);
	fileBrowser.reset(nullptr);
	// задаем максимум для SpinBox-а с высотными слоями
	ui.spinBox->setMaximum(cDataState::GetNumLayers());
	// выбираем ширину и высоту окна (в т.ч. минимальные) в зависимости от размеров экрана
	if (ScreenParameters.width() >= ScreenParameters.height())
	{
		this->setMinimumSize(0.6 * ScreenParameters.height(), 0.6 * ScreenParameters.height());
		this->resize(ScreenParameters.height()-150, ScreenParameters.height()-150); 
	} else {
		this->setMinimumSize(0.6 * ScreenParameters.width(), 0.6 * ScreenParameters.width());
		this->resize(ScreenParameters.width()-150, ScreenParameters.width()-150);
	}
	// двигаем окно в центр экрана
	this->move(ScreenParameters.center().x() - this->width()/2,50);

	// изменяем размеры статус-бара, создаем на нем Label, задаем его геометрию и выравнивание и обновляем текст
	ui.statusBar->resize(this->width(), 20);
	statusLabel.reset(new QLabel(ui.statusBar));
	statusLabel->setGeometry(4, 0, this->width(), ui.statusBar->height());
	statusLabel->setAlignment(Qt::AlignVCenter);
	StatusBarUpdate();

	// двигаем в угол SpinBox и Label с номером слоя и делаем Label невидимым
	ui.groupBox->move( this->width() - (ui.groupBox->width() + 2) , this->height() - (ui.groupBox->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() - 15));
	ui.LayerLabel->setVisible(false);
	ui.LayerLabel->move( this->width() - (ui.LayerLabel->width() + 2) , this->height() - (ui.LayerLabel->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() - 5));
	// фиксируем высоту окна - будет работать пропорциональное расширение окна при изменение ширины
	ComparisonField::setFixedHeight(ComparisonField::height());

	// создаем поле для рисования
	renderArea.reset(new RenderArea(ui.centralWidget, 
									rState1,
                                    rState2
                                    ));
	// присоединяем сигналы и слоты для поля рисования и основного окна картограммы
	// проверка активности гистограммы Kv
	connect(renderArea.data(), SIGNAL(isKvDiagramActive()), this, SLOT(KvDiagramAvaliability()));
	// сигнал для поля рисования, что режим просмотра был изменен
	connect(this, SIGNAL(ViewModeChange(bool)), renderArea.data(), SLOT(ViewModeChanged(bool)));
	// сигнал для поля рисования, что состояния были изменены
	connect(this, SIGNAL(ChangeStatesCF(const cDataState&, const cDataState&)), renderArea.data(), SLOT(StatesBinding(const cDataState&, const cDataState&)));
	// поднимаем на первый план SpinBox и Label с номером слоя (для скриншота)
	ui.LayerLabel->raise();
	ui.groupBox->raise();
	
	// параметры шрифтов для совместимости с high dpi
	QFont font(ui.groupBox->font());
	font.setPixelSize(13);
	ui.groupBox->setFont(font);
	font = ui.LayerLabel->font();
	font.setPixelSize(16);
	ui.LayerLabel->setFont(font);
	font = ui.spinBox->font();
	font.setPixelSize(16);
	ui.spinBox->setFont(font);
}

// деструктор
ComparisonField::~ComparisonField() {}

// сравнить другие состояния
void ComparisonField::on_otherStates_triggered()
{
	// создание нового обозреателя файлов
	fileBrowser.reset(new FileBrowser());
	// присоединяем сигналы и слоты для обозревателя файлов и основного окна картограммы
	// извещение о закрытии обозревателя файлов
	connect(fileBrowser.data(),SIGNAL(closing()),this,SLOT(FileBrowserClosed()));
	// запрос от обозревателя файлов проверить наличие картограммы
	connect(fileBrowser.data(),SIGNAL(isComparisonFieldActive()),this,SLOT(ComparisonFieldAvaliability()));
	// файлы выбраны - необходимо изменить состояния
	connect(fileBrowser.data(),SIGNAL(ChangeStatesFB(const cDataState&, const cDataState&)),this,SLOT(StatesChanged(const cDataState&, const cDataState&)));
	// показываем обозреватель файлов
	fileBrowser->show();
}


// включена/выключена цветовая гамма поля рисования
void ComparisonField::on_ColorFont_toggled(bool checked)
{
	// делаем ТВС цветными/безцветными
	cFA_Box::SetColorful(checked);
	// перерисовываем поля для рисования
	renderArea->repaint();
}

// изменение режима просмотра
void ComparisonField::on_ChangeViewMode_toggled(bool DeltaChecked)
{
	// отправка сигнала в поля для рисования для изменения текстов и цветов
	emit ViewModeChange(DeltaChecked);	
	// обновление статус-бара
	StatusBarUpdate();
}

// выбран режим Kq
void ComparisonField::on_activateKq_triggered()
{
	// вдавливаем кнопку Kq, остальные не вдавлены
	ui.activateKq->setChecked(true);
	ui.activateBurn->setChecked(false);
	ui.activateKv->setChecked(false);
	// если режим не Kq, то проводим изменения
	if (!(cFA_Box::ActiveMode()==Parameters::Kq))
	{
		// SpinBox высотных слоев делаем невидимым
		ui.groupBox->setVisible(false);
		// кнопка показать максимум не активна
		ui.MaxShow->setEnabled(false);
		// изменям текст поля рисования на текст, соответствующий Kq
		renderArea->ChangeText(Parameters::Kq, 0);
		// перерисовываем поле
		renderArea->repaint();

		// если активна гистограмма Kv - убираем линию, соответствующую уровню слоя
		if (KvDistrib)
		{
			emit hideLayerLine();
		}
	}
}

void ComparisonField::on_activateBurn_triggered()
{
	// вдавливаем кнопку Burn, остальные не вдавлены
	ui.activateKq->setChecked(false);
	ui.activateBurn->setChecked(true);
	ui.activateKv->setChecked(false);
	// если режим не Burn, то проводим изменения
	if (!(cFA_Box::ActiveMode()==Parameters::Burn))
	{
		// SpinBox высотных слоев делаем невидимым
		ui.groupBox->setVisible(false);
		// кнопка показать максимум не активна
		ui.MaxShow->setEnabled(false);
		// изменям текст поля рисования на текст, соответствующий Burn
		renderArea->ChangeText(Parameters::Burn, 0);
		// перерисовываем поле
		renderArea->repaint();
		
		// если активна гистограмма Kv - убираем линию, соответствующую уровню слоя
		if (KvDistrib)
		{
			emit hideLayerLine();
		}
	}
}

void ComparisonField::on_activateKv_triggered()
{
	// вдавливаем кнопку Kv, остальные не вдавлены
	ui.activateKq->setChecked(false);
	ui.activateBurn->setChecked(false);
	ui.activateKv->setChecked(true);
	// если режим не Kv, то проводим изменения
	if (!(cFA_Box::ActiveMode()==Parameters::Kv))
	{
		// SpinBox высотных слоев делаем видимым
		ui.groupBox->setVisible(true);
		// кнопка показать максимум активна
		ui.MaxShow->setEnabled(true);
		// изменям текст поля рисования на текст, соответствующий Kv с текущим уровнем слоя
		renderArea->ChangeText(Parameters::Kv, ui.spinBox->value());
		// перерисовываем поле
		renderArea->repaint();

		// если активна гистограмма Kv - перерисовываем ее с линией слоя
		if (KvDistrib)
		{
			emit select_FA(KvDistrib->currentFA());
		}
	}
}


// нажата кнопка "показать максимум"
void ComparisonField::on_MaxShow_triggered()
{
	// должн быть режим Kv
	if (cFA_Box::ActiveMode() != Parameters::Kv) return;
	// задаем значение слоя = слою с максимальной разницей Kv
	ui.spinBox->setValue(cFA_Box::FAwithMaxDeltaKv.Layer);
	// если активна гистограмма Kv
	if(KvDistrib)
	{
		// задаем ТВС с максимальной разницей Kv
		emit select_FA(cFA_Box::FAwithMaxDeltaKv.FA);
	}
}

void ComparisonField::on_Kv_diagramActivator_triggered()
{
	if (!KvDistrib) 
	{
		// создаем новую гистограмму Kv
		KvDistrib.reset(new Kv_Distribution(Kv1SavedPen, Kv2SavedPen, KvSavedAutoAxis));
		// присоединяем сигналы и слоты для гистограммы и основного окна картограммы
		connect(KvDistrib.data(), SIGNAL(closing(const QPen&, const QPen&, const AutoAxis::AutoAxisEnum&)), this, SLOT(KvDiagramClosed(const QPen&, const QPen&, const AutoAxis::AutoAxisEnum&)));
		// выбор ТВС на гистограмме
		connect(this, SIGNAL(select_FA(int)), KvDistrib.data(), SLOT(FA_selected(int)));
		// скрытие линии слоя
		connect(this, SIGNAL(hideLayerLine()), KvDistrib.data(), SLOT(LayerLineHiding()));
		// необходима перенастройка оси X гистограммы
		connect(this, SIGNAL(resetAxisNeeded(int)), KvDistrib.data(), SLOT(ResetAxisNeededTrue()));
		// выбор ТВС на поле картограммы - перерисовка гистограммы Kv при ее наличии
		connect(renderArea.data(), SIGNAL(select_FA(int)), KvDistrib.data(), SLOT(FA_selected(int)));
		// получение гистограммой текущего значения высотного слоя
		connect(KvDistrib.data(), &Kv_Distribution::getSpinBoxValue, [this] () { return ui.spinBox->value();});
		// отправить гистограмме указатель на поле рисования
		connect(this, SIGNAL(sendRenderAreaPointer(QWeakPointer<RenderArea>)), KvDistrib.data(), SLOT(getRenderAreaPointer(QWeakPointer<RenderArea>)));
		
		// отправляем гистограмме указатель на поле рисования
		emit sendRenderAreaPointer(renderArea);

		// задаем размеры и положение гистограммы на экране
		QDesktopWidget Desktop;
		KvDistrib->setMinimumSize(0.176 * Desktop.screenGeometry(this).width(), 0.338 * Desktop.screenGeometry(this).height());
		KvDistrib->resize(0.218 * Desktop.screenGeometry(this).width(), 0.465 * Desktop.screenGeometry(this).height());
		KvDistrib->move(Desktop.screenGeometry(this).x() + Desktop.screenGeometry(this).width() - 0.026 * Desktop.screenGeometry(this).width() - KvDistrib->width(), 
							   0.046 * Desktop.screenGeometry(this).height());
		// показываем гистограмму
		KvDistrib->show();
	}
}


// скриншот картограммы
void ComparisonField::on_Screenshot_triggered()
{
	// вызываем диалог сохранения файла
	QString ScreenFileName = QFileDialog::getSaveFileName(this,
														  QString::fromStdWString(L"Сохранить как..."),
														  defaultPath(),
														  tr("JPG Files (*.jpg);;PNG Files (*.png);;BMP Files (*.bmp);;All Files (*.*)")
														  );
	// если имя файла выбрано
	if (!ScreenFileName.isEmpty()) 
	{
		// сохраняем стандартный путь
		setDefaultPath(ScreenFileName.left(ScreenFileName.lastIndexOf('/')+1));
		// если виден номер слоя - прячем SpinBox и показываем надпись с номером слоя
		bool LayersVisible = false;
		if (ui.groupBox->isVisible()) 
		{ 
			LayersVisible = true; 
			ui.groupBox->setVisible(false); 
			ui.LayerLabel->setText(QString::fromStdWString( L"Слой " + ExtFunctions::to_wstring(ui.spinBox->value()) )); 
			ui.LayerLabel->setVisible(true);
		}
		// сохраняем изображение
		QPixmap screen = QPixmap::grabWidget(ui.centralWidget);
		// если был виден номер слоя - показываем SpinBox номера слоя
		if (LayersVisible) { ui.groupBox->setVisible(true); ui.LayerLabel->setVisible(false);}
		// пытаемся улучшить качество - увеличиаем размеры без потери качества
		screen = screen.scaled(ui.centralWidget->width()*4,ui.centralWidget->height()*4,Qt::KeepAspectRatio,Qt::SmoothTransformation);
		// сохраняем каринку в файл
        screen.save(ScreenFileName, NULL, 100);
	}
}

// вызвано окно настройки картограммы
void ComparisonField::on_ChangeColors_triggered()
{
	// если его нет
	if (!ColorChanger)
	{
		// то создаем новое окно
		ColorChanger.reset(new CF_ColorChanger());
		// присоединяем сигналы и слоты для окна настройки и основного окна картограммы
		// извещение о закрытии настройщика картограммы
		connect(ColorChanger.data(), SIGNAL(closing()), this, SLOT(ColorChangerClosed()));
		// необходимо перерисовать картограмму
		connect(ColorChanger.data(), SIGNAL(RecolorComparisonField()), this, SLOT(RecolorAll()));
		// изменение активного параметра - перерисовка картограммы
		connect(ColorChanger.data(), &CF_ColorChanger::changeActiveMode, [this] (const Parameters::ParametersEnum& newActiveMode) 
				{
					// при выборе нового режима в окне настройки будем переключать режим и в окне картограммы
					switch(newActiveMode) {
					case Parameters::Kq   : ui.activateKq->trigger();
										    break;
					case Parameters::Burn : ui.activateBurn->trigger();
										    break;
					case Parameters::Kv   : ui.activateKv->trigger();
										    break;
					}
				});
		// показываем окно настройки
		ColorChanger->show();
	}
}

// перекрасить все ТВС
void ComparisonField::RecolorAll(void)
{
	// Изменяем текст всех ТВС - актуально при изменении режима отображения
	renderArea->ChangeText(cFA_Box::ActiveMode(), ui.spinBox->value());
	// перерисовываем поле
	renderArea->repaint();
}

// изменение значения номера слоя 
void ComparisonField::on_spinBox_valueChanged(int)
{
	// если включен режим Kv - изменяем текст на поле рисования, делая его соответствующим новому высотному слою, и перерисовываем его
	if (cFA_Box::ActiveMode() == Parameters::Kv)
	{
		renderArea->ChangeText(Parameters::Kv, ui.spinBox->value());
		renderArea->repaint();
	}
	// если активна гистограмма Kv - перерисовываем ее (чтобы сместилась линия, показывающая слой)
	if (KvDistrib)
	{
		emit select_FA(KvDistrib->currentFA());
	}
}

// обновление статус-бара - в зависимости от режима отображения выводятся разные сообщения, включающие названия файлов состояний
void ComparisonField::StatusBarUpdate(void)
{
	if (cFA_Box::ViewMode() == View::DeltaView)
	{
		statusLabel->setText(QString::fromWCharArray(L"Значение: ") + State1FileName() + 
							 QString::fromWCharArray(L" - ") + State2FileName());
	} else if (cFA_Box::ViewMode() == View::TwoStatesView) {
		statusLabel->setText(QString::fromWCharArray(L"Значение 1: ") + State1FileName() + 
							 QString::fromWCharArray(L", Значение 2: ") + State2FileName());
	}
}

// изменение размеров поля - пропорциональное
void ComparisonField::resizeEvent(QResizeEvent *)
{
	// меняется только ширина - поэтому задаем новую фиксированную высоту, при этом рекурсии не возникает
	this->setFixedHeight(this->width());
	// двигаем в угол SpinBox и Label с номером слоя и подстраиваем размеры поля для рисования
	ui.LayerLabel->move( this->width() - (ui.LayerLabel->width() + 2) , this->height() - (ui.LayerLabel->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() + 2));
	ui.groupBox->move( this->width() - (ui.groupBox->width() + 2) , this->height() - (ui.groupBox->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() + 2));
	renderArea->resize( this->width(), this->height() - (ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height()));
}

// проверка активности гистограммы Kv
bool ComparisonField::KvDiagramAvaliability() const
{ 
	// указатель возвратит true, если у него есть значение, на которое он указывает 
	if (KvDistrib) { return true;} 
	else {return false;} 
}

// закрытие гистограммы Kv - обнуляем указатель на него и сохраняем параметры отрисовки и автонастройки оси X
void ComparisonField::KvDiagramClosed(const QPen& Kv1PenToSave, const QPen& Kv2PenToSave, const AutoAxis::AutoAxisEnum& KvAutoAxisToSave)
{
	Kv1SavedPen = Kv1PenToSave;
	Kv2SavedPen = Kv2PenToSave;
	KvSavedAutoAxis = KvAutoAxisToSave;
	KvDistrib.reset(nullptr);
}

// закрытие окна настройки картограммы - обнуляем указатель на него
void ComparisonField::ColorChangerClosed()
{
	ColorChanger.reset(nullptr);
}

// закрытие обозревателя файлов - обнуляем указатель на него
void ComparisonField::FileBrowserClosed()
{
	fileBrowser.reset(nullptr);
}

// если поле есть - вернет true
bool ComparisonField::ComparisonFieldAvaliability() const
{
	return true;
}

// изменить состояния - выбраны новые состояния
void ComparisonField::StatesChanged(const cDataState& rState1, const cDataState& rState2)
{
	// задаем новый максимум количества выстных слоев
	ui.spinBox->setMaximum(cDataState::GetNumLayers());
	// обновляем статус-бар
	StatusBarUpdate();
	// изменяем состояния в поле рисования, изменяем текст ТВС и перерисовываем поле
	emit ChangeStatesCF(rState1, rState2);
	renderArea->ChangeText(cFA_Box::ActiveMode(), ui.spinBox->value());
	renderArea->repaint();
	// если есть гистограмма Kv - требуем перенастройку оси X и перерисовываем все
	if (KvDistrib)
	{
		emit resetAxisNeeded();
		emit select_FA(KvDistrib->currentFA());
	}
}
