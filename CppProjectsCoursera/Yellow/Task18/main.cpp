#include <iostream>

using namespace std;

class Animal {
public:
    // ваш код
    Animal(const string& nm) : Name(nm) {}
    const string Name;
};


class Dog : public Animal {
public:
    // ваш код
    Dog(const string& nm) : Animal(nm) {}

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};
