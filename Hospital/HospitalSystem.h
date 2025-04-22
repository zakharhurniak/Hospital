#pragma once

#include <queue>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "Patient.h"
#include "Doctor.h"
#include "HospitalStatistics.h"

// Enhanced Hospital System
class HospitalSystem {
private:
    std::priority_queue<Patient, std::vector<Patient>, PatientCompare> mainQueue;
    std::unordered_map<std::string, std::priority_queue<Patient, std::vector<Patient>, PatientCompare>> doctorQueues;
    std::vector<Patient> treatedPatients;
    std::vector<Patient> departedPatients;
    std::vector<Doctor> doctors;
    HospitalStatistics statistics;
    int nextPatientId;

    // File handling methods
    void saveToFile(const std::vector<Patient>& patients, const std::string& filename);
    void saveDoctorData(const std::string& filename);
    void processPatient(Patient& currentPatient, Doctor* doctor);

public:
    HospitalSystem();

    // Doctor management
    void addDoctor(const std::string& name, const std::string& specialization, Department dept, int maxPatients = 10);
    std::vector<Doctor*> getAvailableDoctors(Department dept = GENERAL);
    Doctor* findDoctorWithShortestQueue(Department dept = GENERAL);
    Doctor* findDoctor(const std::string& name);
    void suggestDoctors(Department dept);

    // Patient management
    void addPatient(const std::string& name, const std::string& doctorName,
        Priority priority = NORMAL, Department dept = GENERAL,
        int age = 0, const std::string& gender = "",
        const std::string& contact = "", const std::string& symptoms = "");
    void processDoctorPatient(const std::string& doctorName);
    void processNextPatient();

    // Queue display methods
    void displayAllQueues();
    void displayMainQueue();
    void displayDoctorQueue(const std::string& doctorName);
    void displayQueue();

    // Other functions
    void saveData();
    void showStatistics();
    int getQueueSize() const;
    void startNewDay();
};
