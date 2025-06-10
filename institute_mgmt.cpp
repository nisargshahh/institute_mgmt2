#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSpinBox>
#include <sqlite3.h>
#include <sstream>
#include <string>

class StudentManagementSystem : public QMainWindow
{
    Q_OBJECT

private:
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    
    // Input form
    QGroupBox *inputGroup;
    QGridLayout *inputLayout;
    QSpinBox *idInput;
    QLineEdit *nameInput;
    QLineEdit *majorInput;
    QDoubleSpinBox *gpaInput;
    
    // Buttons
    QPushButton *addBtn;
    QPushButton *updateBtn;
    QPushButton *deleteBtn;
    QPushButton *refreshBtn;
    QPushButton *clearBtn;
    
    // Table
    QTableWidget *studentTable;
    
    // Database
    sqlite3 *db;
    const char *DB_NAME = "institute.db";

public:
    StudentManagementSystem(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
        setupDatabase();
        connectSignals();
        loadStudents();
    }

    ~StudentManagementSystem()
    {
        if (db) {
            sqlite3_close(db);
        }
    }

private slots:
    void addStudent()
    {
        if (validateInput()) {
            std::stringstream query;
            query << "INSERT INTO students (ID, Name, Major, GPA) VALUES (" 
                  << idInput->value() << ", '" 
                  << nameInput->text().toStdString() << "', '" 
                  << majorInput->text().toStdString() << "', " 
                  << gpaInput->value() << ")";
            
            if (executeQuery(query.str())) {
                QMessageBox::information(this, "Success", "Student added successfully!");
                clearInputs();
                loadStudents();
            }
        }
    }

    void updateStudent()
    {
        int currentRow = studentTable->currentRow();
        if (currentRow < 0) {
            QMessageBox::warning(this, "Warning", "Please select a student to update!");
            return;
        }

        if (validateInput()) {
            std::stringstream query;
            query << "UPDATE students SET Name='" << nameInput->text().toStdString() 
                  << "', Major='" << majorInput->text().toStdString() 
                  << "', GPA=" << gpaInput->value() 
                  << " WHERE ID=" << idInput->value();
            
            if (executeQuery(query.str())) {
                QMessageBox::information(this, "Success", "Student updated successfully!");
                clearInputs();
                loadStudents();
            }
        }
    }

    void deleteStudent()
    {
        int currentRow = studentTable->currentRow();
        if (currentRow < 0) {
            QMessageBox::warning(this, "Warning", "Please select a student to delete!");
            return;
        }

        int id = studentTable->item(currentRow, 0)->text().toInt();
        
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirm Delete", 
            "Are you sure you want to delete this student?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            std::stringstream query;
            query << "DELETE FROM students WHERE ID=" << id;
            
            if (executeQuery(query.str())) {
                QMessageBox::information(this, "Success", "Student deleted successfully!");
                clearInputs();
                loadStudents();
            }
        }
    }

    void loadStudents()
    {
        studentTable->setRowCount(0);
        
        sqlite3_stmt *stmt;
        std::string query = "SELECT * FROM students ORDER BY ID";
        
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            int row = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                studentTable->insertRow(row);
                
                studentTable->setItem(row, 0, new QTableWidgetItem(
                    QString::number(sqlite3_column_int(stmt, 0))));
                studentTable->setItem(row, 1, new QTableWidgetItem(
                    QString::fromUtf8((char*)sqlite3_column_text(stmt, 1))));
                studentTable->setItem(row, 2, new QTableWidgetItem(
                    QString::fromUtf8((char*)sqlite3_column_text(stmt, 2))));
                studentTable->setItem(row, 3, new QTableWidgetItem(
                    QString::number(sqlite3_column_double(stmt, 3), 'f', 2)));
                
                row++;
            }
            sqlite3_finalize(stmt);
        }
    }

    void onTableItemClicked()
    {
        int currentRow = studentTable->currentRow();
        if (currentRow >= 0) {
            idInput->setValue(studentTable->item(currentRow, 0)->text().toInt());
            nameInput->setText(studentTable->item(currentRow, 1)->text());
            majorInput->setText(studentTable->item(currentRow, 2)->text());
            gpaInput->setValue(studentTable->item(currentRow, 3)->text().toDouble());
        }
    }

    void clearInputs()
    {
        idInput->setValue(0);
        nameInput->clear();
        majorInput->clear();
        gpaInput->setValue(0.0);
        studentTable->clearSelection();
    }

private:
    void setupUI()
    {
        setWindowTitle("Student Management System");
        setMinimumSize(800, 600);
        
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        mainLayout = new QVBoxLayout(centralWidget);
        topLayout = new QHBoxLayout();
        mainLayout->addLayout(topLayout);
        
        // Left side - Input form
        leftLayout = new QVBoxLayout();
        setupInputForm();
        leftLayout->addWidget(inputGroup);
        leftLayout->addStretch();
        
        // Right side - Table
        rightLayout = new QVBoxLayout();
        setupTable();
        rightLayout->addWidget(studentTable);
        
        topLayout->addLayout(leftLayout, 1);
        topLayout->addLayout(rightLayout, 2);
    }

    void setupInputForm()
    {
        inputGroup = new QGroupBox("Student Information");
        inputLayout = new QGridLayout(inputGroup);
        
        // ID input
        inputLayout->addWidget(new QLabel("Student ID:"), 0, 0);
        idInput = new QSpinBox();
        idInput->setRange(1, 999999);
        inputLayout->addWidget(idInput, 0, 1);
        
        // Name input
        inputLayout->addWidget(new QLabel("Full Name:"), 1, 0);
        nameInput = new QLineEdit();
        inputLayout->addWidget(nameInput, 1, 1);
        
        // Major input
        inputLayout->addWidget(new QLabel("Major:"), 2, 0);
        majorInput = new QLineEdit();
        inputLayout->addWidget(majorInput, 2, 1);
        
        // GPA input
        inputLayout->addWidget(new QLabel("GPA:"), 3, 0);
        gpaInput = new QDoubleSpinBox();
        gpaInput->setRange(0.0, 10.0);
        gpaInput->setDecimals(2);
        gpaInput->setSingleStep(0.1);
        inputLayout->addWidget(gpaInput, 3, 1);
        
        // Buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        addBtn = new QPushButton("Add Student");
        updateBtn = new QPushButton("Update Student");
        deleteBtn = new QPushButton("Delete Student");
        clearBtn = new QPushButton("Clear");
        refreshBtn = new QPushButton("Refresh");
        
        addBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
        updateBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");
        deleteBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; }");
        clearBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; }");
        refreshBtn->setStyleSheet("QPushButton { background-color: #9C27B0; color: white; font-weight: bold; }");
        
        buttonLayout->addWidget(addBtn);
        buttonLayout->addWidget(updateBtn);
        buttonLayout->addWidget(deleteBtn);
        buttonLayout->addWidget(clearBtn);
        buttonLayout->addWidget(refreshBtn);
        
        inputLayout->addLayout(buttonLayout, 4, 0, 1, 2);
    }

    void setupTable()
    {
        studentTable = new QTableWidget();
        studentTable->setColumnCount(4);
        
        QStringList headers;
        headers << "ID" << "Name" << "Major" << "GPA";
        studentTable->setHorizontalHeaderLabels(headers);
        
        studentTable->horizontalHeader()->setStretchLastSection(true);
        studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        studentTable->setAlternatingRowColors(true);
        studentTable->setSortingEnabled(true);
    }

    void connectSignals()
    {
        connect(addBtn, &QPushButton::clicked, this, &StudentManagementSystem::addStudent);
        connect(updateBtn, &QPushButton::clicked, this, &StudentManagementSystem::updateStudent);
        connect(deleteBtn, &QPushButton::clicked, this, &StudentManagementSystem::deleteStudent);
        connect(clearBtn, &QPushButton::clicked, this, &StudentManagementSystem::clearInputs);
        connect(refreshBtn, &QPushButton::clicked, this, &StudentManagementSystem::loadStudents);
        connect(studentTable, &QTableWidget::itemClicked, this, &StudentManagementSystem::onTableItemClicked);
    }

    void setupDatabase()
    {
        if (sqlite3_open(DB_NAME, &db)) {
            QMessageBox::critical(this, "Database Error", "Failed to connect to database!");
            return;
        }
        
        std::string query = "CREATE TABLE IF NOT EXISTS students (ID INTEGER PRIMARY KEY, Name TEXT, Major TEXT, GPA REAL)";
        executeQuery(query);
    }

    bool executeQuery(const std::string &query)
    {
        char *errMsg = nullptr;
        if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            QMessageBox::critical(this, "Database Error", QString("Error: %1").arg(errMsg));
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }

    bool validateInput()
    {
        if (nameInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Please enter student name!");
            return false;
        }
        
        if (majorInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Please enter major!");
            return false;
        }
        
        if (idInput->value() <= 0) {
            QMessageBox::warning(this, "Validation Error", "Please enter a valid student ID!");
            return false;
        }
        
        return true;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    StudentManagementSystem window;
    window.show();
    
    return app.exec();
}

#include "institute_mgmt.moc"