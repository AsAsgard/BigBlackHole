#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name, const string& type) : Name(name), Type(type) {}
    // every class with even one virtual method needs virtual destructor
    virtual ~Person() = 0;

    virtual void Walk(const string& destination) const {
        cout << type_name() << " walks to: " << destination << endl;
    }
    string name() const { return Name;}
    string type() const { return Type;}

protected:
    // methods
    string type_name() const { return Type + ": " + Name;}
    // fields
    string Name;
    const string Type;
};
inline Person::~Person() {}


class Student : public Person {
public:

    Student(const string& name, const string& favouriteSong)
    : Person(name, "Student"),
      FavouriteSong(favouriteSong) {}

    void Learn() const {
        cout << type_name() << " learns" << endl;
    }

    void Walk(const string& destination) const override {
        Person::Walk(destination);
        SingSong();
    }

    void SingSong() const {
        cout << type_name() << " sings a song: " << FavouriteSong << endl;
    }

private:
    string FavouriteSong;
};


class Teacher : public Person {
public:

    Teacher(const string& name, const string& subject)
    : Person (name, "Teacher"),
      Subject(subject) {}

    void Teach() const {
        cout << type_name() << " teaches: " << Subject << endl;
    }

private:
    string Subject;
};


class Policeman : public Person {
public:

    Policeman(const string& name) : Person(name, "Policeman") {}

    void Check(const Person& pers) {
        cout << type_name() << " checks " << pers.type() << ". " << pers.type() << "'s name is: " << pers.name() << endl;
    }
};


void VisitPlaces(const Person& pers, const vector<string>& places) {
    for (const auto& p : places) {
        pers.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    p.Check(t);
    VisitPlaces(p,{"Pekin","New York","Frankfurt"});
    return 0;
}
