#pragma once

#include <map>
#include "Patient.h"
#include "Enums.h"

// Hospital statistics class
class HospitalStatistics {
private:
    int totalPatients;
    int treatedPatients;
    int departedPatients;
    double totalWaitTime;
    double totalTreatmentTime;
    std::map<std::string, int> patientsByDoctor;
    std::map<Department, int> patientsByDepartment;
    std::map<std::string, double> averageWaitTimeByDoctor;

public:
    HospitalStatistics();

    void addNewPatient();
    void addTreatedPatient(const Patient& patient);
    void addDepartedPatient(const Patient& patient);
    void displayStatistics() const;
};
