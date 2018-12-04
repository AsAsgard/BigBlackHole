#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <sstream>

class invalid_month : public std::invalid_argument {
public:
    invalid_month(const char * pchar) : invalid_argument(pchar) {}
};

class invalid_day : public std::invalid_argument {
public:
    invalid_day(const char * pchar) : invalid_argument(pchar) {}
};

class Date {
public:
  Date(int year = 0, int month = 0, int day = 0) : year(year), month(month), day(day) {}
  int GetYear() const {return year;}
  int GetMonth() const {return month;}
  int GetDay() const {return day;}
  friend std::istream& operator>>(std::istream& in, Date& date);
  friend bool operator==(const Date& lhs, const Date& rhs);
  friend bool operator<(const Date& lhs, const Date& rhs);

private:
  int year;
  int month;
  int day;
};

bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
std::ostream& operator<<(std::ostream& out, const Date& date);
std::istream& operator>>(std::istream& in, Date& date);
Date ParseDate(std::istream& in);

#endif // DATE_H
