#ifndef FA_BOX_H
#define FA_BOX_H

/*
 * class cFA_Box
 *
 * Version 1.10
 *
 * Writed by Brylkin Dmitry. 03.12.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QCoreApplication>
#include <QLabel>
#include <QPainterPath>
#include <map>

/*
 *  Перечисление параметров, отображаемых в программе
 */
namespace  Parameters {
	enum ParametersEnum {
		Kq = 0,
		Burn = 1,
		Kv = 2
	};
}

/*
 * Перечисление режимов вывода данных на картограмме
 */
namespace View {
	enum ViewEnum {
		DeltaView = 0,
		TwoStatesView = 1
	};
}

/*
 * Класс cFA_Box
 *
 * ТВС, отображающаяся на поле для рисования
 *
 * Хранит в себе все поля и методы управления
 * ТВС на поле.
 */
class cFA_Box{

    Q_DECLARE_TR_FUNCTIONS(cFA_Box)

private : 
	// ПОЛЯ
    // рамка ТВС для отрисовки
	QPainterPath painterPath;
	// надпись с номером ТВС
	QLabel FA_Number;
	// текст с разницей значений
	QLabel Delta;
	// текст с первым состоянием
	QLabel State1;
	// текст со вторым состоянием
	QLabel State2;
	// ширина и высота ТВС на плоскости
	int _width;
	int _height;
    // отрисовывается ли ТВС
    bool Visible;
	// координаты центра ТВС
	QPoint center;
	// цвет ТВС
	QColor Color;
	// родитель ТВС
	QWidget * Parent;
	// является ли ТВС - ТВС с легендой
	const bool titleFA;
	
	// включено ли отображение цветов
	static bool Colorful;
	// активный параметр
	static Parameters::ParametersEnum activeMode;
	// активный режим просмотра
	static View::ViewEnum viewMode;
	
	// вспомогательная структура - максимальное и минимальное значения
	struct Lim {
		double maxValue;
		double minValue;
	};
	// вспомогательная структура - значения цветов максимума и минимума
	struct LimColors {
		QColor maxColor;
		QColor minColor;
	};

	// предельные значения параметров - после их превышения цвет не меняется
	static std::map<Parameters::ParametersEnum, Lim> Limiters;
	// цвета пределов
	static std::map<Parameters::ParametersEnum, LimColors> LimitColors;
	// цвета пределов по умолчанию
	static std::map<Parameters::ParametersEnum, LimColors> defaultLimitColors;

	// МЕТОДЫ
	// установить пределы по умолчанию
	static void SetDefaultLimiters();
	// установить цвета пределов по умолчанию
	static void SetDefaultLimitColors();
	// установить цвет ТВС
	void SetColor(QColor newColor) {Color = newColor;}
	// пересчитать все
	void recalculate(void);
	// перекрасить ТВС в соответствии со значением
	void recolor(const double &newValue);
	// установить параметры текста в ТВС
	void SetLabelParameters(void);
	// установить параметры текста в ТВС, если она - ТВС с легендой
	void SetTitleLabelParameters(void);
public :
	// конструкторы
	cFA_Box(int x_cent = 0, int y_cent = 0, 
			int newWidth = 0, int newHeight = 0, 
			int newFA_Number = 0, QWidget *parent = 0) : 
				FA_Number(parent), Delta(parent),
				State1(parent), State2(parent),
				_width(newWidth), _height(newHeight), Visible(false),
				center(x_cent, y_cent), Color(Qt::gray), Parent(parent), titleFA(false)
				{ 
					recalculate(); 
					if (Limiters.empty()) SetDefaultLimiters(); 
					if (defaultLimitColors.empty()) SetDefaultLimitColors();
					if (LimitColors.empty()) SetLimitColorsFromDefaults();
					SetFA_Number(newFA_Number);
				}
	cFA_Box(int x_cent, int y_cent, 
			int newWidth, int newHeight,
			int newFA_Number, const QString& newDeltaText,
            QWidget *parent = 0)
                : cFA_Box(x_cent, y_cent, newWidth, newHeight, newFA_Number, parent)
                {
                    SetDelta(newDeltaText);
                }
	cFA_Box(int x_cent, int y_cent, 
			int newWidth, int newHeight,
			int newFA_Number, double newDelta,
            QWidget *parent = 0)
                : cFA_Box(x_cent, y_cent, newWidth, newHeight, newFA_Number, parent)
                {
                    SetDelta(newDelta);
                }
	cFA_Box(bool _titleFA, int x_cent = 0, int y_cent = 0, 
            int newWidth = 0, int newHeight = 0, QWidget *parent = 0) :
				FA_Number(parent), Delta(parent),
				State1(parent), State2(parent),
				_width(newWidth), _height(newHeight), Visible(false),
				center(x_cent, y_cent), Color(Qt::white), Parent(parent), titleFA(_titleFA)
				{
					recalculate(); 
					if (Limiters.empty()) SetDefaultLimiters(); 
					if (defaultLimitColors.empty()) SetDefaultLimitColors();
					if (LimitColors.empty()) SetLimitColorsFromDefaults();
					if (titleFA) {
                        resetTitleFAText();
                        SetColor(Qt::white);
					}
				}
	cFA_Box(const cFA_Box &rFA);
	cFA_Box& operator=(const cFA_Box &rFA);
	// деструктор
	~cFA_Box() {}
	// получение данных
	int x_center() const {return center.x();}
	int y_center() const {return center.y();}
	int width() const {return _width;}
	int height() const {return _height;}
	int GetFA_Number() const {if (!titleFA) return stoi(FA_Number.text().toStdString()); else return 0;}
	QString delta() const {return Delta.text();}
	QString state1() const {return State1.text();}
	QString state2() const {return State2.text();}
	QPainterPath GetPainterPath() const {return painterPath;}
	QColor GetColor() const;
	QWidget * GetParent() { return Parent;}
	bool isVisible() const { return Visible;}
	static bool isColorful();
	static QColor GetMinColor(const Parameters::ParametersEnum& parameter);
	static QColor GetMaxColor(const Parameters::ParametersEnum& parameter);
	static QColor GetDefaultMinColor(const Parameters::ParametersEnum& parameter);
	static QColor GetDefaultMaxColor(const Parameters::ParametersEnum& parameter);
	static Parameters::ParametersEnum ActiveMode() {return activeMode;}
	static View::ViewEnum ViewMode() {return viewMode;}
	// установка новых данных
	void SetCenterX(int newCenterX) {center.setX(newCenterX);}
	void SetCenterY(int newCenterY) {center.setY(newCenterY);}
	void SetWidth(int newWidth) {_width = newWidth;}
	void SetHeight(int newHeight) {_height = newHeight;}
	void SetFA_Number(const int &newFA_Number);
	// установка разности состояний из текста, если ТВС не легенда. Перекрашиваем ТВС в серый (т.к. числового значения нет, т.е. в основном NaN)
	void SetDelta(const QString &newDeltaText) { if (!titleFA) { Delta.setText(newDeltaText); this->SetColor(Qt::gray); } }
	void SetDelta(const double &newDelta);
	// установка состояние 1 из текста, если ТВС не легенда
	void SetState1(const QString &newState1Text) { if (!titleFA) State1.setText(newState1Text);}
	void SetState1(const double &newState1);
	// установка состояние 2 из текста, если ТВС не легенда
	void SetState2(const QString &newState2Text) { if (!titleFA) State2.setText(newState2Text);}
	void SetState2(const double &newState2);
	// установка родителей для всех Label-ов и нового значения переменной
	void SetParent(QWidget * parent) {Parent = parent; Delta.setParent(Parent); FA_Number.setParent(Parent); State1.setParent(Parent); State2.setParent(Parent);}
	void SetVisible(bool visible);
    void resetTitleFAText(void);
	static void SetColorful(bool colorful);
	static void SetMinColor(const Parameters::ParametersEnum& parameter, const QColor& NewMinColor);
	static void SetMaxColor(const Parameters::ParametersEnum& parameter, const QColor& NewMaxColor);
	static void SetLimiters(const Parameters::ParametersEnum& parameter, const double &max);
	static void SetActiveMode(const Parameters::ParametersEnum& newActiveMode) {activeMode = newActiveMode;}
	static void SetViewMode(const View::ViewEnum& newViewMode) {viewMode = newViewMode;}
	static void SetLimitColorsFromDefaults();
	// изменить вид на текущий
	void ChangeView(void);
	// установить геометрию
	void SetGeometry(int newCenterX, int newCenterY, int newWidth, int newHeight);
	// передвинуть ТВС
	void move(int newCenterX, int newCenterY);
	// изменить размер ТВС
	void resize(int newWidth, int newHeight);

	// ПУБЛИЧНЫЕ ПОЛЯ
	// вспомогательная публичная структура - ТВС и слой
	struct FAandLayer {
		FAandLayer() : FA(1), Layer(1) {}
		int FA;
		int Layer;
	};
	// ТВС и слой с максимальной разницей Kv
	static FAandLayer FAwithMaxDeltaKv;
};


#endif // FA_BOX_H
