#ifndef CELEMENT_H
#define CELEMENT_H


class cElement
{
private:
    long Coef;               // coefficient before poly term
    unsigned long Degree;    // poly term degree
public:
    // output variables
    long GetCoef(void) const {return Coef;}
    unsigned long GetDegree(void) const {return Degree;}
    // input variables
    void SetCoef(long newCoef) {Coef = newCoef;}
    void SetDegree(unsigned long newDegree) {Degree = newDegree;}
    // finding derivation for the element
    void Derivation(void);
    // default constructor
    cElement() : Coef(0), Degree(0) {}
    // full constructor
    cElement(long newCoef, unsigned long newDegree) : Coef(newCoef), Degree(newDegree) {}
    // destructor
    ~cElement() {}
};

#endif // CELEMENT_H
