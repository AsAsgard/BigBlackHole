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

int main() {
  Person person{"Aleksandra","Rabova",1964};

  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;

  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;

  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;


  Person personX("Polina", "Sergeeva", 1960);
    for (int year : {1959, 1960}) {
      cout << personX.GetFullNameWithHistory(year) << endl;
    }

    personX.ChangeFirstName(1965, "Appolinaria");
    personX.ChangeLastName(1967, "Ivanova");
    for (int year : {1965, 1967}) {
      cout << personX.GetFullNameWithHistory(year) << endl;
    }

  return 0;
}
