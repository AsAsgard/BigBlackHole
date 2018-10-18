#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Rational
{
private:
    int numer;
    int denom;
public:
    // get variables
    int Numerator() const {return numer;}
    int Denominator() const {return denom;}
    // set variables
    void SetNumer(int newNumer);
    void SetDenom(int newDenom);
    void SetRational(int newNumer, int newDenom);
    // cast numer and denom
    void RationalReduction();
    // raises Rational object to power minus one
    Rational& powMinusOne();
    // operators
    Rational& operator+=(int intValue);
    Rational& operator-=(int intValue);
    Rational& operator*=(int intValue);
    Rational& operator/=(int intValue);
    Rational& operator+=(const Rational &rRational);
    Rational& operator-=(const Rational &rRational);
    Rational& operator*=(const Rational &rRational);
    Rational& operator/=(const Rational &rRational);
    Rational  operator-() { Rational RationalValue( - this->numer, this->denom); return (RationalValue);}
    Rational& operator+() { return (*this);}
    Rational& operator=(int intValue) { this->numer = intValue; this->denom = 1; return (*this);}
    Rational& operator=(const Rational &rRational) { this->numer = rRational.numer; this->denom = rRational.denom; return (*this);}
    Rational& operator=(Rational &&rrRational) noexcept;
    // Rational to double and to int
    operator double();
    explicit operator int();
    // comparison
    bool operator>(const Rational &rRational) const {return (this->Numerator()*rRational.Denominator() > rRational.Numerator()*this->Denominator());}
    bool operator<(const Rational &rRational) const {return (rRational > *this);}
    bool operator>=(const Rational &rRational) const {return !(*this < rRational);}
    bool operator<=(const Rational &rRational) const {return !(*this > rRational);}
    bool operator==(const Rational &rRational) const {return (this->Numerator() == rRational.Numerator() && this->Denominator() == rRational.Denominator());}
    bool operator!=(const Rational &rRational) const {return !(*this == rRational);}
    // swap
    void Swap(Rational &rRational) noexcept;
    // constructor
    Rational() noexcept : numer(0), denom(1) {}
    explicit Rational(int newNumer) : numer(newNumer), denom(1) {}
    Rational(int newNumer, int newDenom);
    Rational(const Rational &rRational);
    Rational(Rational &&rrRational) noexcept : Rational() { if (this != &rrRational) this->Swap(rrRational);}
    template<typename T1, typename T2>
    Rational(T1, T2) = delete;
};

// operators
Rational operator+(Rational RationalValue, int value);
Rational operator+(Rational RationalValue, const Rational & rRational);
Rational operator+(int value, Rational RationalValue);

Rational operator-(Rational RationalValue, int value);
Rational operator-(Rational RationalValue, const Rational & rRational);
Rational operator-(int value, Rational RationalValue);

Rational operator*(Rational RationalValue, int value);
Rational operator*(Rational RationalValue, const Rational & rRational);
Rational operator*(int value, Rational RationalValue);

Rational operator/(Rational RationalValue, int value);
Rational operator/(Rational RationalValue, const Rational & rRational);
Rational operator/(int value, Rational RationalValue);

// output and input
std::ostream& operator<<(std::ostream& out, const Rational &rRational);
std::istream& operator>>(std::istream& in, Rational &rRational);

//REALIZATION
Rational::Rational(int newNumer, int newDenom)
{
    if (newDenom == 0) throw std::invalid_argument("Division by zero!");
    this->numer = newNumer;
    if (newDenom < 0)
    {
        this->numer *= -1;
        newDenom *= -1;
    }
    this->denom = newDenom;
    this->RationalReduction();
}

Rational::Rational(const Rational &rRational)
{
    if (this != &rRational)
    {
        this->numer = rRational.numer;
        this->denom = rRational.denom;
    }
}

void Rational::RationalReduction()
{
    int a = abs(this->numer);
    int b = abs(this->denom);
    while (b!=0)
    {
        int c = a % b;
        a = b;
        b = c;
    }
    this->numer /= a;
    this->denom /= a;
}

void Rational::Swap(Rational &rRational) noexcept
{
    if (this != &rRational)
    {
        this->numer = this->numer + rRational.numer - (rRational.numer = this->numer);
        this->denom = this->denom + rRational.denom - (rRational.denom = this->denom);
    }
}

void Rational::SetNumer(int newNumer)
{
    this->numer = newNumer;
    this->RationalReduction();
}

void Rational::SetDenom(int newDenom)
{
    if (newDenom == 0) throw std::invalid_argument("Division by zero!");
    if (newDenom < 0)
    {
        this->numer *= -1;
        newDenom *= -1;
    }
    this->denom = newDenom;
    this->RationalReduction();
}

void Rational::SetRational(int newNumer, int newDenom)
{
    if (newDenom == 0) throw std::invalid_argument("Division by zero!");
    this->numer = newNumer;
    if (newDenom < 0)
    {
        this->numer *= -1;
        newDenom *= -1;
    }
    this->denom = newDenom;
    this->RationalReduction();
}

Rational& Rational::powMinusOne()
{
    if (this->numer != 0)
    {
        this->numer = this->numer + this->denom - (this->denom = this->numer);
    }
    else
    {
        throw std::domain_error("Division by zero!");
    }
    return (*this);
}

Rational& Rational::operator+=(int intValue)
{
    this->numer += intValue*this->denom;
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator-=(int intValue)
{
    this->numer -= intValue*this->denom;
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator*=(int intValue)
{
    this->numer *= intValue;
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator/=(int intValue)
{
    if (intValue != 0)
    {
        this->denom *= abs(intValue);
        this->numer *= intValue/abs(intValue);
        this->RationalReduction();
    }
    else
    {
        throw std::domain_error("Division by zero!");
    }
    return (*this);
}

Rational& Rational::operator+=(const Rational &rRational)
{
    this->numer = this->numer * rRational.Denominator() + rRational.Numerator() * this->denom;
    this->denom = this->denom * rRational.Denominator();
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator-=(const Rational &rRational)
{
    this->numer = this->numer * rRational.Denominator() - rRational.Numerator() * this->denom;
    this->denom = this->denom * rRational.Denominator();
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator*=(const Rational &rRational)
{
    this->numer *= rRational.Numerator();
    this->denom *= rRational.Denominator();
    this->RationalReduction();
    return (*this);
}

Rational& Rational::operator/=(const Rational &rRational)
{
    if (rRational.Numerator() != 0)
    {
        this->denom *= abs(rRational.Numerator());
        this->numer *= rRational.Denominator()*(rRational.Numerator()/abs(rRational.Numerator()));
        this->RationalReduction();
    }
    else
    {
        throw std::domain_error("Division by zero!");
    }
    return (*this);
}

Rational& Rational::operator=(Rational &&rrRational) noexcept
{
    Rational tmp(std::move(rrRational));
    this->Swap(tmp);
    return *this;
}

/*
 * Rational to double
 * */
Rational::operator double()
{
    return static_cast<double>(this->numer)/static_cast<double>(this->denom);
}

/*
 * Rational to int
 * */
Rational::operator int()
{
    if ( (static_cast<double>(*this) - static_cast<int>(static_cast<double>(*this))) >= 0.5 ) return static_cast<int>(static_cast<double>(*this) + 1);
    if ( (static_cast<double>(*this) - static_cast<int>(static_cast<double>(*this))) <= -0.5 ) return static_cast<int>(static_cast<double>(*this) - 1);
    return static_cast<int>(static_cast<double>(*this));
}


/*
 * operators + - and *
 * */
Rational operator+(Rational RationalValue, int value) {RationalValue += value; return RationalValue;}
Rational operator+(Rational RationalValue, const Rational & rRational) {RationalValue += rRational; return RationalValue;}
Rational operator+(int value, Rational RationalValue) {RationalValue += value; return RationalValue;}

Rational operator-(Rational RationalValue, int value) {RationalValue -= value; return RationalValue;}
Rational operator-(Rational RationalValue, const Rational & rRational) {RationalValue -= rRational; return RationalValue;}
Rational operator-(int value, Rational RationalValue) {RationalValue += value; return -RationalValue;}

Rational operator*(Rational RationalValue, int value) {RationalValue *= value; return RationalValue;}
Rational operator*(Rational RationalValue, const Rational & rRational) {RationalValue *= rRational; return RationalValue;}
Rational operator*(int value, Rational RationalValue) {RationalValue *= value; return RationalValue;}

/*
 * operator /
 * */

Rational operator/(Rational RationalValue, int value)
{
    RationalValue /= value;
    return RationalValue;
}

Rational operator/(Rational RationalValue, const Rational & rRational)
{
    RationalValue /= rRational;
    return RationalValue;
}

Rational operator/(int value, Rational RationalValue)
{
    RationalValue /= value;
    return RationalValue.powMinusOne();
}

/*
 * Output
 * */
std::ostream& operator<<(std::ostream& out, const Rational &rRational)
{
    out << rRational.Numerator() << "/" << rRational.Denominator();
    return out;
}

/*
 * Input
 * */
std::istream& operator>>(std::istream& in, Rational &rRational)
{
    string str = "";
    try {
        in >> str;
        if (str.length()==0)return in;
        if (str.find("/") == string::npos) throw std::invalid_argument("Bad rational value.");
        rRational.SetRational( stoi( str.substr(0,str.find("/")) ), stoi( str.substr(str.find("/")+1) ) );
        return in;
    } catch (const exception &) {
        in.clear();
        in.ignore();
        throw std::invalid_argument("Bad rational value.");
    }
}

int main() {
    {
        const Rational r(3, 10);
        if (r.Numerator() != 3 || r.Denominator() != 10) {
            cout << "Rational(3, 10) != 3/10" << endl;
            return 1;
        }
    }

    {
        const Rational r(8, 12);
        if (r.Numerator() != 2 || r.Denominator() != 3) {
            cout << "Rational(8, 12) != 2/3" << endl;
            return 2;
        }
    }

    {
        const Rational r(-4, 6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(-4, 6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(4, -6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(4, -6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const Rational r(0, 15);
        if (r.Numerator() != 0 || r.Denominator() != 1) {
            cout << "Rational(0, 15) != 0/1" << endl;
            return 4;
        }
    }

    {
        const Rational defaultConstructed;
        if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
            cout << "Rational() != 0/1" << endl;
            return 5;
        }
    }

    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }

    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    {
        const set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}
