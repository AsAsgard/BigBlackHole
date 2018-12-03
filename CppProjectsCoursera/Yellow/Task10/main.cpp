#include "phone_number.h"
#include <iostream>


int main()
{
    PhoneNumber ph("+3-3-3");
    cout << ph.GetCountryCode() << " | " << ph.GetCityCode() << " | " << ph.GetLocalNumber() << endl;
    cout << ph.GetInternationalNumber() << endl;
    return 0;
}
