#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Employee {
private:
    string empID;
    string name;
    double basicPay;
    int otHours;
    const double OT_RATE = 500.0;

public:
    Employee(string id, string n, double basic, int ot) {
        empID = id;
        name = n;
        basicPay = basic;
        otHours = ot;
    }

    string getID() const { return empID; }
    string getName() const { return name; }
    double getBasic() const { return basicPay; }
    int getOT() const { return otHours; }

    void setOT(int ot) { otHours = ot; }

    double calculateGross() const {
        double hra = basicPay * 0.20;
        double da = basicPay * 0.10;
        double otPay = otHours * OT_RATE;
        return basicPay + hra + da + otPay;
    }

    double calculateTax() const {
        double gross = calculateGross();
        if (gross <= 30000) return 0.0;
        else if (gross <= 50000) return gross * 0.05;
        else return gross * 0.10;
    }

    double calculateNetPay() const {
        return calculateGross() - calculateTax();
    }

    void printPayslip() const {
        double hra = basicPay * 0.20;
        double da = basicPay * 0.10;
        double otPay = otHours * OT_RATE;
        double gross = calculateGross();
        double tax = calculateTax();
        double net = calculateNetPay();

        cout << "\n========================================\n";
        cout << "          OFFICIAL SALARY SLIP          \n";
        cout << "========================================\n";
        cout << left << setw(20) << "Employee ID:" << empID << "\n";
        cout << left << setw(20) << "Employee Name:" << name << "\n";
        cout << "----------------------------------------\n";
        cout << "EARNINGS\n";
        cout << left << setw(20) << "Basic Pay:" << "Rs " << fixed << setprecision(2) << basicPay << "\n";
        cout << left << setw(20) << "HRA (20%):" << "Rs " << hra << "\n";
        cout << left << setw(20) << "DA (10%):" << "Rs " << da << "\n";
        cout << left << setw(20) << "Overtime Pay:" << "Rs " << otPay << " (" << otHours << " hrs)\n";
        cout << "----------------------------------------\n";
        cout << left << setw(20) << "GROSS EARNINGS:" << "Rs " << gross << "\n";
        cout << "----------------------------------------\n";
        cout << "DEDUCTIONS\n";
        cout << left << setw(20) << "Income Tax:" << "Rs " << tax << "\n";
        cout << "========================================\n";
        cout << left << setw(20) << "NET PAY:" << "Rs " << net << "\n";
        cout << "========================================\n";
    }
};

class PayrollSystem {
private:
    vector<Employee> employees;
    string filename = "payroll_data.txt";

    bool isIDUnique(string id) {
        for (const auto& emp : employees) {
            if (emp.getID() == id) return false;
        }
        return true;
    }

public:
    void loadData() {
        ifstream file(filename);
        if (!file) return;

        string id, name;
        double basic;
        int ot;
        while (file >> id >> name >> basic >> ot) {
            employees.push_back(Employee(id, name, basic, ot));
        }
        file.close();
    }

    void saveData() {
        ofstream file(filename);
        for (const auto& emp : employees) {
            file << emp.getID() << " "
                 << emp.getName() << " "
                 << emp.getBasic() << " "
                 << emp.getOT() << "\n";
        }
        file.close();
    }

    void addEmployee() {
        string id, name;
        double basic;
        int ot;

        cout << "\nEnter Employee ID: ";
        cin >> id;

        if (!isIDUnique(id)) {
            cout << "[ERROR] Employee ID already exists!\n";
            return;
        }

        cout << "Enter First Name: ";
        cin >> name;

        cout << "Enter Basic Pay (Rs): ";
        while (!(cin >> basic) || basic < 0) {
            cout << "[ERROR] Invalid Pay. Enter a positive number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        cout << "Enter Overtime Hours: ";
        while (!(cin >> ot) || ot < 0) {
            cout << "[ERROR] Invalid Hours. Enter a positive number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        employees.push_back(Employee(id, name, basic, ot));
        cout << "\n[SUCCESS] Employee added successfully!\n";
    }

    void updateOvertime() {
        string id;
        int newOT;
        cout << "\nEnter Employee ID to update: ";
        cin >> id;

        for (auto& emp : employees) {
            if (emp.getID() == id) {
                cout << "Enter new Overtime Hours: ";
                while (!(cin >> newOT) || newOT < 0) {
                    cout << "[ERROR] Invalid Hours. Enter a positive number: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                emp.setOT(newOT);
                cout << "[SUCCESS] Overtime updated!\n";
                return;
            }
        }
        cout << "[ERROR] Employee ID not found.\n";
    }

    void generatePayslip() {
        string id;
        cout << "\nEnter Employee ID for Payslip: ";
        cin >> id;

        for (const auto& emp : employees) {
            if (emp.getID() == id) {
                emp.printPayslip();
                return;
            }
        }
        cout << "[ERROR] Employee ID not found.\n";
    }

    void reportTotalPayout() {
        if (employees.empty()) {
            cout << "\nNo employees in the system.\n";
            return;
        }
        double total = 0;
        for (const auto& emp : employees) {
            total += emp.calculateNetPay();
        }
        cout << "\n----------------------------------------\n";
        cout << "TOTAL MONTHLY PAYOUT: Rs " << fixed << setprecision(2) << total << "\n";
        cout << "----------------------------------------\n";
    }

    void reportHighestPaid() {
        if (employees.empty()) {
            cout << "\nNo employees in the system.\n";
            return;
        }

        auto highest = max_element(employees.begin(), employees.end(),
            [](const Employee& a, const Employee& b) {
                return a.calculateNetPay() < b.calculateNetPay();
            });

        cout << "\n--- HIGHEST PAID EMPLOYEE ---\n";
        cout << "Name: " << highest->getName() << " (ID: " << highest->getID() << ")\n";
        cout << "Net Pay: Rs " << fixed << setprecision(2) << highest->calculateNetPay() << "\n";
    }
};

int main() {
    PayrollSystem system;
    system.loadData();

    int choice;
    while (true) {
        cout << "\n========================================\n";
        cout << "    PAYROLL & SALARY SLIP GENERATOR     \n";
        cout << "========================================\n";
        cout << "1. Add New Employee\n";
        cout << "2. Update Overtime Hours\n";
        cout << "3. Generate Employee Payslip\n";
        cout << "4. Report: Total Company Payout\n";
        cout << "5. Report: Highest Paid Employee\n";
        cout << "6. Exit & Save Data\n";
        cout << "----------------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: system.addEmployee(); break;
            case 2: system.updateOvertime(); break;
            case 3: system.generatePayslip(); break;
            case 4: system.reportTotalPayout(); break;
            case 5: system.reportHighestPaid(); break;
            case 6:
                system.saveData();
                cout << "\nData saved securely. Exiting Program... Goodbye!\n";
                return 0;
            default:
                cout << "\n[ERROR] Invalid choice. Select between 1-6.\n";
        }
    }
    return 0;
}