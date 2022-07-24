#pragma once
#include <string>

using namespace std;

enum class AnimalType {
    Cat,
    Dog,
    Mouse,
};

class Animal {
public:
    AnimalType GetType() const {
        return type_;
    }

    const string& GetName() const {
        return name_;
    }

    const string& GetOwnerName() const {
        return owner_name_;
    }

    Animal(AnimalType type, string name, string owner_name):type_(type), name_(name), owner_name_(owner_name) {}
private:
    AnimalType type_;
    string name_;
    string owner_name_;
};