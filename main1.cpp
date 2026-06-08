#include "UserVehicle.h"
#include <fstream>
#include <sstream>

void addRecord();

void displayRecords() {
    ifstream file("records.txt");
    string line;

    cout << "\nPlate\tType\tYear\tOwner\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string plate, type, year, owner, email, phone;

        getline(ss, plate, ',');
        getline(ss, type, ',');
        getline(ss, year, ',');
        getline(ss, owner, ',');
        getline(ss, email, ',');
        getline(ss, phone, ',');

        cout << plate << "\t" << type << "\t"
             << year << "\t" << owner << endl;
    }
}

int main() {
    int choice;

    do {
        cout << "\n1. Add Record\n2. Display Records\n0. Exit\nChoice: ";
        cin >> choice;

        switch(choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
        }
    } while(choice != 0);

    return 0;
}
