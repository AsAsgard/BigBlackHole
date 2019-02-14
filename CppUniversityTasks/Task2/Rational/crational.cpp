#include "crational.h"

cRational::cRational(int newNumer, int newDenom)
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

cRational::cRational(const cRational &rRational)
{
    if (this != &rRational)
    {
        this->numer = rRational.numer;
        this->denom = rRational.denom;
    }
}

void cRational::RationalReduction()
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

void cRational::Swap(cRational &rRational) noexcept
{
    if (this != &rRational)
    {
        this->numer = this->numer + rRational.numer - (rRational.numer = this->numer);
        this->denom = this->denom + rRational.denom - (rRational.denom = this->denom);
    }
}

void cRational::SetNumer(int newNumer)
{
    this->numer = newNumer;
    this->RationalReduction();
}

void cRational::SetDenom(int newDenom)
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

void cRational::SetRational(int newNumer, int newDenom)
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

cRational& cRational::powMinusOne()
{
    if (this->numer != 0)
    {
        this->numer = this->numer + this->denom - (this->denom = this->numer);
    }
    else
    {
        throw std::invalid_argument("Division by zero!");
    }
    return (*this);
}

cRational& cRational::operator+=(int intValue)
{
    this->numer += intValue*this->denom;
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator-=(int intValue)
{
    this->numer -= intValue*this->denom;
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator*=(int intValue)
{
    this->numer *= intValue;
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator/=(int intValue)
{
    if (intValue != 0)
    {
        this->denom *= abs(intValue);
        this->numer *= intValue/abs(intValue);
        this->RationalReduction();
    }
    else
    {
        throw std::invalid_argument("Division by zero!");
    }
    return (*this);
}

cRational& cRational::operator+=(const cRational &rRational)
{
    this->numer = this->numer * rRational.GetDenom() + rRational.GetNumer() * this->denom;
    this->denom = this->denom * rRational.GetDenom();
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator-=(const cRational &rRational)
{
    this->numer = this->numer * rRational.GetDenom() - rRational.GetNumer() * this->denom;
    this->denom = this->denom * rRational.GetDenom();
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator*=(const cRational &rRational)
{
    this->numer *= rRational.GetNumer();
    this->denom *= rRational.GetDenom();
    this->RationalReduction();
    return (*this);
}

cRational& cRational::operator/=(const cRational &rRational)
{
    if (rRational.GetNumer() != 0)
    {
        this->denom *= abs(rRational.GetNumer());
        this->numer *= rRational.GetDenom()*(rRational.GetNumer()/abs(rRational.GetNumer()));
        this->RationalReduction();
    }
    else
    {
        throw std::invalid_argument("Division by zero!");
    }
    return (*this);
}

cRational& cRational::operator=(cRational &&rrRational) noexcept
{
    cRational tmp(std::move(rrRational));
    this->Swap(tmp);
    return *this;
}

/*
 * cRational to double
 * */
cRational::operator double()
{
    return static_cast<double>(this->numer)/static_cast<double>(this->denom);
}

/*
 * cRational to int
 * */
cRational::operator int()
{
    if ( (static_cast<double>(*this) - static_cast<int>(static_cast<double>(*this))) >= 0.5 ) return static_cast<int>(static_cast<double>(*this) + 1);
    if ( (static_cast<double>(*this) - static_cast<int>(static_cast<double>(*this))) <= -0.5 ) return static_cast<int>(static_cast<double>(*this) - 1);
    return static_cast<int>(static_cast<double>(*this));
}


/*
 * operators + - and *
 * */
cRational operator+(cRational RationalValue, int value) {RationalValue += value; return RationalValue;}
cRational operator+(cRational RationalValue, const cRational & rRational) {RationalValue += rRational; return RationalValue;}
cRational operator+(int value, cRational RationalValue) {RationalValue += value; return RationalValue;}

cRational operator-(cRational RationalValue, int value) {RationalValue -= value; return RationalValue;}
cRational operator-(cRational RationalValue, const cRational & rRational) {RationalValue -= rRational; return RationalValue;}
cRational operator-(int value, cRational RationalValue) {RationalValue += value; return -RationalValue;}

cRational operator*(cRational RationalValue, int value) {RationalValue *= value; return RationalValue;}
cRational operator*(cRational RationalValue, const cRational & rRational) {RationalValue *= rRational; return RationalValue;}
cRational operator*(int value, cRational RationalValue) {RationalValue *= value; return RationalValue;}

/*
 * operator /
 * */

cRational operator/(cRational RationalValue, int value)
{
    RationalValue /= value;
    return RationalValue;
}

cRational operator/(cRational RationalValue, const cRational & rRational)
{
    RationalValue /= rRational;
    return RationalValue;
}

cRational operator/(int value, cRational RationalValue)
{
    RationalValue /= value;
    return RationalValue.powMinusOne();
}

/*
 * Output
 * */
std::ostream& operator<<(std::ostream& out, const cRational &rRational)
{
    out << rRational.GetNumer() << "/" << rRational.GetDenom();
    return out;
}

/*
 * Input
 * */
std::istream& operator>>(std::istream& in, cRational &rRational)
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
