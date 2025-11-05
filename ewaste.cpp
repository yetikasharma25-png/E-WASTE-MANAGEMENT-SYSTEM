#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib> // Added for atoi
#include <limits> // Added for numeric_limits
using namespace std;

struct Ewaste {
    int id;
    string name;
    string category;
    string condition;
    string status; // "Stored", "Donated", "Recycled"
};

void addItem() {
    ofstream file("ewaste.txt", ios::app);
    Ewaste item;
    cout << "\nEnter Item ID: ";
    
    // Check for valid number
    while (!(cin >> item.id)) {
        cout << "\nInvalid input! Please enter a number for ID: ";
        cin.clear(); // Clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input line
    }
    
    // Clear the newline left by cin >>
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Enter Item Name: ";
    getline(cin, item.name);
    cout << "Enter Category (e.g. Mobile, Laptop, TV): ";
    getline(cin, item.category);
    cout << "Enter Condition (Good / Damaged): ";
    getline(cin, item.condition);
    item.status = "Stored";

    file << item.id << "|" << item.name << "|" << item.category << "|" 
         << item.condition << "|" << item.status << endl;

    file.close();
    cout << "\nItem added successfully!\n";
}

void viewAll() {
    ifstream file("ewaste.txt");
    Ewaste item;
    string line;
    cout << "\n------------------ E-WASTE ITEMS ------------------\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) 
         << "Category" << setw(15) << "Condition" << setw(15) << "Status" << endl;
    cout << "----------------------------------------------------\n";

    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t pos = 0;
        string data[5];
        int i = 0;
        // Correctly parse the line, even the last part
        while ((pos = line.find('|')) != string::npos && i < 4) { // Changed to i < 4
            data[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        data[i] = line; // Assign the last part (or the whole line if no '|')

        // Ensure we have 5 data points before trying to print
        if (i == 4) {
            cout << left << setw(10) << data[0] << setw(20) << data[1] << setw(15)
                 << data[2] << setw(15) << data[3] << setw(15) << data[4] << endl;
        }
    }
    file.close();
}

void updateStatus(int id, string newStatus) {
    ifstream file("ewaste.txt");
    ofstream temp("temp.txt");
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.empty()) continue;
        
        string originalLine = line; // Keep a copy
        size_t pos = 0;
        string data[5];
        int i = 0;
        
        // Parse the line
        while ((pos = line.find('|')) != string::npos && i < 4) { // Changed to i < 4
            data[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        data[i] = line; // Assign the last part

        if (i == 4) { // Check if parsing was successful
            // *** THIS IS THE FIX ***
            // Changed stoi(data[0]) to atoi(data[0].c_str())
            if (atoi(data[0].c_str()) == id) {
                data[4] = newStatus;
                found = true;
                cout << "\nItem status updated to " << newStatus << "!\n";
                // Write the modified data
                temp << data[0] << "|" << data[1] << "|" << data[2] << "|" 
                     << data[3] << "|" << data[4] << endl;
            } else {
                // Write the original line if no match
                temp << originalLine << endl;
            }
        } else {
             // Write the original line if parsing failed (e.g., bad format)
            temp << originalLine << endl;
        }
    }

    file.close();
    temp.close();
    remove("ewaste.txt");
    rename("temp.txt", "ewaste.txt");

    if (!found)
        cout << "\nItem not found!\n";
}

int main() {
    int choice, id;
    do {
        cout << "\n========== E-WASTE MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add E-waste Item\n";
        cout << "2. View All Items\n";
        cout << "3. Donate Item\n";
        cout << "4. Recycle Item\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        
        // Check for valid number
        while (!(cin >> choice)) {
            cout << "\nInvalid input! Please enter a number (1-5): ";
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input line
        }

        // Clear the newline left by cin >>
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            viewAll();
            break;
        case 3:
            cout << "Enter Item ID to mark as Donated: ";
            // Check for valid number
            while (!(cin >> id)) {
                cout << "\nInvalid input! Please enter a number for ID: ";
                cin.clear(); // Clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input line
            }
            // Clear the newline left by cin >>
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            updateStatus(id, "Donated");
            break;
        case 4:
            cout << "Enter Item ID to mark as Recycled: ";
            // Check for valid number
            while (!(cin >> id)) {
                cout << "\nInvalid input! Please enter a number for ID: ";
                cin.clear(); // Clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input line
            }
            // Clear the newline left by cin >>
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            updateStatus(id, "Recycled");
            break;
        case 5:
            cout << "\nExiting... Thank you!\n";
            break;
        default:
            cout << "\nInvalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
