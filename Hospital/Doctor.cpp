#include "Doctor.h"

Doctor::Doctor(const std::string& n, const std::string& spec, Department dept, int maxPatients)
    : name(n), specialization(spec), department(dept), available(true),
    maxPatientsPerDay(maxPatients), currentPatientCount(0) {
}

const std::string& Doctor::getName() const {
    return name;
}

const std::string& Doctor::getSpecialization() const {
    return specialization;
}

Department Doctor::getDepartment() const {
    return department;
}

bool Doctor::isAvailable() const {
    return available && currentPatientCount < maxPatientsPerDay;
}

int Doctor::getQueueSize() const {
    return doctorQueue.size();
}

bool Doctor::hasReachedDailyLimit() const {
    return currentPatientCount >= maxPatientsPerDay;
}

void Doctor::setMaxPatientsPerDay(int max) {
    maxPatientsPerDay = max;
}

void Doctor::queuePatient(const Patient& patient) {
    doctorQueue.push(patient);
}

bool Doctor::getNextPatient(Patient& outPatient) {
    if (doctorQueue.empty()) {
        return false;
    }
    outPatient = doctorQueue.top();
    doctorQueue.pop();
    return true;
}

void Doctor::setAvailability(bool avail) {
    available = avail;
}

void Doctor::addPatient(const std::string& patientName) {
    patientHistory.push_back(patientName);
    currentPatientCount++;
    available = false; // Doctor becomes busy
}

void Doctor::completePatient() {
    available = true; // Doctor becomes available again
}

void Doctor::resetDailyCount() {
    currentPatientCount = 0;
}

int Doctor::getPatientCount() const {
    return patientHistory.size();
}

int Doctor::getCurrentPatientCount() const {
    return currentPatientCount;
}
