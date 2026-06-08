#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;
class User {
private:
    string name;
    string email;
    string phone;

public:
    User() : name(""), email(""), phone("") {}
    User(const string& n, const string& e, const string& p)
        : name(n), email(e), phone(p) {}

    string getName()  const { return name;  }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }

    void setName (const string& n) { name  = n; }
    void setEmail(const string& e) { email = e; }
    void setPhone(const string& p) { phone = p; }
    virtual ~User() {}
};
class VehicleOwner : public User {
public:
    VehicleOwner() : User() {}
    VehicleOwner(const string& n, const string& e, const string& p)
        : User(n, e, p) {}
};
class Vehicle {
private:
    string       plateNumber;
    string       vehicleType;
    int          year;
    VehicleOwner owner;  

public:
    Vehicle() : plateNumber(""), vehicleType(""), year(0) {}
    Vehicle(const string& plate, const string& type, int yr,
            const VehicleOwner& o)
        : plateNumber(plate), vehicleType(type), year(yr), owner(o) {}

    string       getPlate() const { return plateNumber; }
    string       getType()  const { return vehicleType; }
    int          getYear()  const { return year;        }
    VehicleOwner getOwner() const { return owner;       }
};
bool isValidEmail(const string& email) {
    if (email.empty()) return false;
    char first = email.front(), last = email.back();
    if (first == '@' || first == '.' || first == '-' || first == ' ') return false;
    if (last  == '@' || last  == '.' || last  == '-' || last  == ' ') return false;

    bool hasAt  = (email.find('@') != string::npos);
    bool hasDot = (email.find('.') != string::npos);
    return hasAt && hasDot;
}
bool isValidPhone(const string& phone) {
    if (phone.size() != 13) return false;
    if (phone.substr(0, 4) != "+250") return false;
    for (size_t i = 4; i < phone.size(); ++i)
        if (!isdigit(phone[i])) return false;
    return true;
}
bool isValidPlate(const string& plate) {
    if (plate.size() != 7) return false;
    for (int i = 0; i < 3; ++i)
        if (!isalpha(plate[i])) return false;
    for (int i = 3; i < 6; ++i)
        if (!isdigit(plate[i])) return false;
    if (!isalpha(plate[6])) return false;
    return true;
}
const string FILENAME = "records.txt";
void saveRecord(const Vehicle& v) {
    ofstream file(FILENAME, ios::app);
    if (!file) {
        cerr << "  [ERROR] Cannot open " << FILENAME << " for writing.\n";
        return;
    }
    file << v.getPlate()            << ","
         << v.getType()             << ","
         << v.getYear()             << ","
         << v.getOwner().getName()  << ","
         << v.getOwner().getEmail() << ","
         << v.getOwner().getPhone() << "\n";
    file.close();
}
vector<Vehicle> loadRecords() {
    vector<Vehicle> records;
    ifstream file(FILENAME);
    if (!file) return records;  

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string plate, type, yearStr, name, email, phone;

        getline(ss, plate,   ',');
        getline(ss, type,    ',');
        getline(ss, yearStr, ',');
        getline(ss, name,    ',');
        getline(ss, email,   ',');
        getline(ss, phone,   ',');

        int year = 0;
        try { year = stoi(yearStr); } catch (...) { year = 0; }

        VehicleOwner owner(name, email, phone);
        Vehicle v(plate, type, year, owner);
        records.push_back(v);
    }
    file.close();
    return records;
}
void displayAll() {
    vector<Vehicle> records = loadRecords();

    if (records.empty()) {
        cout << "\n  No records found in " << FILENAME << ".\n";
        return;
    }
    cout << "\n";
    cout << string(95, '-') << "\n";
    cout << left
         << setw(10) << "Plate"
         << setw(12) << "Type"
         << setw(6)  << "Year"
         << setw(18) << "Owner Name"
         << setw(28) << "Email"
         << setw(15) << "Phone"
         << "\n";
    cout << string(95, '-') << "\n";

    for (const Vehicle& v : records) {
        cout << left
             << setw(10) << v.getPlate()
             << setw(12) << v.getType()
             << setw(6)  << v.getYear()
             << setw(18) << v.getOwner().getName()
             << setw(28) << v.getOwner().getEmail()
             << setw(15) << v.getOwner().getPhone()
             << "\n";
    }
    cout << string(95, '-') << "\n";
    cout << "  Total records: " << records.size() << "\n";
}
void searchByPlate() {
    string query;
    cout << "\n  Enter plate number to search: ";
    cin >> query;

    transform(query.begin(), query.end(), query.begin(), ::toupper);
    vector<Vehicle> records = loadRecords();
    bool found = false;

    for (const Vehicle& v : records) {
        string plate = v.getPlate();
        transform(plate.begin(), plate.end(), plate.begin(), ::toupper);

        if (plate == query) {
            found = true;
            cout << "\n  ---- Vehicle Found ----\n";
            cout << "  Plate Number : " << v.getPlate()            << "\n";
            cout << "  Vehicle Type : " << v.getType()             << "\n";
            cout << "  Year         : " << v.getYear()             << "\n";
            cout << "  Owner Name   : " << v.getOwner().getName()  << "\n";
            cout << "  Email        : " << v.getOwner().getEmail() << "\n";
            cout << "  Phone        : " << v.getOwner().getPhone() << "\n";
            cout << "  -----------------------\n";
            break;
        }
    }

    if (!found)
        cout << "\n  [NOT FOUND] No vehicle with plate \"" << query << "\" exists.\n";
}
void addRecord() {
    string plate, type, yearStr, name, email, phone;
    int year;
    cout << "\n  --- Add New Vehicle Record ---\n";
    
    while (true) {
        cout << "  Plate Number (e.g. RAA123A): ";
        cin >> plate;
        transform(plate.begin(), plate.end(), plate.begin(), ::toupper);
        if (isValidPlate(plate)) break;
        cout << "  [INVALID] Format must be 3 letters + 3 digits + 1 letter (e.g. RAA123A).\n";
    }
    cout << "  Vehicle Type (e.g. Sedan, SUV, Truck): ";
    cin.ignore();
    getline(cin, type);

    while (true) {
        cout << "  Year (e.g. 2020): ";
        cin >> yearStr;
        bool allDigits = !yearStr.empty() &&
                         all_of(yearStr.begin(), yearStr.end(), ::isdigit);
        if (allDigits) {
            year = stoi(yearStr);
            if (year >= 1900 && year <= 2100) break;
        }
        cout << "  [INVALID] Please enter a valid 4-digit year.\n";
    }
    cin.ignore();
    cout << "  Owner Full Name: ";
    getline(cin, name);

    while (true) {
        cout << "  Owner Email: ";
        cin >> email;
        if (isValidEmail(email)) break;
        cout << "  [INVALID] Email must contain '@' and '.', with no invalid start/end chars.\n";
    }
    while (true) {
        cout << "  Owner Phone (+250XXXXXXXXX): ";
        cin >> phone;
        if (isValidPhone(phone)) break;
        cout << "  [INVALID] Phone must start with +250 and be exactly 13 characters.\n";
    }

    VehicleOwner owner(name, email, phone);
    Vehicle v(plate, type, year, owner);
    saveRecord(v);

    cout << "\n  [SUCCESS] Record saved to " << FILENAME << ".\n";
}
int main() {
    int choice;

    cout << "======================================\n";
    cout << "  Vehicle Registration & Owner System \n";
    cout << "======================================\n";

    do {
        cout << "\n  MENU\n";
        cout << "  1. Add Vehicle Record\n";
        cout << "  2. Display All Records\n";
        cout << "  3. Search by Plate Number\n";
        cout << "  4. Exit\n";
        cout << "  Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addRecord();    break;
            case 2: displayAll();   break;
            case 3: searchByPlate(); break;
            case 4: cout << "\n  Goodbye!\n"; break;
            default: cout << "  [ERROR] Invalid option. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}