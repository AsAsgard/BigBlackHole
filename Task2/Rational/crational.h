#ifndef CRATIONAL_H
#define CRATIONAL_H


class cRational
{
private:
    int numer;
    unsigned int denom;
public:
    // get variables
    int GetNumer() const {return numer;}
    unsigned int GetDenom() const {return denom;}
    // set variables
    void SetNumer(int newNumer);
    void SetDenom(unsigned int newDenom);
    // constructor
    cRational();
};

#endif // CRATIONAL_H
