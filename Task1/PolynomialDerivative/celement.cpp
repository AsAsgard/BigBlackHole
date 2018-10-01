#include "celement.h"

/*
 *  Getting derivative from one element of poly
 * */

void cElement::Derivation(void)
{
    if (Degree > 0)
    {
        Coef *= Degree;     // d(c*x^a)/dx = (c*a)*x^(a-1)
        Degree--;
    } else {
        Coef = 0;   // if degree == 0, then derivative = 0 too.
    }
}
