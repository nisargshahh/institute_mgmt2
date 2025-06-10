# Student Management System

A desktop application for managing student records built with Qt and SQLite. This application provides a complete CRUD (Create, Read, Update, Delete) interface for managing student information including ID, name, major, and GPA.

## Features

- **Add Students**: Insert new student records with validation
- **Update Students**: Modify existing student information
- **Delete Students**: Remove student records with confirmation dialog
- **View Students**: Display all students in a sortable table
- **Data Persistence**: SQLite database for reliable data storage
- **User-friendly Interface**: Clean Qt-based GUI with form validation


## System Requirements

- Ubuntu 18.04 or later
- Qt6 development libraries
- SQLite3
- C++17 compatible compiler
- CMake 3.16 or later

## Installation and Setup

### Step 1: Install Dependencies

For **Qt6**:
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools libsqlite3-dev build-essential cmake
```

### Step 2: Clone or Download

```bash
# Create project directory
mkdir institute_mgmt
cd institute_mgmt

# Copy the source files:
# - institute_mgmt.cpp (main source code)
# - CMakeLists.txt (build configuration)
```

### Step 3: Build the Application

```bash
mkdir build
cd build
cmake ..
make
./institute_mgmt
```

## Project Structure

```
institute_mgmt/
├── institute_mgmt.cpp    # Main source code
├── CMakeLists.txt        # Build configuration
├── README.md            # This file
└── build/               # Build directory (created during compilation)
    └── institute_mgmt   # Compiled executable
```

## Usage

### Adding a Student
1. Fill in the student information in the left panel:
   - **Student ID**: Unique identifier (1-999999)
   - **Full Name**: Student's complete name
   - **Major**: Field of study
   - **GPA**: Grade point average (0.0-10.0)
2. Click "Add Student" to save the record

### Updating a Student
1. Click on a student row in the table to select
2. The form will auto-populate with the student's data
3. Modify the information as needed
4. Click "Update Student" to save changes

### Deleting a Student
1. Select a student from the table
2. Click "Delete Student"
3. Confirm the deletion in the dialog box

### Other Operations
- **Clear**: Reset all form fields
- **Refresh**: Reload data from database
- **Sort**: Click column headers to sort the table

## Database

The application creates an SQLite database file named `institute.db` in the application directory. The database schema includes:

```sql
CREATE TABLE students (
    ID INTEGER PRIMARY KEY,
    Name TEXT,
    Major TEXT,
    GPA REAL
);
```

## Screenshots

![Screenshot from 2025-06-11 00-24-40](https://github.com/user-attachments/assets/5b8e7581-2c85-4322-b364-0b8e02e01347)
![Screenshot from 2025-06-11 00-25-24](https://github.com/user-attachments/assets/387bd779-03e5-4e6e-9721-a245fb971aa5)
![Screenshot from 2025-06-11 00-25-37](https://github.com/user-attachments/assets/e0f6be11-23f8-4f30-85a3-f035cea397b8)
![Screenshot from 2025-06-11 00-25-43](https://github.com/user-attachments/assets/0398c089-f9db-4400-9394-93e80f198d83)
![Screenshot from 2025-06-11 00-25-50](https://github.com/user-attachments/assets/64636438-98ce-42a6-8c3e-966cb7a8299f)
![Screenshot from 2025-06-11 00-25-54](https://github.com/user-attachments/assets/d4e3daea-251b-43ff-831a-43ba20a51935)


## Technical Details

- **Framework**: Qt6 for GUI
- **Database**: SQLite3 for data persistence
- **Language**: C++17
- **Build System**: CMake
- **Architecture**: Single-file application with embedded MOC

## License

This project is open source. Feel free to use, modify, and distribute as needed.
