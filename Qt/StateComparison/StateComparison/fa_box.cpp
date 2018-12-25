#include <sstream>
#include <iomanip>
#include <cmath>
#include "fa_box.h"

// задаем начальные значения ТВС и слоя с максимальным Kv 
cFA_Box::FAandLayer cFA_Box::FAwithMaxDeltaKv;

// задаем начальные значения активного параметра и режима просмотра
Parameters::ParametersEnum cFA_Box::activeMode = Parameters::Kv;
View::ViewEnum cFA_Box::viewMode = View::TwoStatesView;
 
// инициализация словаря предельных значений
std::map<Parameters::ParametersEnum, cFA_Box::Lim> cFA_Box::Limiters;

// задание стандартных значений предельных отклонений
void cFA_Box::SetDefaultLimiters()
{
	Limiters[Parameters::Kq].maxValue = 1.0;
	Limiters[Parameters::Kq].minValue = -1.0;
	Limiters[Parameters::Burn].maxValue = 1.0;
	Limiters[Parameters::Burn].minValue = -1.0;
	Limiters[Parameters::Kv].maxValue = 1.0;
	Limiters[Parameters::Kv].minValue = -1.0;
}

// задание предельных отклонений для конкретного параметра
void cFA_Box::SetLimiters(const Parameters::ParametersEnum& parameter, const double &max)
{	
	// значение должно быть != 0 - чуть больше, т.к. в алгоритме есть деление на это значение
	if ( abs(max) > pow(10.0, -7) ) 
	{
		Limiters[parameter].maxValue = abs(max);
		Limiters[parameter].minValue = -abs(max);
	}
}

// задание стандартного значения наличия цвета у поля и методов установки и получения этого значения
bool cFA_Box::Colorful = false;
bool cFA_Box::isColorful() { return cFA_Box::Colorful;}
void cFA_Box::SetColorful(bool colorful) { cFA_Box::Colorful = colorful;}

// инициализация словарей стандартного и текущего значения цветов максимальных отклонений
std::map<Parameters::ParametersEnum, cFA_Box::LimColors> cFA_Box::LimitColors;
std::map<Parameters::ParametersEnum, cFA_Box::LimColors> cFA_Box::defaultLimitColors;

// задание стандартных значений цветов максимальных отклонений
void cFA_Box::SetDefaultLimitColors()
{
	defaultLimitColors[Parameters::Kq].maxColor = QColor(158,0,91);
	defaultLimitColors[Parameters::Kq].minColor = QColor(0,91,158);
	defaultLimitColors[Parameters::Burn].maxColor = QColor(155,121,0);
	defaultLimitColors[Parameters::Burn].minColor = QColor(121,0,155);
	defaultLimitColors[Parameters::Kv].maxColor = QColor(255,0,0);
	defaultLimitColors[Parameters::Kv].minColor = QColor(0,255,0);
}

// задание цветов из стандартных значений
void cFA_Box::SetLimitColorsFromDefaults()
{
	LimitColors[Parameters::Kq].maxColor = defaultLimitColors.at(Parameters::Kq).maxColor; // QColor(158,0,91);
	LimitColors[Parameters::Kq].minColor = defaultLimitColors.at(Parameters::Kq).minColor; // QColor(0,91,158);
	LimitColors[Parameters::Burn].maxColor = defaultLimitColors.at(Parameters::Burn).maxColor; //QColor(155,121,0);
	LimitColors[Parameters::Burn].minColor = defaultLimitColors.at(Parameters::Burn).minColor; //QColor(121,0,155);
	LimitColors[Parameters::Kv].maxColor = defaultLimitColors.at(Parameters::Kv).maxColor; //QColor(255,0,0);
	LimitColors[Parameters::Kv].minColor = defaultLimitColors.at(Parameters::Kv).minColor; //QColor(0,255,0);
}

// получение стандартных и текущих цветов максимальных отклонений
QColor cFA_Box::GetDefaultMinColor(const Parameters::ParametersEnum& parameter) { return defaultLimitColors.at(parameter).minColor;}
QColor cFA_Box::GetDefaultMaxColor(const Parameters::ParametersEnum& parameter) { return defaultLimitColors.at(parameter).maxColor;}

QColor cFA_Box::GetMinColor(const Parameters::ParametersEnum& parameter) { return LimitColors.at(parameter).minColor;}
QColor cFA_Box::GetMaxColor(const Parameters::ParametersEnum& parameter) { return LimitColors.at(parameter).maxColor;}

// установка новых цветов максимальных отклонений
void cFA_Box::SetMinColor(const Parameters::ParametersEnum& parameter, const QColor& NewMinColor) { LimitColors[parameter].minColor = NewMinColor;}
void cFA_Box::SetMaxColor(const Parameters::ParametersEnum& parameter, const QColor& NewMaxColor) { LimitColors[parameter].maxColor = NewMaxColor;}

// копирующий конструктор - копирование всех параметров
cFA_Box::cFA_Box(const cFA_Box &rFA) : titleFA(rFA.titleFA)
{
	Parent = rFA.Parent;
	FA_Number.setParent(Parent);
	Delta.setParent(Parent);
	State1.setParent(Parent);
	State2.setParent(Parent);
	FA_Number.setText(rFA.FA_Number.text());
	Delta.setText(rFA.Delta.text());
	State1.setText(rFA.State1.text());
	State2.setText(rFA.State2.text());
	_width = rFA._width;
	_height = rFA._height;
	center.setX(rFA.center.x());
	center.setY(rFA.center.y());
	Color = rFA.Color;
	Visible = rFA.Visible;
	recalculate();
	if (Limiters.empty()) SetDefaultLimiters();
	if (defaultLimitColors.empty()) SetDefaultLimitColors();
	if (LimitColors.empty()) SetLimitColorsFromDefaults();
}

// копирующий оператор= - копирование всех параметров
cFA_Box& cFA_Box::operator=(const cFA_Box &rFA)
{
	Parent = rFA.Parent;
	FA_Number.setParent(Parent);
	Delta.setParent(Parent);
	State1.setParent(Parent);
	State2.setParent(Parent);
	FA_Number.setText(rFA.FA_Number.text());
	Delta.setText(rFA.Delta.text());
	State1.setText(rFA.State1.text());
	State2.setText(rFA.State2.text());
	_width = rFA._width;
	_height = rFA._height;
	center.setX(rFA.center.x());
	center.setY(rFA.center.y());
	Color = rFA.Color;
	Visible = rFA.Visible;
	recalculate();
	if (Limiters.empty()) SetDefaultLimiters();
	if (defaultLimitColors.empty()) SetDefaultLimitColors();
	if (LimitColors.empty()) SetLimitColorsFromDefaults();
	return (*this);
}

// устанавливаем значение номера ТВС из значения типа int
void cFA_Box::SetFA_Number(const int &newFA_Number) 
{ 
	// ТВС не должна быть легендой
	if (!titleFA) {
		// задаем текст в виде целого числа
		std::stringstream ss(""); 
		ss << std::fixed << newFA_Number; 
		FA_Number.setText(QString::fromStdString(ss.str()));
	}
}

// установить новый текст для разности состояний из значения типа double
void cFA_Box::SetDelta(const double &newDelta) 
{ 
	// ТВС не должна быть легендой
	if (!titleFA) {
		// задаем значение с точкой и 3 знаками после запятой
		std::stringstream ss(""); 
		ss << std::fixed << std::setprecision(3) << newDelta;
		Delta.setText(QString::fromStdString(ss.str()));
		// задаем новый цвет ТВС исходя из значения разности
		recolor(newDelta);
	}
}

// установить новый текст для состояния 1 из значения типа double
void cFA_Box::SetState1(const double &newState1) 
{ 
	// ТВС не должна быть легендой
	if (!titleFA) {
		// задаем значение с точкой и 2 знаками после запятой
		std::stringstream ss(""); 
		ss << std::fixed << std::setprecision(2) << newState1;
		State1.setText(QString::fromStdString(ss.str()));
	}
}

// установить новый текст для состояния 2 из значения типа double
void cFA_Box::SetState2(const double &newState2) 
{
	// ТВС не должна быть легендой
	if (!titleFA) {
		// задаем значение с точкой и 2 знаками после запятой
		std::stringstream ss(""); 
		ss << std::fixed << std::setprecision(2) << newState2;
		State2.setText(QString::fromStdString(ss.str()));
	}
}

// получение текущего цвета ТВС
QColor cFA_Box::GetColor() const 
{ 
	// если поле цветное - возвращаем цвет ТВС, иначе - белый
	if (cFA_Box::isColorful()) 
	{ 
		return Color;
	} else { 
		return QColor(255,255,255);
	}
}

// изменение режима просмотра - осуществлятся в методах recalculate и SetVisible
void cFA_Box::ChangeView(void)
{
	recalculate();
	this->SetVisible(this->isVisible());
}

void cFA_Box::SetVisible(bool visible) 
{ 
	Visible = visible;
	FA_Number.setVisible(visible); 
	if (viewMode == View::DeltaView) 
	{
		Delta.setVisible(visible);
	} else {
		Delta.setVisible(false);
	}
	if (viewMode == View::TwoStatesView)
	{
		State1.setVisible(visible);
		State2.setVisible(visible);
	} else {
		State1.setVisible(false);
		State2.setVisible(false);
	}
}

// установка новых значений ширины, высоты и центра ТВС, перерасчет параметров
void cFA_Box::SetGeometry(int newCenterX, int newCenterY, int newWidth, int newHeight) 
{
	center.setX(newCenterX);
	center.setY(newCenterY);
	_width = newWidth;
	_height = newHeight;
	recalculate();
}

// установка нового значения центра ТВС, перерасчет параметров
void cFA_Box::move(int newCenterX, int newCenterY)
{
	center.setX(newCenterX);
	center.setY(newCenterY);
	recalculate();
}

// установка новых значений ширины и высоты ТВС, перерасчет параметров
void cFA_Box::resize(int newWidth, int newHeight)
{
	_width = newWidth;
	_height = newHeight;
	recalculate();
}

// перерасчет геометрии ТВС
void cFA_Box::recalculate(void)
{
	// отрисовка контура ТВС
	painterPath = QPainterPath();
	painterPath.moveTo(center.x()-_width/2,center.y()-_height/4);
	painterPath.lineTo(center.x(),center.y()-_height/2);
	painterPath.lineTo(center.x()+_width/2,center.y()-_height/4);
	painterPath.lineTo(center.x()+_width/2,center.y()+_height/4);
	painterPath.lineTo(center.x(),center.y()+_height/2);
	painterPath.lineTo(center.x()-_width/2,center.y()+_height/4);
	painterPath.closeSubpath();
	// задание геометрии Label-а с номером ТВС
	FA_Number.setGeometry(center.x()-7*_width/48, center.y()-5*_height/12, 7*_width/24, _height/6); 
	// расчет геометрии необходимых Label-ов в зависимости от режима просмотра
	if (viewMode == View::DeltaView) 
	{
		Delta.setGeometry(center.x()-5*_width/12, center.y()-7*_height/48, 5*_width/6, 7*_height/24);
	}
	if (viewMode == View::TwoStatesView)
	{
		State1.setGeometry(center.x()-5*_width/12, center.y()-10*_height/48, 5*_width/6, 11*_height/48);
		State2.setGeometry(center.x()-5*_width/12, center.y()+3*_height/48, 5*_width/6, 11*_height/48);
	}
	// задание шрифтов - в зависимости от того, является ли ТВС легендой
	if (!titleFA) {
		SetLabelParameters();
	} else {
		SetTitleLabelParameters();
	}
}

// изменение цвета отрисовки ТВС в соответствии со значением
void cFA_Box::recolor(const double &newValue)
{
	// если значение больше максимального или меньше минимального (= по модулю максимальному, но со знаком -), то устанавливаем цвет максимума/минимума 
	if ( newValue >= Limiters.at(activeMode).maxValue ) { SetColor(GetMaxColor(activeMode)); return;}
	if ( newValue <= Limiters.at(activeMode).minValue ) { SetColor(GetMinColor(activeMode)); return;}
	// если ноль - то цвет белый
	if ( newValue == 0 ) { SetColor(Qt::white); return;}
	// если значение больше 0 (состояние 1 > состояния 2)
	if ( newValue > 0 ) 
	{
		// новые значения цветов
		int r, g, b;
		// цвет максимума
		QColor Max = GetMaxColor(activeMode);
		// вычисляем новое значение цвета - точка на линии [(Rmax, Gmax, Bmax); Max] - [(255,255,255), 0] пропорционально значению
		r = Max.red() + ( 255 - Max.red() ) * (Limiters[activeMode].maxValue - newValue) / Limiters[activeMode].maxValue;
		g = Max.green() + ( 255 - Max.green() ) * (Limiters[activeMode].maxValue - newValue) / Limiters[activeMode].maxValue;
		b = Max.blue() + ( 255 - Max.blue() ) * (Limiters[activeMode].maxValue - newValue) / Limiters[activeMode].maxValue;
		// выставляем вычисленный цвет
		SetColor(QColor(r, g, b));
	// если значение меньше 0 (состояние 1 < состояния 2)
	} else {
		// новые значения цветов
		int r, g, b;
		// цвет минимума
		QColor Min = GetMinColor(activeMode);
		// вычисляем новое значение цвета - точка на линии [(Rmin, Gmin, Bmin); Min] - [(255,255,255), 0] пропорционально значению
		r = Min.red() + ( 255 - Min.red() ) * (Limiters[activeMode].minValue - newValue) / Limiters[activeMode].minValue;
		g = Min.green() + ( 255 - Min.green() ) * (Limiters[activeMode].minValue - newValue) / Limiters[activeMode].minValue;
		b = Min.blue() + ( 255 - Min.blue() ) * (Limiters[activeMode].minValue - newValue) / Limiters[activeMode].minValue;
		// выставляем вычисленный цвет
		SetColor(QColor(r, g, b));
	}
}

// выравнивание текста всегда посередине, установка значений шрифтов для нужных Label-ов (в зависимости от режима просмотра)
void cFA_Box::SetLabelParameters(void)
{
	// FA_Number
	FA_Number.setAlignment(Qt::AlignCenter);
	QFont font("MS Shell Dlg 2",8,50,false);
	if (this->FA_Number.height() <= 8) {font.setPixelSize(8);}
	else if (this->FA_Number.height() <= 10) {font.setPixelSize(9);}
	else if (this->FA_Number.height() <= 11) {font.setPixelSize(10);}
	else if (this->FA_Number.height() <= 13) {font.setPixelSize(11);}
	else if (this->FA_Number.height() <= 15) {font.setPixelSize(13);}
	else if (this->FA_Number.height() <= 17) {font.setPixelSize(15);}
	else {font.setPixelSize(16);}
	FA_Number.setFont(font);
	if (viewMode == View::DeltaView) {
		// DeltaLabel
		Delta.setAlignment(Qt::AlignCenter);
		if (this->Delta.height() <= 8) {font.setPixelSize(8);}
		else if (this->Delta.height() <= 9) {font.setPixelSize(9);}
		else if (this->Delta.height() <= 10) {font.setPixelSize(10);}
		else if (this->Delta.height() <= 11) {font.setPixelSize(11);}
		else if (this->Delta.height() <= 12) {font.setPixelSize(12);}
		else if (this->Delta.height() <= 13) {font.setPixelSize(13);}
		else if (this->Delta.height() <= 14) {font.setPixelSize(14);}
		else if (this->Delta.height() <= 16) {font.setPixelSize(15);}
		else if (this->Delta.height() <= 18) {font.setPixelSize(16);}
		else if (this->Delta.height() <= 21) {font.setPixelSize(17);}
		else if (this->Delta.height() <= 24) {font.setPixelSize(19);}
		else {font.setPixelSize(21);}
		Delta.setFont(font);
	}
	if (viewMode == View::TwoStatesView) {
		// State1Label
		State1.setAlignment(Qt::AlignCenter);
		if (this->State1.height() <= 8) {font.setPixelSize(8);}
		else if (this->State1.height() <= 9) {font.setPixelSize(9);}
		else if (this->State1.height() <= 10) {font.setPixelSize(10);}
		else if (this->State1.height() <= 11) {font.setPixelSize(11);}
		else if (this->State1.height() <= 12) {font.setPixelSize(12);}
		else if (this->State1.height() <= 13) {font.setPixelSize(13);}
		else if (this->State1.height() <= 14) {font.setPixelSize(14);}
		else if (this->State1.height() <= 16) {font.setPixelSize(15);}
		else if (this->State1.height() <= 18) {font.setPixelSize(16);}
		else if (this->State1.height() <= 21) {font.setPixelSize(17);}
		else {font.setPixelSize(19);}
		State1.setFont(font);
		// State2Label
		State2.setAlignment(Qt::AlignCenter);
		if (this->State2.height() <= 8) {font.setPixelSize(8);}
		else if (this->State2.height() <= 9) {font.setPixelSize(9);}
		else if (this->State2.height() <= 10) {font.setPixelSize(10);}
		else if (this->State2.height() <= 11) {font.setPixelSize(11);}
		else if (this->State2.height() <= 12) {font.setPixelSize(12);}
		else if (this->State2.height() <= 13) {font.setPixelSize(13);}
		else if (this->State2.height() <= 14) {font.setPixelSize(14);}
		else if (this->State2.height() <= 16) {font.setPixelSize(15);}
		else if (this->State2.height() <= 18) {font.setPixelSize(16);}
		else if (this->State2.height() <= 21) {font.setPixelSize(17);}
		else {font.setPixelSize(19);}
		State2.setFont(font);
	}
}

// ТВС-легенда: выравнивание текста всегда посередине, установка значений шрифтов для нужных Label-ов (в зависимости от режима просмотра)
void cFA_Box::SetTitleLabelParameters(void)
{
	// FA_Number
	FA_Number.setAlignment(Qt::AlignCenter);
	QFont font("MS Shell Dlg 2",8,50,false);
	if (this->FA_Number.height() <= 7) {font.setPixelSize(8);}
	else if (this->FA_Number.height() <= 9) {font.setPixelSize(9);}
	else if (this->FA_Number.height() <= 10) {font.setPixelSize(10);}
	else if (this->FA_Number.height() <= 12) {font.setPixelSize(11);}
	else if (this->FA_Number.height() <= 15) {font.setPixelSize(12);}
	else if (this->FA_Number.height() <= 18) {font.setPixelSize(13);}
	else if (this->FA_Number.height() <= 21) {font.setPixelSize(15);}
	else if (this->FA_Number.height() <= 24) {font.setPixelSize(16);}
	else {font.setPixelSize(17);}
	FA_Number.setFont(font);
	if (viewMode == View::DeltaView) {
		// DeltaLabel
		Delta.setAlignment(Qt::AlignCenter);
		if (this->Delta.height() <= 9) {font.setPixelSize(7);}
		else if (this->Delta.height() <= 11) {font.setPixelSize(8);}
		else if (this->Delta.height() <= 14) {font.setPixelSize(9);}
		else if (this->Delta.height() <= 16) {font.setPixelSize(10);}
		else if (this->Delta.height() <= 18) {font.setPixelSize(11);}
		else if (this->Delta.height() <= 20) {font.setPixelSize(12);}
		else if (this->Delta.height() <= 22) {font.setPixelSize(13);}
		else if (this->Delta.height() <= 24) {font.setPixelSize(14);}
		else if (this->Delta.height() <= 26) {font.setPixelSize(15);}
		else if (this->Delta.height() <= 29) {font.setPixelSize(16);}
		else if (this->Delta.height() <= 32) {font.setPixelSize(17);}
		else if (this->Delta.height() <= 35) {font.setPixelSize(19);}
		else {font.setPixelSize(21);}
		Delta.setFont(font);
	}
	if (viewMode == View::TwoStatesView) {
		// State1Label
		State1.setAlignment(Qt::AlignCenter);
		if (this->State1.height() <= 9) {font.setPixelSize(7);}
		else if (this->State1.height() <= 11) {font.setPixelSize(8);}
		else if (this->State1.height() <= 14) {font.setPixelSize(9);}
		else if (this->State1.height() <= 16) {font.setPixelSize(10);}
		else if (this->State1.height() <= 18) {font.setPixelSize(11);}
		else if (this->State1.height() <= 20) {font.setPixelSize(12);}
		else if (this->State1.height() <= 22) {font.setPixelSize(13);}
		else if (this->State1.height() <= 24) {font.setPixelSize(14);}
		else if (this->State1.height() <= 26) {font.setPixelSize(15);}
		else if (this->State1.height() <= 29) {font.setPixelSize(16);}
		else if (this->State1.height() <= 32) {font.setPixelSize(17);}
		else {font.setPixelSize(19);}
		State1.setFont(font);
		// State2Label
		State2.setAlignment(Qt::AlignCenter);
		if (this->State2.height() <= 9) {font.setPixelSize(7);}
		else if (this->State2.height() <= 11) {font.setPixelSize(8);}
		else if (this->State2.height() <= 14) {font.setPixelSize(9);}
		else if (this->State2.height() <= 16) {font.setPixelSize(10);}
		else if (this->State2.height() <= 18) {font.setPixelSize(11);}
		else if (this->State2.height() <= 20) {font.setPixelSize(12);}
		else if (this->State2.height() <= 22) {font.setPixelSize(13);}
		else if (this->State2.height() <= 24) {font.setPixelSize(14);}
		else if (this->State2.height() <= 26) {font.setPixelSize(15);}
		else if (this->State2.height() <= 29) {font.setPixelSize(16);}
		else if (this->State2.height() <= 32) {font.setPixelSize(17);}
		else {font.setPixelSize(19);}
		State2.setFont(font);
	}
}