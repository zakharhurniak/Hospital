#include "HospitalStatistics.h"
#include <iostream>
#include <iomanip>

HospitalStatistics::HospitalStatistics()
    : totalPatients(0), treatedPatients(0), departedPatients(0),
    totalWaitTime(0), totalTreatmentTime(0) {
}

void HospitalStatistics::addNewPatient() {
    totalPatients++;
}

void HospitalStatistics::addTreatedPatient(const Patient& patient) {
    treatedPatients++;
    double waitTime = patient.getWaitTime();
    double treatmentTime = patient.getTreatmentTime();

    totalWaitTime += waitTime;
    totalTreatmentTime += treatmentTime;

    patientsByDoctor[patient.doctor]++;
    patientsByDepartment[patient.department]++;

    // Update average wait time for this doctor
    if (averageWaitTimeByDoctor.find(patient.doctor) == averageWaitTimeByDoctor.end()) {
        averageWaitTimeByDoctor[patient.doctor] = waitTime;
    }
    else {
        double currentAvg = averageWaitTimeByDoctor[patient.doctor];
        int doctorPatients = patientsByDoctor[patient.doctor];
        averageWaitTimeByDoctor[patient.doctor] =
            ((currentAvg * (doctorPatients - 1)) + waitTime) / doctorPatients;
    }
}

void HospitalStatistics::addDepartedPatient(const Patient& patient) {
    departedPatients++;
}

void HospitalStatistics::displayStatistics() const {
    std::cout << "\n=== Hospital Statistics ===\n";
    std::cout << "Total patients: " << totalPatients << std::endl;
    std::cout << "Treated patients: " << treatedPatients << std::endl;
    std::cout << "Departed patients: " << departedPatients << std::endl;

    if (treatedPatients > 0) {
        std::cout << "Average wait time: " << std::fixed << std::setprecision(2)
            << (totalWaitTime / treatedPatients) << " seconds" << std::endl;
        std::cout << "Average treatment time: " << std::fixed << std::setprecision(2)
            << (totalTreatmentTime / treatedPatients) << " seconds" << std::endl;
    }

    std::cout << "\nPatients by Doctor:\n";
    for (const auto& pair : patientsByDoctor) {
        std::cout << pair.first << ": " << pair.second;
        auto waitTime = averageWaitTimeByDoctor.find(pair.first);
        if (waitTime != averageWaitTimeByDoctor.end()) {
            std::cout << " (Avg wait: " << std::fixed << std::setprecision(2)
                << waitTime->second << " seconds)";
        }
        std::cout << std::endl;
    }

    std::cout << "\nPatients by Department:\n";
    for (const auto& pair : patientsByDepartment) {
        std::cout << departmentToString(pair.first) << ": " << pair.second << std::endl;
    }
}
