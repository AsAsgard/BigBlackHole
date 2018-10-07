#ifndef CRATIONAL_H
#define CRATIONAL_H

#include <iostream>

using namespace std;

class cRational
{
private:
    int numer;
    int denom;
public:
    // get variables
    int GetNumer() const {return numer;}
    int GetDenom() const {return denom;}
    // set variables
    void SetNumer(int newNumer);
    void SetDenom(int newDenom);
    void SetRational(int newNumer, int newDenom);
    // cast numer and denom
    void RationalReduction();
    // operators
    cRational& operator+=(int intValue);
    cRational& operator-=(int intValue);
    cRational& operator*=(int intValue);
    cRational& operator/=(int intValue);
    cRational& operator!() { this->numer *= -1; return (*this);}
    // cRational to double
    operator double();
    // constructor
    cRational() : numer(0), denom(1) {}
    cRational(int newNumer, int newDenom);
    cRational(const cRational &rRational);
    template<typename T1, typename T2>
    cRational(T1, T2) = delete;
};

// comparison
template<typename T1, typename T2>
bool operator>(T1 &rValue1, T2 &rValue2);

template<typename T1, typename T2>
bool operator<(T1 &rValue1, T2 &rValue2);

template<typename T1, typename T2>
bool operator>=(T1 &rValue1, T2 &rValue2);

template<typename T1, typename T2>
bool operator<=(T1 &rValue1, T2 &rValue2);

template<typename T1, typename T2>
bool operator==(T1 &rValue1, T2 &rValue2);

template<typename T1, typename T2>
bool operator!=(T1 &rValue1, T2 &rValue2);

// operators
cRational& operator+(const cRational & rRational, int value);
cRational& operator+(const cRational & rRational1, const cRational & rRational2);
cRational& operator+(int value, const cRational & rRational);

cRational& operator-(const cRational & rRational, int value);
cRational& operator*(const cRational & rRational, int value);
cRational& operator/(const cRational & rRational, int value);


// output
std::ostream& operator<<(std::ostream& out, const cRational &rRational);


#endif // CRATIONAL_H
