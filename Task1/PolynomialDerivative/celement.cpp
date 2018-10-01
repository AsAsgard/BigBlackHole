#include "celement.h"

void cElement::Derivation(void)
{
    if (Degree > 0)
    {
        Coef *= Degree;
        Degree--;
    } else {
        Coef = 0;
    }
}
