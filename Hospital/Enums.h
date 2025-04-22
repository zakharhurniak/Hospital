#pragma once

#include <string>

// Enums for patient priority and hospital departments
enum Priority { NORMAL = 0, URGENT = 1, EMERGENCY = 2 };
enum Department { GENERAL, CARDIOLOGY, PEDIATRICS, NEUROLOGY, ORTHOPEDICS };

// Convert enum to string
std::string priorityToString(Priority p);
std::string departmentToString(Department d);
