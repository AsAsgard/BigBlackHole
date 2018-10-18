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


// FUNCTION

Rational ResultOfOperation(const Rational &x, const Rational &y, const char &operation)
{
    if (operation=='+') return x+y;
    if (operation=='-') return x-y;
    if (operation=='*') return x*y;
    if (operation=='/') return x/y;
    return x;
}

// MAIN

int main()
{
    Rational x, y;
    char operation = ' ';
    try {
        cin >> x >> operation >> y;
        cout << ResultOfOperation(x,y,operation);
    } catch (const invalid_argument&) {
        cout << "Invalid argument";
    } catch (const domain_error&) {
        cout << "Division by zero";
    }
    return 0;
}
