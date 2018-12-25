#ifndef FA_BOX_H
#define FA_BOX_H

#include <map>
#include <QLabel>
#include <QPainterPath>

// ������������ ����������, ������������ � ���������
namespace  Parameters {
	enum ParametersEnum {
		Kq = 0,
		Burn = 1,
		Kv = 2
	};
}

// ������������ ������� ������ ������
namespace View {
	enum ViewEnum {
		DeltaView = 0,
		TwoStatesView = 1
	};
}

// ���, ���������������� �� ����
class cFA_Box{
private : 
	// ����
	// 
	QPainterPath painterPath;
	// ������� � ������� ���
	QLabel FA_Number;
	// ����� � �������� ��������
	QLabel Delta;
	// ����� � ������ ����������
	QLabel State1;
	// ����� �� ������ ����������
	QLabel State2;
	// ������ � ������ ��� �� ���������
	int _width;
	int _height;
	// ���������� ������ ���
	QPoint center;
	// ���� ���
	QColor Color;
	// �������� ���
	QWidget * Parent;
	// �������������� �� ���
	bool Visible;
	// �������� �� ��� - ��� � ��������
	const bool titleFA;
	
	// �������� �� ����������� ������
	static bool Colorful;
	// �������� ��������
	static Parameters::ParametersEnum activeMode;
	// �������� ����� ���������
	static View::ViewEnum viewMode;
	
	// ��������������� ��������� - ������������ � ����������� ��������
	struct Lim {
		double maxValue;
		double minValue;
	};
	// ��������������� ��������� - �������� ������ ��������� � ��������
	struct LimColors {
		QColor maxColor;
		QColor minColor;
	};

	// ���������� �������� ���������� - ����� �� ���������� ���� �� ��������
	static std::map<Parameters::ParametersEnum, Lim> Limiters;
	// ����� ��������
	static std::map<Parameters::ParametersEnum, LimColors> LimitColors;
	// ����� �������� �� ���������
	static std::map<Parameters::ParametersEnum, LimColors> defaultLimitColors;

	// ������
	// ���������� ������� �� ���������
	static void SetDefaultLimiters();
	// ���������� ����� �������� �� ���������
	static void SetDefaultLimitColors();
	// ���������� ���� ���
	void SetColor(QColor newColor) {Color = newColor;}
	// ����������� ���
	void recalculate(void);
	// ����������� ��� � ������������ �� ���������
	void recolor(const double &newValue);
	// ���������� ��������� ������ � ���
	void SetLabelParameters(void);
	// ���������� ��������� ������ � ���, ���� ��� - ��� � ��������
	void SetTitleLabelParameters(void);
public :
	// ������������
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
			QWidget *parent = 0) : 
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
					SetDelta(newDeltaText);
				}
	cFA_Box(int x_cent, int y_cent, 
			int newWidth, int newHeight,
			int newFA_Number, double newDelta,
			QWidget *parent = 0) : 
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
						FA_Number.setText(QString::fromWCharArray(L"�"));
						Delta.setText(QString::fromWCharArray(L"��������"));
						State1.setText(QString::fromWCharArray(L"�������� 1"));
						State2.setText(QString::fromWCharArray(L"�������� 2"));
						SetColor(Qt::white);
					}
				}
	cFA_Box(const cFA_Box &rFA);
	cFA_Box& operator=(const cFA_Box &rFA);
	// ����������
	~cFA_Box() {}
	// ��������� ������
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
	// ��������� ����� ������
	void SetCenterX(int newCenterX) {center.setX(newCenterX);}
	void SetCenterY(int newCenterY) {center.setY(newCenterY);}
	void SetWidth(int newWidth) {_width = newWidth;}
	void SetHeight(int newHeight) {_height = newHeight;}
	void SetFA_Number(const int &newFA_Number);
	// ��������� �������� ��������� �� ������, ���� ��� �� �������. ������������� ��� � ����� (�.�. ��������� �������� ���, �.�. � �������� NaN)
	void SetDelta(const QString &newDeltaText) { if (!titleFA) { Delta.setText(newDeltaText); this->SetColor(Qt::gray); } }
	void SetDelta(const double &newDelta);
	// ��������� ��������� 1 �� ������, ���� ��� �� �������
	void SetState1(const QString &newState1Text) { if (!titleFA) State1.setText(newState1Text);}
	void SetState1(const double &newState1);
	// ��������� ��������� 2 �� ������, ���� ��� �� �������
	void SetState2(const QString &newState2Text) { if (!titleFA) State2.setText(newState2Text);}
	void SetState2(const double &newState2);
	// ��������� ��������� ��� ���� Label-�� � ������ �������� ����������
	void SetParent(QWidget * parent) {Parent = parent; Delta.setParent(Parent); FA_Number.setParent(Parent); State1.setParent(Parent); State2.setParent(Parent);}
	void SetVisible(bool visible);
	static void SetColorful(bool colorful);
	static void SetMinColor(const Parameters::ParametersEnum& parameter, const QColor& NewMinColor);
	static void SetMaxColor(const Parameters::ParametersEnum& parameter, const QColor& NewMaxColor);
	static void SetLimiters(const Parameters::ParametersEnum& parameter, const double &max);
	static void SetActiveMode(const Parameters::ParametersEnum& newActiveMode) {activeMode = newActiveMode;}
	static void SetViewMode(const View::ViewEnum& newViewMode) {viewMode = newViewMode;}
	static void SetLimitColorsFromDefaults();
	// �������� ��� �� �������
	void ChangeView(void);
	// ���������� ���������
	void SetGeometry(int newCenterX, int newCenterY, int newWidth, int newHeight);
	// ����������� ���
	void move(int newCenterX, int newCenterY);
	// �������� ������ ���
	void resize(int newWidth, int newHeight);

	// ��������� ����
	// ��������������� ��������� ��������� - ��� � ����
	struct FAandLayer {
		FAandLayer() : FA(1), Layer(1) {}
		int FA;
		int Layer;
	};
	// ��� � ���� � ������������ �������� Kv
	static FAandLayer FAwithMaxDeltaKv;
};


#endif // FA_BOX_H