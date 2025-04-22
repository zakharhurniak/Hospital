#include "Enums.h"

std::string priorityToString(Priority p) {
    switch (p) {
    case NORMAL: return "Normal";
    case URGENT: return "Urgent";
    case EMERGENCY: return "Emergency";
    default: return "Unknown";
    }
}

std::string departmentToString(Department d) {
    switch (d) {
    case GENERAL: return "General";
    case CARDIOLOGY: return "Cardiology";
    case PEDIATRICS: return "Pediatrics";
    case NEUROLOGY: return "Neurology";
    case ORTHOPEDICS: return "Orthopedics";
    default: return "Unknown";
    }
}
