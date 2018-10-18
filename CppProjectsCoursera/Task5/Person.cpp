#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

class Person {
    //PUBLIC
public:
  //constructor
  Person(const string & newfirstname, const string & newlastname, int birth) : birthyear(birth)
  {
      PersonsMap[birth].first = newfirstname;
      PersonsMap[birth].second = newlastname;
  }
  void ChangeFirstName(int year, const string& first_name) {
      // добавить факт изменения имени на first_name в год year
    if (year < birthyear) return;
    if(PersonsMap.count(year) == 0)PersonsMap[year].second = "";
    PersonsMap[year].first = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
      // добавить факт изменения имени на first_name в год year
    if (year < birthyear) return;
    if(PersonsMap.count(year) == 0)PersonsMap[year].first = "";
    PersonsMap[year].second = last_name;
  }
  string GetFullName(int year) const {
      // получить имя и фамилию по состоянию на конец года year
    if (year < birthyear) return "No person";
    if (year < PersonsMap.begin()->first || PersonsMap.empty()) return "Incognito";
    string firstname = "", lastname = "";
    for (auto it = PersonsMap.begin(); it != PersonsMap.end(); it++)
    {
        if (year < it->first) break;
        if(it->second.first != "")firstname = it->second.first;
        if(it->second.second != "")lastname = it->second.second;
    }
    if (firstname == "") return lastname+" with unknown first name";
    if (lastname == "") return firstname+" with unknown last name";
    return firstname + " " + lastname;
  }
  string GetFullNameWithHistory(int year) const {
      // получить все имена и фамилии по состоянию на конец года year
      if (year < birthyear) return "No person";
      if (year < PersonsMap.begin()->first || PersonsMap.empty()) return "Incognito";
      string firstname = "", lastname = "";
      vector<string> history_of_firstname, history_of_lastname;
      for (auto it = PersonsMap.begin(); it != PersonsMap.end(); it++)
      {
          if (year < it->first) break;
          if(it->second.first != "")
          {
              if(firstname != "" && firstname != it->second.first)history_of_firstname.emplace_back(firstname);
              firstname = it->second.first;
          }
          if(it->second.second != "")
          {
              if(lastname != "" && lastname != it->second.second)history_of_lastname.emplace_back(lastname);
              lastname = it->second.second;
          }
      }

      firstname += AddHistory(history_of_firstname);
      lastname += AddHistory(history_of_lastname);
      if (firstname == "") return lastname+" with unknown first name";
      if (lastname == "") return firstname+" with unknown last name";
      return firstname + " " + lastname;
  }
// PRIVATE
private:
  string AddHistory(const vector<string> &historyVector) const  {
      string history = "";
      if (!historyVector.empty())
      {
          history +=" (";
          for (auto oneof = historyVector.rbegin(); oneof != historyVector.rend(); oneof++)
          {
            if (history.length()>2) history += ", ";
            history += *oneof;
          }
          history +=")";
      }
      return history;
  }
// FIELDS
  map<int,pair<string,string>> PersonsMap;
  int birthyear;
};