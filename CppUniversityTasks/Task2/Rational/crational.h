#ifndef CRATIONAL_H
#define CRATIONAL_H

#include <iostream>
#include <stdexcept>
#include <string>

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
    cRational  operator-() { cRational RationalValue( - this->numer, this->denom); return (RationalValue);}
    cRational& operator+() { return (*this);}
    cRational& operator=(int intValue) { this->numer = intValue; this->denom = 1; return (*this);}
    cRational& operator=(const cRational &rRational) { this->numer = rRational.numer; this->denom = rRational.denom; return (*this);}
    cRational& operator=(cRational &&rrRational) noexcept;
    // cRational to double and to int
    operator double();
    explicit operator int();
    // comparison
    bool operator>(const cRational &rRational) const {return (this->GetNumer()*rRational.GetDenom() > rRational.GetNumer()*this->GetDenom());}
    bool operator<(const cRational &rRational) const {return (rRational > *this);}
    bool operator>=(const cRational &rRational) const {return !(*this < rRational);}
    bool operator<=(const cRational &rRational) const {return !(*this > rRational);}
    bool operator==(const cRational &rRational) const {return (this->GetNumer() == rRational.GetNumer() && this->GetDenom() == rRational.GetDenom());}
    bool operator!=(const cRational &rRational) const {return !(*this == rRational);}
    // swap
    void Swap(cRational &rRational) noexcept;
    // constructor
    cRational() noexcept : numer(0), denom(1) {}
    explicit cRational(int newNumer) : numer(newNumer), denom(1) {}
    cRational(int newNumer, int newDenom);
    cRational(const cRational &rRational);
    cRational(cRational &&rrRational) noexcept : cRational() { if (this != &rrRational) this->Swap(rrRational);}
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

// output and input
std::ostream& operator<<(std::ostream& out, const cRational &rRational);
std::istream& operator>>(std::istream& in, cRational &rRational);

#endif // CRATIONAL_H
