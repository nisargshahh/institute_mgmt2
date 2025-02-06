#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

const char *DB_NAME = "institute.db";

class Student
{
private:
	int studentID;
	string fullName, major;
	float gpa;

public:
	Student() : studentID(0), fullName(""), major(""), gpa(0.0) {}

	void setStudentID(int id) { studentID = id; }
	void setFullName(string name) { fullName = name; }
	void setMajor(string subject) { major = subject; }
	void setGPA(float grade) { gpa = grade; }

	int getStudentID() { return studentID; }
	string getFullName() { return fullName; }
	string getMajor() { return major; }
	float getGPA() { return gpa; }
};

void executeQuery(sqlite3 *db, const string &query)
{
	char *errMsg = nullptr;
	if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
	{
		cout << RED << "Error: " << errMsg << RESET << endl;
		sqlite3_free(errMsg);
	}
}

void insertStudent(sqlite3 *db)
{
	Student s;
	int id;
	string name, major;
	float gpa;

	cout << GREEN << "Let's add a new student!" << RESET << endl;
	cout << YELLOW << "Enter Student ID: " << RESET;
	cin >> id;
	s.setStudentID(id);
	cin.ignore();

	cout << YELLOW << "Enter Student's Full Name: " << RESET;
	getline(cin, name);
	s.setFullName(name);

	cout << YELLOW << "Enter Major: " << RESET;
	getline(cin, major);
	s.setMajor(major);

	do
	{
		cout << YELLOW << "Enter GPA (0 - 10): " << RESET;
		cin >> gpa;
	} while (gpa < 0 || gpa > 10);
	s.setGPA(gpa);

	stringstream query;
	query << "INSERT INTO students (ID, Name, Major, GPA) VALUES (" << s.getStudentID() << ", '" << s.getFullName() << "', '" << s.getMajor() << "', " << s.getGPA() << ")";
	executeQuery(db, query.str());
	cout << GREEN << "Student added successfully!" << RESET << endl;
}

void displayStudents(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	string query = "SELECT * FROM students";
	if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		cout << CYAN << "\nID | Name | Major | GPA" << RESET << endl;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			cout << sqlite3_column_int(stmt, 0) << " | "
				 << sqlite3_column_text(stmt, 1) << " | "
				 << sqlite3_column_text(stmt, 2) << " | "
				 << sqlite3_column_double(stmt, 3) << endl;
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		cout << RED << "Error retrieving students!" << RESET << endl;
	}
}

void updateGPA(sqlite3 *db)
{
	int id;
	float gpa;
	cout << YELLOW << "Enter Student ID: " << RESET;
	cin >> id;
	do
	{
		cout << YELLOW << "Enter new GPA (0 - 10): " << RESET;
		cin >> gpa;
	} while (gpa < 0 || gpa > 10);

	stringstream query;
	query << "UPDATE students SET GPA=" << gpa << " WHERE ID=" << id;
	executeQuery(db, query.str());
	cout << GREEN << "GPA Updated Successfully!" << RESET << endl;
}

void deleteStudent(sqlite3 *db)
{
	int id;
	cout << YELLOW << "Enter Student ID to delete: " << RESET;
	cin >> id;
	stringstream query;
	query << "DELETE FROM students WHERE ID=" << id;
	executeQuery(db, query.str());
	cout << GREEN << "Student record deleted." << RESET << endl;
}

void setupDatabase(sqlite3 *db)
{
	string query = "CREATE TABLE IF NOT EXISTS students (ID INTEGER PRIMARY KEY, Name TEXT, Major TEXT, GPA REAL)";
	executeQuery(db, query);
}

int main()
{
	sqlite3 *db;
	if (sqlite3_open(DB_NAME, &db))
	{
		cout << RED << "Failed to connect to database!" << RESET << endl;
		return 1;
	}
	cout << GREEN << "Connected to database successfully!" << RESET << endl;

	setupDatabase(db);

	bool running = true;
	while (running)
	{
		cout << "\n"
			 << BLUE << "Institute Management System" << RESET << endl;
		cout << "1. Add Student\n2. View Students\n3. Update GPA\n4. Delete Student\n0. Exit\nChoice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			insertStudent(db);
			break;
		case 2:
			displayStudents(db);
			break;
		case 3:
			updateGPA(db);
			break;
		case 4:
			deleteStudent(db);
			break;
		case 0:
			running = false;
			cout << GREEN << "Goodbye!" << RESET << endl;
			break;
		default:
			cout << RED << "Invalid choice, try again!" << RESET << endl;
		}
	}
	sqlite3_close(db);
	return 0;
}