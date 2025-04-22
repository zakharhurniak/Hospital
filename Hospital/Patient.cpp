#include "Patient.h"

Patient::Patient() :
    name(""), doctor(""), priority(NORMAL), department(GENERAL),
    age(0), gender(""), contactNumber(""), symptoms(""), patientId(0) {
    arrivalTime = time(nullptr);
    processingStartTime = 0;
    processingEndTime = 0;
}

Patient::Patient(const std::string& n, const std::string& d, Priority p,
    Department dept, int a, const std::string& g,
    const std::string& c, const std::string& s, int id)
    : name(n), doctor(d), priority(p), department(dept),
    age(a), gender(g), contactNumber(c), symptoms(s), patientId(id) {
    arrivalTime = time(nullptr);
    processingStartTime = 0;
    processingEndTime = 0;
}

double Patient::getWaitTime() const {
    if (processingStartTime == 0) return 0;
    return difftime(processingStartTime, arrivalTime);
}

double Patient::getTreatmentTime() const {
    if (processingEndTime == 0 || processingStartTime == 0) return 0;
    return difftime(processingEndTime, processingStartTime);
}

bool PatientCompare::operator()(const Patient& a, const Patient& b) const {
    if (a.priority != b.priority)
        return a.priority < b.priority;  // Higher priority first
    return a.arrivalTime > b.arrivalTime; // Then FIFO
}
