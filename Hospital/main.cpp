#include <iostream>
#include "HospitalSystem.h"
#include "Utils.h"

int main() {
    HospitalSystem hospital;
    int choice;

    do {
        std::cout << "\n=== Hospital Patient Management System ===\n";
        std::cout << "1. Add patient to queue\n";
        std::cout << "2. Process next patient from main queue\n";
        std::cout << "3. Process patient from specific doctor's queue\n";
        std::cout << "4. Display queues (" << hospital.getQueueSize() << " total patients waiting)\n";
        std::cout << "5. Add a new doctor\n";
        std::cout << "6. View statistics\n";
        std::cout << "7. Start new day (reset doctor counts)\n";
        std::cout << "8. Save data and exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name, doctor, gender, contact, symptoms;
            int age;

            std::cin.ignore();
            std::cout << "Enter patient name: ";
            std::getline(std::cin, name);

            std::cout << "Enter doctor name (or 'auto' for automatic assignment): ";
            std::getline(std::cin, doctor);

            std::cout << "Enter patient age: ";
            std::cin >> age;
            std::cin.ignore();

            std::cout << "Enter patient gender: ";
            std::getline(std::cin, gender);

            std::cout << "Enter contact number: ";
            std::getline(std::cin, contact);

            std::cout << "Enter symptoms: ";
            std::getline(std::cin, symptoms);

            Priority priority = selectPriority();
            Department department = selectDepartment();

            if (doctor == "auto") {
                hospital.suggestDoctors(department);
                std::cout << "Enter doctor name: ";
                std::getline(std::cin, doctor);
            }

            hospital.addPatient(name, doctor, priority, department, age, gender, contact, symptoms);
            break;
        }
        case 2:
            hospital.processNextPatient();
            break;
        case 3: {
            std::cin.ignore();
            std::cout << "Enter doctor name: ";
            std::string doctorName;
            std::getline(std::cin, doctorName);
            hospital.processDoctorPatient(doctorName);
            break;
        }
        case 4:
            hospital.displayAllQueues();
            break;
        case 5: {
            std::string name, specialization;
            int maxPatients;

            std::cin.ignore();
            std::cout << "Enter doctor name: ";
            std::getline(std::cin, name);

            std::cout << "Enter specialization: ";
            std::getline(std::cin, specialization);

            std::cout << "Enter maximum patients per day: ";
            std::cin >> maxPatients;

            Department department = selectDepartment();

            hospital.addDoctor(name, specialization, department, maxPatients);
            break;
        }
        case 6:
            hospital.showStatistics();
            break;
        case 7:
            hospital.startNewDay();
            break;
        case 8:
            hospital.saveData();
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
