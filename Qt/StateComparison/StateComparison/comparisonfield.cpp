#include <sstream>
#include <iomanip>
#include <QFileDialog>
#include <QDesktopWidget>
#include "comparisonfield.h"
#include "kv_distribution.h"
#include "extfunctions.h"

// �����������
ComparisonField::ComparisonField(QWidget *parent, const cDataState &rState1, const cDataState &rState2, const QRect &ScreenParameters)
	: QMainWindow(parent), Kv1SavedPen(Qt::white), Kv2SavedPen(Qt::white), KvSavedAutoAxis(AutoAxis::NoAuto)
{
	ui.setupUi(this);
	// �������������� ��������� �� ��������� ����� �������� �����������
	KvDistrib.reset(nullptr);
	ColorChanger.reset(nullptr);
	fileBrowser.reset(nullptr);
	// ������ �������� ��� SpinBox-� � ��������� ������
	ui.spinBox->setMaximum(cDataState::GetNumLayers());
	// �������� ������ � ������ ���� (� �.�. �����������) � ����������� �� �������� ������
	if (ScreenParameters.width() >= ScreenParameters.height())
	{
		this->setMinimumSize(0.6 * ScreenParameters.height(), 0.6 * ScreenParameters.height());
		this->resize(ScreenParameters.height()-150, ScreenParameters.height()-150); 
	} else {
		this->setMinimumSize(0.6 * ScreenParameters.width(), 0.6 * ScreenParameters.width());
		this->resize(ScreenParameters.width()-150, ScreenParameters.width()-150);
	}
	// ������� ���� � ����� ������
	this->move(ScreenParameters.center().x() - this->width()/2,50);

	// �������� ������� ������-����, ������� �� ��� Label, ������ ��� ��������� � ������������ � ��������� �����
	ui.statusBar->resize(this->width(), 20);
	statusLabel.reset(new QLabel(ui.statusBar));
	statusLabel->setGeometry(4, 0, this->width(), ui.statusBar->height());
	statusLabel->setAlignment(Qt::AlignVCenter);
	StatusBarUpdate();

	// ������� � ���� SpinBox � Label � ������� ���� � ������ Label ���������
	ui.groupBox->move( this->width() - (ui.groupBox->width() + 2) , this->height() - (ui.groupBox->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() - 15));
	ui.LayerLabel->setVisible(false);
	ui.LayerLabel->move( this->width() - (ui.LayerLabel->width() + 2) , this->height() - (ui.LayerLabel->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() - 5));
	// ��������� ������ ���� - ����� �������� ���������������� ���������� ���� ��� ��������� ������
	ComparisonField::setFixedHeight(ComparisonField::height());

	// ������� ���� ��� ���������
	renderArea.reset(new RenderArea(ui.centralWidget, 
									rState1,
									rState2,
									QSize(this->width(),this->height() - (ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() - 7))	
									));
	// ������������ ������� � ����� ��� ���� ��������� � ��������� ���� �����������
	// �������� ���������� ����������� Kv
	connect(renderArea.data(), SIGNAL(isKvDiagramActive()), this, SLOT(KvDiagramAvaliability()));
	// ������ ��� ���� ���������, ��� ����� ��������� ��� �������
	connect(this, SIGNAL(ViewModeChange(bool)), renderArea.data(), SLOT(ViewModeChanged(bool)));
	// ������ ��� ���� ���������, ��� ��������� ���� ��������
	connect(this, SIGNAL(ChangeStatesCF(const cDataState&, const cDataState&)), renderArea.data(), SLOT(StatesBinding(const cDataState&, const cDataState&)));
	// ��������� �� ������ ���� SpinBox � Label � ������� ���� (��� ���������)
	ui.LayerLabel->raise();
	ui.groupBox->raise();
	
	// ��������� ������� ��� ������������� � high dpi
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

// ����������
ComparisonField::~ComparisonField() 
{
	// ������������� - ������������ ������ (��-�� ����, ��� ���� �������� � ������� ��� ���������� ����������)
	KvDistrib.reset(nullptr);
	renderArea.reset(nullptr);
	statusLabel.reset(nullptr);
	ColorChanger.reset(nullptr);
	fileBrowser.reset(nullptr);
}

// �������� ������ ���������
void ComparisonField::on_otherStates_triggered()
{
	// �������� ������ ����������� ������
	fileBrowser.reset(new FileBrowser());
	// ������������ ������� � ����� ��� ������������ ������ � ��������� ���� �����������
	// ��������� � �������� ������������ ������
	connect(fileBrowser.data(),SIGNAL(closing()),this,SLOT(FileBrowserClosed()));
	// ������ �� ������������ ������ ��������� ������� �����������
	connect(fileBrowser.data(),SIGNAL(isComparisonFieldActive()),this,SLOT(ComparisonFieldAvaliability()));
	// ����� ������� - ���������� �������� ���������
	connect(fileBrowser.data(),SIGNAL(ChangeStatesFB(const cDataState&, const cDataState&)),this,SLOT(StatesChanged(const cDataState&, const cDataState&)));
	// ���������� ������������ ������
	fileBrowser->show();
}


// ��������/��������� �������� ����� ���� ���������
void ComparisonField::on_ColorFont_toggled(bool checked)
{
	// ������ ��� ��������/�����������
	cFA_Box::SetColorful(checked);
	// �������������� ���� ��� ���������
	renderArea->repaint();
}

// ��������� ������ ���������
void ComparisonField::on_ChangeViewMode_toggled(bool DeltaChecked)
{
	// �������� ������� � ���� ��� ��������� ��� ��������� ������� � ������
	emit ViewModeChange(DeltaChecked);	
	// ���������� ������-����
	StatusBarUpdate();
}

// ������ ����� Kq
void ComparisonField::on_activateKq_triggered()
{
	// ���������� ������ Kq, ��������� �� ��������
	ui.activateKq->setChecked(true);
	ui.activateBurn->setChecked(false);
	ui.activateKv->setChecked(false);
	// ���� ����� �� Kq, �� �������� ���������
	if (!(cFA_Box::ActiveMode()==Parameters::Kq))
	{
		// SpinBox �������� ����� ������ ���������
		ui.groupBox->setVisible(false);
		// ������ �������� �������� �� �������
		ui.MaxShow->setEnabled(false);
		// ������� ����� ���� ��������� �� �����, ��������������� Kq
		renderArea->ChangeText(Parameters::Kq, 0);
		// �������������� ����
		renderArea->repaint();

		// ���� ������� ����������� Kv - ������� �����, ��������������� ������ ����
		if (KvDistrib)
		{
			emit hideLayerLine();
		}
	}
}

void ComparisonField::on_activateBurn_triggered()
{
	// ���������� ������ Burn, ��������� �� ��������
	ui.activateKq->setChecked(false);
	ui.activateBurn->setChecked(true);
	ui.activateKv->setChecked(false);
	// ���� ����� �� Burn, �� �������� ���������
	if (!(cFA_Box::ActiveMode()==Parameters::Burn))
	{
		// SpinBox �������� ����� ������ ���������
		ui.groupBox->setVisible(false);
		// ������ �������� �������� �� �������
		ui.MaxShow->setEnabled(false);
		// ������� ����� ���� ��������� �� �����, ��������������� Burn
		renderArea->ChangeText(Parameters::Burn, 0);
		// �������������� ����
		renderArea->repaint();
		
		// ���� ������� ����������� Kv - ������� �����, ��������������� ������ ����
		if (KvDistrib)
		{
			emit hideLayerLine();
		}
	}
}

void ComparisonField::on_activateKv_triggered()
{
	// ���������� ������ Kv, ��������� �� ��������
	ui.activateKq->setChecked(false);
	ui.activateBurn->setChecked(false);
	ui.activateKv->setChecked(true);
	// ���� ����� �� Kv, �� �������� ���������
	if (!(cFA_Box::ActiveMode()==Parameters::Kv))
	{
		// SpinBox �������� ����� ������ �������
		ui.groupBox->setVisible(true);
		// ������ �������� �������� �������
		ui.MaxShow->setEnabled(true);
		// ������� ����� ���� ��������� �� �����, ��������������� Kv � ������� ������� ����
		renderArea->ChangeText(Parameters::Kv, ui.spinBox->value());
		// �������������� ����
		renderArea->repaint();

		// ���� ������� ����������� Kv - �������������� �� � ������ ����
		if (KvDistrib)
		{
			emit select_FA(KvDistrib->currentFA());
		}
	}
}


// ������ ������ "�������� ��������"
void ComparisonField::on_MaxShow_triggered()
{
	// ����� ���� ����� Kv
	if (cFA_Box::ActiveMode() != Parameters::Kv) return;
	// ������ �������� ���� = ���� � ������������ �������� Kv
	ui.spinBox->setValue(cFA_Box::FAwithMaxDeltaKv.Layer);
	// ���� ������� ����������� Kv
	if(KvDistrib)
	{
		// ������ ��� � ������������ �������� Kv
		emit select_FA(cFA_Box::FAwithMaxDeltaKv.FA);
	}
}

void ComparisonField::on_Kv_diagramActivator_triggered()
{
	if (!KvDistrib) 
	{
		// ������� ����� ����������� Kv
		KvDistrib.reset(new Kv_Distribution(Kv1SavedPen, Kv2SavedPen, KvSavedAutoAxis));
		// ������������ ������� � ����� ��� ����������� � ��������� ���� �����������
		connect(KvDistrib.data(), SIGNAL(closing(const QPen&, const QPen&, const AutoAxis::AutoAxisEnum&)), this, SLOT(KvDiagramClosed(const QPen&, const QPen&, const AutoAxis::AutoAxisEnum&)));
		// ����� ��� �� �����������
		connect(this, SIGNAL(select_FA(int)), KvDistrib.data(), SLOT(FA_selected(int)));
		// ������� ����� ����
		connect(this, SIGNAL(hideLayerLine()), KvDistrib.data(), SLOT(LayerLineHiding()));
		// ���������� ������������� ��� X �����������
		connect(this, SIGNAL(resetAxisNeeded(int)), KvDistrib.data(), SLOT(ResetAxisNeededTrue()));
		// ����� ��� �� ���� ����������� - ����������� ����������� Kv ��� �� �������
		connect(renderArea.data(), SIGNAL(select_FA(int)), KvDistrib.data(), SLOT(FA_selected(int)));
		// ��������� ������������ �������� �������� ��������� ����
		connect(KvDistrib.data(), &Kv_Distribution::getSpinBoxValue, [this] () { return ui.spinBox->value();});
		// ��������� ����������� ��������� �� ���� ���������
		connect(this, SIGNAL(sendRenderAreaPointer(RenderArea *)), KvDistrib.data(), SLOT(getRenderAreaPointer(RenderArea *)));
		
		// ���������� ����������� ��������� �� ���� ���������
		emit sendRenderAreaPointer(renderArea.data());

		// ������ ������� � ��������� ����������� �� ������
		QDesktopWidget Desktop;
		KvDistrib->setMinimumSize(0.176 * Desktop.screenGeometry(this).width(), 0.338 * Desktop.screenGeometry(this).height());
		KvDistrib->resize(0.218 * Desktop.screenGeometry(this).width(), 0.465 * Desktop.screenGeometry(this).height());
		KvDistrib->move(Desktop.screenGeometry(this).x() + Desktop.screenGeometry(this).width() - 0.026 * Desktop.screenGeometry(this).width() - KvDistrib->width(), 
							   0.046 * Desktop.screenGeometry(this).height());
		// ���������� �����������
		KvDistrib->show();
	}
}


// �������� �����������
void ComparisonField::on_Screenshot_triggered()
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
		// ���� ����� ����� ���� - ������ SpinBox � ���������� ������� � ������� ����
		bool LayersVisible = false;
		if (ui.groupBox->isVisible()) 
		{ 
			LayersVisible = true; 
			ui.groupBox->setVisible(false); 
			ui.LayerLabel->setText(QString::fromStdWString( L"���� " + ExtFunctions::to_wstring(ui.spinBox->value()) )); 
			ui.LayerLabel->setVisible(true);
		}
		// ��������� �����������
		QPixmap screen = QPixmap::grabWidget(ui.centralWidget);
		// ���� ��� ����� ����� ���� - ���������� SpinBox ������ ����
		if (LayersVisible) { ui.groupBox->setVisible(true); ui.LayerLabel->setVisible(false);}
		// �������� �������� �������� - ���������� ������� ��� ������ ��������
		screen = screen.scaled(ui.centralWidget->width()*4,ui.centralWidget->height()*4,Qt::KeepAspectRatio,Qt::SmoothTransformation);
		// ��������� ������� � ����
		screen.save(ScreenFileName, 0, 100);
	}
}

// ������� ���� ��������� �����������
void ComparisonField::on_ChangeColors_triggered()
{
	// ���� ��� ���
	if (!ColorChanger)
	{
		// �� ������� ����� ����
		ColorChanger.reset(new CF_ColorChanger());
		// ������������ ������� � ����� ��� ���� ��������� � ��������� ���� �����������
		// ��������� � �������� ����������� �����������
		connect(ColorChanger.data(), SIGNAL(closing()), this, SLOT(ColorChangerClosed()));
		// ���������� ������������ �����������
		connect(ColorChanger.data(), SIGNAL(RecolorComparisonField()), this, SLOT(RecolorAll()));
		// ��������� ��������� ��������� - ����������� �����������
		connect(ColorChanger.data(), &CF_ColorChanger::changeActiveMode, [this] (const Parameters::ParametersEnum& newActiveMode) 
				{
					// ��� ������ ������ ������ � ���� ��������� ����� ����������� ����� � � ���� �����������
					switch(newActiveMode) {
					case Parameters::Kq   : ui.activateKq->trigger();
										    break;
					case Parameters::Burn : ui.activateBurn->trigger();
										    break;
					case Parameters::Kv   : ui.activateKv->trigger();
										    break;
					}
				});
		// ���������� ���� ���������
		ColorChanger->show();
	}
}

// ����������� ��� ���
void ComparisonField::RecolorAll(void)
{
	// �������� ����� ���� ��� - ��������� ��� ��������� ������ �����������
	renderArea->ChangeText(cFA_Box::ActiveMode(), ui.spinBox->value());
	// �������������� ����
	renderArea->repaint();
}

// ��������� �������� ������ ���� 
void ComparisonField::on_spinBox_valueChanged(int newValue)
{
	// ���� ������� ����� Kv - �������� ����� �� ���� ���������, ����� ��� ��������������� ������ ��������� ����, � �������������� ���
	if (cFA_Box::ActiveMode() == Parameters::Kv)
	{
		renderArea->ChangeText(Parameters::Kv, ui.spinBox->value());
		renderArea->repaint();
	}
	// ���� ������� ����������� Kv - �������������� �� (����� ���������� �����, ������������ ����)
	if (KvDistrib)
	{
		emit select_FA(KvDistrib->currentFA());
	}
}

// ���������� ������-���� - � ����������� �� ������ ����������� ��������� ������ ���������, ���������� �������� ������ ���������
void ComparisonField::StatusBarUpdate(void)
{
	if (cFA_Box::ViewMode() == View::DeltaView)
	{
		statusLabel->setText(QString::fromWCharArray(L"��������: ") + State1FileName() + 
							 QString::fromWCharArray(L" - ") + State2FileName());
	} else if (cFA_Box::ViewMode() == View::TwoStatesView) {
		statusLabel->setText(QString::fromWCharArray(L"�������� 1: ") + State1FileName() + 
							 QString::fromWCharArray(L", �������� 2: ") + State2FileName());
	}
}

// ��������� �������� ���� - ����������������
void ComparisonField::resizeEvent(QResizeEvent * res)
{
	// �������� ������ ������ - ������� ������ ����� ������������� ������, ��� ���� �������� �� ���������
	this->setFixedHeight(this->width());
	// ������� � ���� SpinBox � Label � ������� ���� � ������������ ������� ���� ��� ���������
	ui.LayerLabel->move( this->width() - (ui.LayerLabel->width() + 2) , this->height() - (ui.LayerLabel->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() + 2));
	ui.groupBox->move( this->width() - (ui.groupBox->width() + 2) , this->height() - (ui.groupBox->height() + ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height() + 2));
	renderArea->resize( this->width(), this->height() - (ui.statusBar->height() + ui.mainToolBar->height()+ ui.menuBar->height()));
}

// �������� ���������� ����������� Kv
bool ComparisonField::KvDiagramAvaliability() const
{ 
	// ��������� ��������� true, ���� � ���� ���� ��������, �� ������� �� ��������� 
	if (KvDistrib) { return true;} 
	else {return false;} 
}

// �������� ����������� Kv - �������� ��������� �� ���� � ��������� ��������� ��������� � ������������� ��� X
void ComparisonField::KvDiagramClosed(const QPen& Kv1PenToSave, const QPen& Kv2PenToSave, const AutoAxis::AutoAxisEnum& KvAutoAxisToSave)
{
	Kv1SavedPen = Kv1PenToSave;
	Kv2SavedPen = Kv2PenToSave;
	KvSavedAutoAxis = KvAutoAxisToSave;
	KvDistrib.reset(nullptr);
}

// �������� ���� ��������� ����������� - �������� ��������� �� ����
void ComparisonField::ColorChangerClosed()
{
	ColorChanger.reset(nullptr);
}

// �������� ������������ ������ - �������� ��������� �� ����
void ComparisonField::FileBrowserClosed()
{
	fileBrowser.reset(nullptr);
}

// ���� ���� ���� - ������ true
bool ComparisonField::ComparisonFieldAvaliability() const
{
	return true;
}

// �������� ��������� - ������� ����� ���������
void ComparisonField::StatesChanged(const cDataState& rState1, const cDataState& rState2)
{
	// ������ ����� �������� ���������� ������� �����
	ui.spinBox->setMaximum(cDataState::GetNumLayers());
	// ��������� ������-���
	StatusBarUpdate();
	// �������� ��������� � ���� ���������, �������� ����� ��� � �������������� ����
	emit ChangeStatesCF(rState1, rState2);
	renderArea->ChangeText(cFA_Box::ActiveMode(), ui.spinBox->value());
	renderArea->repaint();
	// ���� ���� ����������� Kv - ������� ������������� ��� X � �������������� ���
	if (KvDistrib)
	{
		emit resetAxisNeeded();
		emit select_FA(KvDistrib->currentFA());
	}
}