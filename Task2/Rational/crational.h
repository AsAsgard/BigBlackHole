#ifndef CRATIONAL_H
#define CRATIONAL_H

#include <iostream>
#include <stdexcept>

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
    // raises cRational object to power minus one
    cRational& powMinusOne();
    // operators
    cRational& operator+=(int intValue);
    cRational& operator-=(int intValue);
    cRational& operator*=(int intValue);
    cRational& operator/=(int intValue);
    cRational& operator+=(const cRational &rRational);
    cRational& operator-=(const cRational &rRational);
    cRational& operator*=(const cRational &rRational);
    cRational& operator/=(const cRational &rRational);
    cRational& operator-() { cRational *pRational = new cRational( - this->numer, this->denom); return (*pRational);}
    cRational& operator+() { return (*this);}
    cRational& operator=(int intValue) { this->numer = intValue; this->denom = 1; return (*this);}
    cRational& operator=(const cRational &rRational) { this->numer = rRational.numer; this->denom = rRational.denom; return (*this);}
    // cRational to double
    operator double();
    // comparison
    bool operator>(const cRational &rRational) const {return (this->GetNumer()*rRational.GetDenom() > rRational.GetNumer()*this->GetDenom());}
    bool operator<(const cRational &rRational) const {return (rRational > *this);}
    bool operator>=(const cRational &rRational) const {return !(*this < rRational);}
    bool operator<=(const cRational &rRational) const {return !(*this > rRational);}
    bool operator==(const cRational &rRational) const {return (this->GetNumer() == rRational.GetNumer() && this->GetDenom() == rRational.GetDenom());}
    bool operator!=(const cRational &rRational) const {return !(*this == rRational);}
    // constructor
    cRational() : numer(0), denom(1) {}
    cRational(int newNumer, int newDenom);
    cRational(const cRational &rRational);
    cRational(cRational &&rrRational);
    template<typename T1, typename T2>
    cRational(T1, T2) = delete;
};

// operators
cRational operator+(cRational RationalValue, int value);
cRational operator+(cRational RationalValue, const cRational & rRational);
cRational operator+(int value, cRational RationalValue);

cRational operator-(cRational RationalValue, int value);
cRational operator-(cRational RationalValue, const cRational & rRational);
cRational operator-(int value, cRational RationalValue);

cRational operator*(cRational RationalValue, int value);
cRational operator*(cRational RationalValue, const cRational & rRational);
cRational operator*(int value, cRational RationalValue);

cRational operator/(cRational RationalValue, int value);
cRational operator/(cRational RationalValue, const cRational & rRational);
cRational operator/(int value, cRational RationalValue);

// output
std::ostream& operator<<(std::ostream& out, const cRational &rRational);


#endif // CRATIONAL_H
