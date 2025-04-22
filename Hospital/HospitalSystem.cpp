#include "HospitalSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <thread>

// Constructor
HospitalSystem::HospitalSystem() : nextPatientId(1) {}

// File handling methods
void HospitalSystem::saveToFile(const std::vector<Patient>& patients, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "ID,Name,Doctor,Department,Priority,Age,Gender,Contact,Symptoms,Arrival Time,Wait Time,Treatment Time\n";
    for (const auto& patient : patients) {
        char timeBuffer[26];
        ctime_s(timeBuffer, sizeof(timeBuffer), &patient.arrivalTime);
        std::string timeStr(timeBuffer);
        timeStr.pop_back(); // Remove newline character

        file << patient.patientId << ","
            << patient.name << ","
            << patient.doctor << ","
            << departmentToString(patient.department) << ","
            << priorityToString(patient.priority) << ","
            << patient.age << ","
            << patient.gender << ","
            << patient.contactNumber << ","
            << patient.symptoms << ","
            << timeStr << ","
            << patient.getWaitTime() << ","
            << patient.getTreatmentTime() << "\n";
    }

    file.close();
    std::cout << "Data saved to " << filename << std::endl;
}

void HospitalSystem::saveDoctorData(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "Name,Specialization,Department,Patients Treated,Current Queue Size\n";
    for (const auto& doctor : doctors) {
        file << doctor.getName() << ","
            << doctor.getSpecialization() << ","
            << departmentToString(doctor.getDepartment()) << ","
            << doctor.getPatientCount() << ","
            << doctor.getQueueSize() << "\n";
    }

    file.close();
    std::cout << "Doctor data saved to " << filename << std::endl;
}

// Doctor management
void HospitalSystem::addDoctor(const std::string& name, const std::string& specialization, Department dept, int maxPatients) {
    doctors.emplace_back(name, specialization, dept, maxPatients);
    std::cout << "Dr. " << name << " (" << specialization << ") added to "
        << departmentToString(dept) << " department." << std::endl;
}

std::vector<Doctor*> HospitalSystem::getAvailableDoctors(Department dept) {
    std::vector<Doctor*> availableDoctors;
    for (auto& doctor : doctors) {
        if (doctor.isAvailable() && (doctor.getDepartment() == dept || dept == GENERAL)) {
            availableDoctors.push_back(&doctor);
        }
    }
    return availableDoctors;
}

Doctor* HospitalSystem::findDoctorWithShortestQueue(Department dept) {
    Doctor* bestDoctor = nullptr;
    int minQueueSize = INT_MAX;

    for (auto& doctor : doctors) {
        if ((doctor.getDepartment() == dept || dept == GENERAL) && !doctor.hasReachedDailyLimit()) {
            int queueSize = doctor.getQueueSize();
            if (queueSize < minQueueSize) {
                minQueueSize = queueSize;
                bestDoctor = &doctor;
            }
        }
    }

    return bestDoctor;
}

Doctor* HospitalSystem::findDoctor(const std::string& name) {
    for (auto& doctor : doctors) {
        if (doctor.getName() == name) {
            return &doctor;
        }
    }
    return nullptr;
}

void HospitalSystem::suggestDoctors(Department dept) {
    auto availableDoctors = getAvailableDoctors(dept);

    if (availableDoctors.empty()) {
        std::cout << "No available doctors in the " << departmentToString(dept) << " department." << std::endl;
        return;
    }

    std::cout << "Available doctors in " << departmentToString(dept) << " department:" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(5) << "No."
        << std::setw(20) << "Name"
        << std::setw(20) << "Specialization"
        << std::setw(15) << "Queue Size"
        << std::setw(10) << "Patients Today" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < availableDoctors.size(); i++) {
        const auto& doctor = availableDoctors[i];
        std::cout << std::left << std::setw(5) << (i + 1)
            << std::setw(20) << doctor->getName()
            << std::setw(20) << doctor->getSpecialization()
            << std::setw(15) << doctor->getQueueSize()
            << std::setw(10) << doctor->getCurrentPatientCount() << std::endl;
    }
}

// Patient management
void HospitalSystem::addPatient(const std::string& name, const std::string& doctorName,
    Priority priority, Department dept, int age, const std::string& gender,
    const std::string& contact, const std::string& symptoms) {

    Doctor* doctor = findDoctor(doctorName);

    // If doctor not found or has reached daily limit, suggest alternative
    if (!doctor || doctor->hasReachedDailyLimit()) {
        std::cout << "Doctor " << doctorName << " is not available or has reached daily patient limit." << std::endl;
        std::cout << "Would you like to see other available doctors? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            suggestDoctors(dept);
            std::cout << "Enter new doctor name (or 'auto' for automatic assignment): ";
            std::string newDoctorName;
            std::cin.ignore();
            std::getline(std::cin, newDoctorName);

            if (newDoctorName == "auto") {
                Doctor* bestDoctor = findDoctorWithShortestQueue(dept);
                if (bestDoctor) {
                    doctor = bestDoctor;
                    std::cout << "Patient automatically assigned to Dr. " << doctor->getName() << std::endl;
                }
                else {
                    std::cout << "No available doctors in this department. Patient added to main queue." << std::endl;
                }
            }
            else {
                doctor = findDoctor(newDoctorName);
                if (!doctor) {
                    std::cout << "Doctor not found. Patient added to main queue." << std::endl;
                }
            }
        }
    }

    // Create new patient with unique ID
    Patient newPatient(name, doctor ? doctor->getName() : doctorName, priority, dept,
        age, gender, contact, symptoms, nextPatientId++);

    // Add to doctor's queue if doctor exists and is available
    if (doctor && doctor->isAvailable()) {
        doctor->queuePatient(newPatient);
        std::cout << priorityToString(priority) << " priority patient " << name
            << " (ID: " << newPatient.patientId << ") added to Dr. " << doctor->getName() << "'s queue"
            << " in " << departmentToString(dept) << " department." << std::endl;
    }
    else {
        // Otherwise add to main queue
        mainQueue.push(newPatient);
        std::cout << priorityToString(priority) << " priority patient " << name
            << " (ID: " << newPatient.patientId << ") added to main queue"
            << " for " << doctorName
            << " in " << departmentToString(dept) << " department." << std::endl;
    }

    statistics.addNewPatient();
}

void HospitalSystem::processDoctorPatient(const std::string& doctorName) {
    Doctor* doctor = findDoctor(doctorName);
    if (!doctor) {
        std::cout << "Doctor not found." << std::endl;
        return;
    }

    Patient currentPatient;
    if (!doctor->getNextPatient(currentPatient)) {
        std::cout << "No patients in Dr. " << doctorName << "'s queue." << std::endl;

        // Try to assign from main queue if doctor has no patients
        if (!mainQueue.empty()) {
            std::cout << "Checking main queue for patients assigned to Dr. " << doctorName << "..." << std::endl;

            std::priority_queue<Patient, std::vector<Patient>, PatientCompare> tempQueue;
            bool found = false;

            while (!mainQueue.empty() && !found) {
                Patient p = mainQueue.top();
                mainQueue.pop();

                if (p.doctor == doctorName) {
                    currentPatient = p;
                    found = true;
                }
                else {
                    tempQueue.push(p);
                }
            }

            // Restore main queue
            while (!tempQueue.empty()) {
                mainQueue.push(tempQueue.top());
                tempQueue.pop();
            }

            if (!found) {
                std::cout << "No patients for Dr. " << doctorName << " in main queue either." << std::endl;
                return;
            }
        }
        else {
            return;
        }
    }

    // Process the patient
    processPatient(currentPatient, doctor);
}

void HospitalSystem::processNextPatient() {
    if (mainQueue.empty()) {
        std::cout << "No patients in the main queue." << std::endl;
        std::cout << "Would you like to check a specific doctor's queue? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            std::cin.ignore();
            std::cout << "Enter doctor name: ";
            std::string doctorName;
            std::getline(std::cin, doctorName);
            processDoctorPatient(doctorName);
        }
        return;
    }

    Patient currentPatient = mainQueue.top();
    mainQueue.pop();

    // Find the assigned doctor
    Doctor* doctor = findDoctor(currentPatient.doctor);
    processPatient(currentPatient, doctor);
}

void HospitalSystem::processPatient(Patient& currentPatient, Doctor* doctor) {
    // Record processing start time
    currentPatient.processingStartTime = time(nullptr);

    // Mark doctor as busy
    if (doctor) {
        doctor->addPatient(currentPatient.name);
    }

    std::cout << "Now processing: " << currentPatient.name << " (ID: " << currentPatient.patientId
        << ", " << priorityToString(currentPatient.priority) << ") for Dr. "
        << currentPatient.doctor << std::endl;

    std::cout << "Department: " << departmentToString(currentPatient.department) << std::endl;
    if (!currentPatient.symptoms.empty()) {
        std::cout << "Symptoms: " << currentPatient.symptoms << std::endl;
    }

    std::cout << "Patient has 5 seconds to decide whether to stay or leave..." << std::endl;

    // Display countdown
    for (int i = 5; i > 0; i--) {
        std::cout << i << "... ";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl;

    // Simulate patient decision
    char decision;
    std::cout << "Did the patient stay? (y/n): ";
    std::cin >> decision;

    // Record processing end time
    currentPatient.processingEndTime = time(nullptr);

    if (decision == 'y' || decision == 'Y') {
        treatedPatients.push_back(currentPatient);
        statistics.addTreatedPatient(currentPatient);
        std::cout << currentPatient.name << " (ID: " << currentPatient.patientId
            << ") has been treated." << std::endl;

        // Doctor completes treatment
        if (doctor) {
            doctor->completePatient();

            // Check if doctor has more patients in their queue
            std::cout << "Dr. " << doctor->getName() << " has "
                << doctor->getQueueSize() << " more patients waiting." << std::endl;
        }
    }
    else {
        departedPatients.push_back(currentPatient);
        statistics.addDepartedPatient(currentPatient);
        std::cout << currentPatient.name << " has left the hospital." << std::endl;

        // Mark doctor as available again
        if (doctor) {
            doctor->completePatient();
        }
    }
}

// Queue display methods
void HospitalSystem::displayAllQueues() {
    std::cout << "\n=== Queue Overview ===\n";
    std::cout << "Main queue: " << mainQueue.size() << " patients\n";

    for (auto& doctor : doctors) {
        std::cout << "Dr. " << doctor.getName() << "'s queue: "
            << doctor.getQueueSize() << " patients\n";
    }

    std::cout << "\nDo you want to see details of: \n"
        << "1. Main queue\n"
        << "2. Specific doctor's queue\n"
        << "3. All queues\n"
        << "Enter choice: ";

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        displayMainQueue();
        break;
    case 2: {
        std::cin.ignore();
        std::cout << "Enter doctor name: ";
        std::string doctorName;
        std::getline(std::cin, doctorName);
        displayDoctorQueue(doctorName);
        break;
    }
    case 3:
        displayMainQueue();
        for (auto& doctor : doctors) {
            displayDoctorQueue(doctor.getName());
        }
        break;
    default:
        std::cout << "Invalid choice.\n";
    }
}

void HospitalSystem::displayMainQueue() {
    if (mainQueue.empty()) {
        std::cout << "Main queue is empty." << std::endl;
        return;
    }

    // Copy queue to temporary vector for display
    std::vector<Patient> tempQueue;
    std::priority_queue<Patient, std::vector<Patient>, PatientCompare> queueCopy = mainQueue;

    while (!queueCopy.empty()) {
        tempQueue.push_back(queueCopy.top());
        queueCopy.pop();
    }

    std::cout << "Main queue (" << tempQueue.size() << " patients):" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(5) << "Pos"
        << std::setw(5) << "ID"
        << std::setw(15) << "Name"
        << std::setw(15) << "Doctor"
        << std::setw(15) << "Department"
        << std::setw(10) << "Priority" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < tempQueue.size(); i++) {
        const Patient& p = tempQueue[i];
        std::cout << std::left << std::setw(5) << (i + 1)
            << std::setw(5) << p.patientId
            << std::setw(15) << p.name
            << std::setw(15) << p.doctor
            << std::setw(15) << departmentToString(p.department)
            << std::setw(10) << priorityToString(p.priority) << std::endl;
    }
}

void HospitalSystem::displayDoctorQueue(const std::string& doctorName) {
    Doctor* doctor = findDoctor(doctorName);
    if (!doctor) {
        std::cout << "Doctor not found." << std::endl;
        return;
    }

    int queueSize = doctor->getQueueSize();
    if (queueSize == 0) {
        std::cout << "Dr. " << doctorName << "'s queue is empty." << std::endl;
        return;
    }

    // We can't easily display the contents without modifying the Doctor class
    // This is a limitation of our current implementation
    std::cout << "Dr. " << doctorName << "'s queue has " << queueSize << " patients waiting." << std::endl;
    std::cout << "Current patient load: " << doctor->getCurrentPatientCount() << "/" << "10" << std::endl;
}

void HospitalSystem::displayQueue() {
    displayMainQueue();
}

// Other functions
void HospitalSystem::saveData() {
    saveToFile(treatedPatients, "treated_patients.csv");
    saveToFile(departedPatients, "departed_patients.csv");
    saveDoctorData("doctors.csv");
}

void HospitalSystem::showStatistics() {
    statistics.displayStatistics();
}

int HospitalSystem::getQueueSize() const {
    int totalSize = mainQueue.size();
    for (const auto& doctor : doctors) {
        totalSize += doctor.getQueueSize();
    }
    return totalSize;
}

void HospitalSystem::startNewDay() {
    for (auto& doctor : doctors) {
        doctor.resetDailyCount();
    }
    std::cout << "New day started. All doctors' patient counts have been reset." << std::endl;
}
