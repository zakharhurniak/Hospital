#pragma once

#include <string>
#include <ctime>
#include "Enums.h"

// Enhanced Patient structure
class Patient {
public:
    std::string name;
    std::string doctor;
    time_t arrivalTime;
    time_t processingStartTime;
    time_t processingEndTime;
    Priority priority;
    Department department;
    int age;
    std::string gender;
    std::string contactNumber;
    std::string symptoms;
    int patientId;

    Patient();

    Patient(const std::string& n, const std::string& d, Priority p = NORMAL,
        Department dept = GENERAL, int a = 0, const std::string& g = "",
        const std::string& c = "", const std::string& s = "", int id = 0);

    double getWaitTime() const;
    double getTreatmentTime() const;
};

// Patient comparison for priority queue
struct PatientCompare {
    bool operator()(const Patient& a, const Patient& b) const;
};
