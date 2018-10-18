#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

using namespace std;
// Реализуйте функции и методы классов и при необходимости добавьте свои

class invalid_month : public invalid_argument {
public:
    invalid_month(const char * pchar) : invalid_argument(pchar) {}
};

class invalid_day : public invalid_argument {
public:
    invalid_day(const char * pchar) : invalid_argument(pchar) {}
};

class Date {
public:
  Date() : year(0), month(0), day(0) {}
  explicit Date(int day, int month, int year) : year(year), month(month), day(day) {}
  int GetYear() const {return year;}
  int GetMonth() const {return month;}
  int GetDay() const {return day;}
  friend istream& operator>>(istream& in, Date& date);
private:
  int year;
  int month;
  int day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    if(lhs.GetYear()<rhs.GetYear())return true;
    if(lhs.GetYear()>rhs.GetYear())return false;
    if(lhs.GetMonth()<rhs.GetMonth())return true;
    if(lhs.GetMonth()>rhs.GetMonth())return false;
    if(lhs.GetDay()<rhs.GetDay())return true;
    return false;
}

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

class Database {
public:
  void AddEvent(const Date& date, const string& event);
  bool DeleteEvent(const Date& date, const string& event);
  int  DeleteDate(const Date& date);

  set<string> Find(const Date& date) const;
  
  void Print() const;

private:
  map<Date,set<string>> EventsMap;
};

void Database::AddEvent(const Date& date, const string& event)
{
    EventsMap[date].insert(event);
}

bool Database::DeleteEvent(const Date& date, const string& event)
{
    if(EventsMap.count(date))
    {
        if(EventsMap.at(date).count(event))
        {
            EventsMap.at(date).erase(event);
            if(EventsMap.at(date).empty())EventsMap.erase(date);
            return true;
        }
    }
    return false;
}

int Database::DeleteDate(const Date& date)
{
    if(EventsMap.count(date))
    {
        int DeletedEventsCount = static_cast<int>(EventsMap.at(date).size());
        EventsMap.erase(date);
        return DeletedEventsCount;
    }
    return 0;
}

set<string> Database::Find(const Date& date) const
{
    if(EventsMap.count(date)) return EventsMap.at(date);
    return set<string>();
}

void Database::Print() const
{
    for(auto mit = EventsMap.begin(); mit != EventsMap.end(); mit++)
    {
        for(auto sit = mit->second.begin(); sit != mit->second.end(); sit++)
        {
            cout << mit->first << ' ' << *sit << endl;
        }
    }
}


// MAIN
int main() {
  Database db;
    
  string command;
  while (getline(cin, command)) {
      // Считайте команды с потока ввода и обработайте каждую
      if (!command.length())continue;
      stringstream CommandStream(command);
      string CommandName;
      CommandStream >> CommandName;
      if(CommandName != "Add" &&
         CommandName != "Del" &&
         CommandName != "Find" &&
         CommandName != "Print") {cout << "Unknown command: " << CommandName << endl; return 0;}
      if (CommandName == "Add")
      {
          Date date;
          string event;
          try {
            CommandStream >> date >> event;
          } catch (const invalid_argument& ex) {
            cout << ex.what() << endl;
            return 0;
          }
          db.AddEvent(date,event);
      }
      if (CommandName == "Del")
      {
          Date date;
          string event;
          try {
            CommandStream >> date >> event;
          } catch (const invalid_argument& ex) {
            cout << ex.what() << endl;
            return 0;
          }
          // if there are no errors in date
          if (event.length() == 0)
          {
              cout << "Deleted " << db.DeleteDate(date)<< " events" << endl;
          } else {
              bool Deleted = db.DeleteEvent(date,event);
              if (Deleted)
              {
                  cout << "Deleted successfully" << endl;
              } else {
                  cout << "Event not found" << endl;
              }
          }
      }
      if (CommandName == "Find")
      {
          Date date;
          try {
            CommandStream >> date;
          } catch (const invalid_argument& ex) {
            cout << ex.what() << endl;
            return 0;
          }
          set<string> Finded = db.Find(date);
          for (auto it = Finded.begin(); it != Finded.end(); it++)
          {
              cout << *it << endl;
          }
      }
      if (CommandName == "Print")
      {
          db.Print();
      }
  }

  return 0;
}
