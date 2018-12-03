#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

class Figure
{
public:
    virtual string Name() = 0;
    virtual long double Perimeter() = 0;
    virtual long double Area() = 0;
    virtual ~Figure() {}
};

class Triangle : public Figure
{
public:
    Triangle(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
    string Name() override { return "TRIANGLE";}
    long double Perimeter() override { return a+b+c;}
    long double Area() override { long double pp = (a+b+c) / 2; return sqrt(pp * (pp - a) * (pp - b) * (pp - c));}
    ~Triangle() override {}
private:
    long double a, b, c;
};

class Rect : public Figure
{
public:
    Rect(int width_, int height_) : width(width_), height(height_) {}
    string Name() override { return "RECT";}
    long double Perimeter() override { return 2 * (width + height);}
    long double Area() override { return width * height;}
    ~Rect() override {}
private:
    long double width, height;
};

class Circle : public Figure
{
public:
    Circle(int r_) : r(r_) {}
    string Name() override { return "CIRCLE";}
    long double Perimeter() override { return 2 * 3.14 * r;}
    long double Area() override { return 3.14 * r * r;}
    ~Circle() override {}
private:
    long double r;
};


shared_ptr<Figure> CreateFigure(istringstream& is)
{
    string figure_type("");
    is >> figure_type;
    if (figure_type == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    } else if (figure_type == "RECT") {
        int width, height;
        is >> width >> height;
        return make_shared<Rect>(width, height);
    } else if (figure_type == "CIRCLE") {
        int r;
        is >> r;
        return make_shared<Circle>(r);
    }
    throw invalid_argument("Bad input");
}


int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        cout << fixed << setprecision(3)
             << current_figure->Name() << " "
             << current_figure->Perimeter() << " "
             << current_figure->Area() << endl;
      }
    }
  }
  return 0;
}
