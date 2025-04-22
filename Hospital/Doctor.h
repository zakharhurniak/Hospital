#pragma once

#include <string>
#include <vector>
#include <queue>
#include "Patient.h"
#include "Enums.h"

// Doctor class with dedicated queue
class Doctor {
private:
    std::string name;
    std::string specialization;
    Department department;
    bool available;
    std::vector<std::string> patientHistory;
    std::priority_queue<Patient, std::vector<Patient>, PatientCompare> doctorQueue;
    int maxPatientsPerDay;
    int currentPatientCount;

public:
    Doctor(const std::string& n, const std::string& spec, Department dept, int maxPatients = 10);

    const std::string& getName() const;
    const std::string& getSpecialization() const;
    Department getDepartment() const;
    bool isAvailable() const;

    // Queue management methods
    int getQueueSize() const;
    bool hasReachedDailyLimit() const;
    void setMaxPatientsPerDay(int max);
    void queuePatient(const Patient& patient);
    bool getNextPatient(Patient& outPatient);

    // Availability and patient management
    void setAvailability(bool avail);
    void addPatient(const std::string& patientName);
    void completePatient();
    void resetDailyCount();
    int getPatientCount() const;
    int getCurrentPatientCount() const;
};
