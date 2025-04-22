## Overview

The Hospital Patient Management System is a comprehensive C++ application designed to manage patient queues, doctor workflows, and hospital statistics. It provides an electronic queuing system that tracks patients through their hospital visit, from arrival to treatment or departure, with features for prioritizing emergency cases and optimizing doctor assignments.

## Features

- **Priority-based Patient Queues**: Patients are assigned priority levels (Normal, Urgent, Emergency) that determine their position in queues.
- **Multi-department Support**: Supports multiple medical departments (General, Cardiology, Pediatrics, Neurology, Orthopedics).
- **Doctor Management**: Tracks doctor availability, specialization, and patient load.
- **Intelligent Patient Assignment**: Can automatically assign patients to doctors with the shortest queues.
- **Patient Decision Timer**: Simulates a 5-second decision period for patients to decide whether to stay or leave.
- **Comprehensive Statistics**: Tracks wait times, treatment times, and patient distribution across departments and doctors.
- **Data Persistence**: Saves patient and doctor information to CSV files for record-keeping.
- **Daily Reset**: Ability to start a new day and reset doctor patient counts.

## System Requirements

- C++14 compatible compiler
- Windows operating system (for `ctime_s` function)
- No external dependencies beyond the C++ Standard Library

## File Structure

- **Enums.h/cpp**: Contains priority and department enumerations and utility functions
- **Patient.h/cpp**: Patient class definition and implementation
- **Doctor.h/cpp**: Doctor class and doctor-specific queue implementation
- **HospitalStatistics.h/cpp**: Statistics tracking and reporting functionality
- **HospitalSystem.h/cpp**: Main system that coordinates patients, doctors, and queues
- **Utils.h/cpp**: Helper functions for menu operations
- **main.cpp**: Application entry point and menu interface

## Installation

1. Clone or download the project repository
2. Open the solution file in Visual Studio 2019 or later
3. Build the solution (Ctrl+Shift+B)
4. Run the program (F5)

## Usage Guide

### First-time Setup

1. Start by adding doctors using option 5 in the main menu
2. Add patients to the system using option 1

### Daily Operations

1. **Adding Patients**:
   - Enter patient details (name, age, gender, symptoms)
   - Choose a doctor or use 'auto' for automatic assignment
   - Select priority level and department

2. **Processing Patients**:
   - Option 2 processes the next patient from the main queue
   - Option 3 processes patients from a specific doctor's queue
   - A 5-second timer simulates patient decision making

3. **Queue Management**:
   - View current queues with option 4
   - See all queues or individual doctor queues

4. **Statistical Analysis**:
   - Option 6 displays comprehensive statistics
   - View average wait times, patient distribution, etc.

5. **Day Management**:
   - Option 7 resets all doctor daily patient counts
   - Start each day with this function

6. **Data Persistence**:
   - Option 8 saves all data to CSV files and exits
   - Three files are created:
     - treated_patients.csv
     - departed_patients.csv
     - doctors.csv

## Data Files

The system generates three CSV files:

1. **treated_patients.csv**: Records of all successfully treated patients
2. **departed_patients.csv**: Records of patients who left without treatment
3. **doctors.csv**: Information about doctors, their specializations, and workload

## Development Notes

- Uses modern C++14 features for cleaner code
- Implements priority queues for patient management
- Designed with clear separation of concerns between system components
- Modular architecture for easy maintenance and extension

## Example Workflow

1. Add doctors to various departments
2. Add patients with different priority levels
3. Process patients either from main queue or specific doctor's queue
4. Monitor statistics to identify bottlenecks
5. Save data at end of day and start fresh the next day

---

This Hospital Patient Management System provides an efficient, priority-based approach to managing patient flow in a hospital environment, helping reduce wait times while ensuring urgent cases receive prompt attention.
