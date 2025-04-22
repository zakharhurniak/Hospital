#include "Utils.h"
#include <iostream>

// Department selection menu
Department selectDepartment() {
    int choice;
    std::cout << "Select department:\n"
        << "1. General\n"
        << "2. Cardiology\n"
        << "3. Pediatrics\n"
        << "4. Neurology\n"
        << "5. Orthopedics\n"
        << "Enter choice: ";
    std::cin >> choice;

    switch (choice) {
    case 2: return CARDIOLOGY;
    case 3: return PEDIATRICS;
    case 4: return NEUROLOGY;
    case 5: return ORTHOPEDICS;
    default: return GENERAL;
    }
}

// Priority selection menu
Priority selectPriority() {
    int choice;
    std::cout << "Select priority:\n"
        << "1. Normal\n"
        << "2. Urgent\n"
        << "3. Emergency\n"
        << "Enter choice: ";
    std::cin >> choice;

    switch (choice) {
    case 2: return URGENT;
    case 3: return EMERGENCY;
    default: return NORMAL;
    }
}
