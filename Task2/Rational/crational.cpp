#include "crational.h"

cRational::cRational(int newNumer, int newDenom)
{
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
    this->numer = rRational.numer;
    this->denom = rRational.denom;
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

void cRational::SetNumer(int newNumer)
{
    this->numer = newNumer;
    this->RationalReduction();
}

void cRational::SetDenom(int newDenom)
{
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
    this->numer = newNumer;
    if (newDenom < 0)
    {
        this->numer *= -1;
        newDenom *= -1;
    }
    this->denom = newDenom;
    this->RationalReduction();
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
        std::cerr << "division by zero" << endl;
    }
    return (*this);
}

cRational::operator double()
{
    return static_cast<double>(this->numer)/static_cast<double>(this->denom);
}

// bolshe
template<typename T1, typename T2>
bool operator>(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) > rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 > static_cast<double>(rValue2));
    }
    return (rValue1 > rValue2);
}

template<> bool operator>(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) > static_cast<double>(rValue2));
}


// menshe
template<typename T1, typename T2>
bool operator<(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) < rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 < static_cast<double>(rValue2));
    }
    return (rValue1 < rValue2);
}

template<> bool operator<(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) < static_cast<double>(rValue2));
}

// bolshe =
template<typename T1, typename T2>
bool operator>=(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) >= rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 >= static_cast<double>(rValue2));
    }
    return (rValue1 >= rValue2);
}

template<> bool operator>=(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) >= static_cast<double>(rValue2));
}

// menshe =
template<typename T1, typename T2>
bool operator<=(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) <= rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 <= static_cast<double>(rValue2));
    }
    return (rValue1 <= rValue2);
}

template<> bool operator<=(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) <= static_cast<double>(rValue2));
}

// ==
template<typename T1, typename T2>
bool operator==(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) == rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 == static_cast<double>(rValue2));
    }
    return (rValue1 == rValue2);
}

template<> bool operator==(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) == static_cast<double>(rValue2));
}

// !=
template<typename T1, typename T2>
bool operator!=(T1 &rValue1, T2 &rValue2)
{
    if (is_same<T1,cRational>::value)
    {
        return (static_cast<double>(rValue1) != rValue2);
    }
    if (is_same<T2,cRational>::value)
    {
        return (rValue1 != static_cast<double>(rValue2));
    }
    return (rValue1 != rValue2);
}

template<> bool operator!=(cRational &rValue1, cRational &rValue2)
{
    return (static_cast<double>(rValue1) != static_cast<double>(rValue2));
}

// end comparison

cRational& operator+(const cRational & rRational, int value)
{
    cRational *pRational = new cRational;
    pRational->SetNumer(rRational.GetNumer() + (rRational.GetDenom() * value));
    pRational->SetDenom(rRational.GetDenom());
    return *pRational;
}

cRational& operator+(int value ,const cRational & rRational)
{
    cRational *pRational = new cRational;
    pRational->SetNumer(rRational.GetNumer() + (rRational.GetDenom() * value));
    pRational->SetDenom(rRational.GetDenom());
    return *pRational;
}

cRational& operator+(const cRational & rRational1 ,const cRational & rRational2)
{
    cRational *pRational = new cRational;
    pRational->SetNumer(rRational1.GetNumer()*rRational2.GetDenom() + rRational2.GetNumer()*rRational1.GetDenom());
    pRational->SetDenom(rRational1.GetDenom()*rRational2.GetDenom());
    return *pRational;
}
