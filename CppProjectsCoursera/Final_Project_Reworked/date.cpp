#include "date.h"

#include <iomanip>
#include <tuple>

using namespace std;

bool operator==(const Date& lhs, const Date& rhs)
{
    if ( tie(lhs.year,lhs.month,lhs.day) == tie(rhs.year,rhs.month,rhs.day) )
        return true;
    else
        return false;
}

bool operator<(const Date& lhs, const Date& rhs)
{
    if ( tie(lhs.year,lhs.month,lhs.day) < tie(rhs.year,rhs.month,rhs.day) )
        return true;
    else
        return false;
}

bool operator!=(const Date& lhs, const Date& rhs) { return !(lhs == rhs);}
bool operator<=(const Date& lhs, const Date& rhs) { return (lhs < rhs || lhs == rhs);}
bool operator>(const Date& lhs, const Date& rhs) { return !(lhs <= rhs);}
bool operator>=(const Date& lhs, const Date& rhs) { return !(lhs < rhs);}

ostream& operator<<(ostream& out, const Date& date)
{
    out << setw(4) << setfill('0') << date.GetYear() << '-'
        << setw(2) << setfill('0') << date.GetMonth() << '-'
        << setw(2) << setfill('0') << date.GetDay();
    return out;
}

void EnsureNextSymbol(stringstream& in)
{
    if (in.peek() != '-') {
        throw std::invalid_argument("No '-' in date declaration");
    }
    in.ignore(1);
}

istream& operator>>(istream& in, Date& date)
{
    string input;
    in >> input;
    stringstream InputStream(input);
    try {
        InputStream >> date.year;
        EnsureNextSymbol(InputStream);
        InputStream >> date.month;
        EnsureNextSymbol(InputStream);
        if(!(InputStream >> date.day))throw exception();
        if (!InputStream.eof()) throw exception();
        if (date.month < 1 || date.month > 12) throw invalid_month("");
        if (date.day < 1 || date.day > 31) throw invalid_day("");
    } catch (const invalid_month&) {
        throw invalid_argument("Month value is invalid: "+to_string(date.month));
    } catch (const invalid_day&) {
        throw invalid_argument("Day value is invalid: "+to_string(date.day));
    } catch (const exception&) {
        throw invalid_argument("Wrong date format: "+input);
    }
    return in;
}

Date ParseDate(std::istream& in)
{
    Date ParsedDate;
    in >> ParsedDate;
    return ParsedDate;
}
