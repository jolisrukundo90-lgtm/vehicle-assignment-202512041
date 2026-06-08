#ifndef USERVEHICLE_H
#define USERVEHICLE_H

#include <iostream>
#include <string>
using namespace std;

class User {
private:
    string name, email, phone;

public:
    User(string n="", string e="", string p="")
        : name(n), email(e), phone(p) {}

    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
};

class VehicleOwner : public User {
public:
    VehicleOwner(string n="", string e="", string p="")
        : User(n,e,p) {}
};

class Vehicle {
private:
    string plateNumber, vehicleType;
    int year;
    VehicleOwner owner;

public:
    Vehicle(string plate="", string type="", int y=0,
            VehicleOwner o=VehicleOwner())
        : plateNumber(plate), vehicleType(type), year(y), owner(o) {}

    string toCSV() const {
        return plateNumber + "," + vehicleType + "," +
               to_string(year) + "," +
               owner.getName() + "," +
               owner.getEmail() + "," +
               owner.getPhone();
    }
};

#endif
