#include "phone_number.h"

#include <stdexcept>

PhoneNumber::PhoneNumber(const string &international_number)
{
    if (international_number.find("+") != 0) throw invalid_argument("");
    if (international_number.find("-") == string::npos || international_number.find("-") < 2)
    {
        throw invalid_argument("");
    }
    string::size_type first_minus_pos = international_number.find('-');
    country_code_ = international_number.substr(1,first_minus_pos - 1);
    if (international_number.find('-',first_minus_pos+1) == string::npos ||
        international_number.find('-',first_minus_pos+1) < first_minus_pos + 2 ||
        international_number.find('-',first_minus_pos+1) == international_number.size() - 1)
    {
        throw invalid_argument("");
    }
    string::size_type second_minus_pos = international_number.find('-',first_minus_pos+1);
    city_code_ = international_number.substr(first_minus_pos+1,second_minus_pos - first_minus_pos - 1);
    local_number_ = international_number.substr(second_minus_pos+1);
}

string PhoneNumber::GetCountryCode() const
{
    return country_code_;
}

string PhoneNumber::GetCityCode() const
{
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const
{
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const
{
    return "+"+country_code_+"-"+city_code_+"-"+local_number_;
}
