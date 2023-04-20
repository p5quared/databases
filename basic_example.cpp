//
// Created by Peter Vaiciulis on 4/11/23.
//
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sqlite3.h>

/*  STOP: READ ME!!!!!!!!
 *
 * I'll admit the default SQLite API is not super beginner-friendly.
 * There are other libraries that wrap the SQLite API and make it easier to use, for example: https://github.com/SRombauts/SQLiteCpp.
 * That being said, if you don't know how to install libraries in C++, you can just use the default SQLite API.
 *
 * This is far from a complete tutorial for working with databases (that is beyond the scope of a single lesson).
 * This example is meant to be used as a reference but does not include things like:
 * - Error handling
 * - Prepared statements
 * - Transactions
 * - Proper Input Sanitation/Validation
 * - etc...
 *
 * You will also see the use of callback functions.
 * If you have done web-development stuff in the past, you may be familiar with callback functions. Call back functions
 * are functions that are passed as parameters to other functions, and invoked inside.
 *
 * A common example is when working with buttons. If you have a button that is supposed to do something
 * 'onClick', you'd typically pass a callback function to the button that the button will store when it is created.
 * Then, when the button is clicked, it will invoke the callback function and do whatever you want.
 *
 * In this example, we are passing the callback function to the sqlite3_exec function. The sqlite3_exec function will
 * call the callback function for each row in the result of the query. For a better idea of how this works, Google
 * "what is an sql cursor?" and "sqlite3_exec callback function".
 *
 * */



// Callback function for executing SQLite queries
static int callback(void *NotUsed, int argc, char* argv[], char* azColName[]) {
    // 'data' parameter can be used to pass any additional data to the callback function
    // 'argc' parameter gives the number of columns in the query result
    // 'argv' parameter is an array of strings that contains the values in the current row
    // 'azColName' parameter is an array of strings that contains the column names

    int i;
    std::cout << "-----------------------------------------" << std::endl;
    for (i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
    return 0;
}

// We can design our own callback functions to do whatever we want with the query results!
static int displayGPA(void *NotUsed, int argc, char* argv[], char* azColName[]);
static int displayStudentsWithGPA(void *NotUsed, int argc, char* argv[], char* azColName[]);

// Global vector to store random names
static std::vector<std::string> random_names{
    "Peter", "John", "Jacob", "Jingleheimer", "Schmidt", "Mary", "Jane", "Sally", "Sue", "Bob", "Billy", "Joe", "Jim",
    "Samantha", "Sara", "Sarah", "Suzie", "Suzanne", "Suzette", "Greenborough", "Forest", "Hill", "Hank", "Hans", "Hansel",
    "Gretel", "Inigo", "Montoya", "Bingley", "Darcy", "Lizzy"};


int main() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    // Open or create the database
    rc = sqlite3_open("students.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return(0);
    }
    else {
        std::cout << "Opened database successfully!" << std::endl;
    }

    // Create the 'students' table if it doesn't exist
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS students (" \
        "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "NAME TEXT NOT NULL," \
        "GRADES TEXT NOT NULL);";
    rc = sqlite3_exec(db, createTableSQL, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Table created successfully!" << std::endl;
    }

    // Interactivity
    while (true) {
        // Display menu options
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1. Add Student" << std::endl;
        std::cout << "2. Remove Student" << std::endl;
        std::cout << "3. Add Grades" << std::endl;
        std::cout << "4. Calculate GPA" << std::endl;
        std::cout << "5. Display Students" << std::endl;
        std::cout << "6. Display Students with GPA" << std::endl;
        std::cout << "9. Generate Random Students" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Add Student
                std::string name;
                std::cout << "Enter student name: ";
                std::cin.ignore();
                std::getline(std::cin, name);

                // Insert student into 'students' table
                std::string insertSQL = "INSERT INTO students (NAME, GRADES) VALUES ('" + name + "', '');";
                rc = sqlite3_exec(db, insertSQL.c_str(), callback, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Student added successfully!" << std::endl;
                }
                break;
            }
            case 2: {
                // Remove Student
                int id;
                std::cout << "Enter student ID to remove: ";
                std::cin >> id;

                // Delete student from 'students' table
                std::string deleteSQL = "DELETE FROM students WHERE ID = " + std::to_string(id) + ";";
                rc = sqlite3_exec(db, deleteSQL.c_str(), callback, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Student removed successfully!" << std::endl;
                }
                break;
            }
            case 3: {
                // Add Grades
                int id;
                std::cout << "Enter student ID to add grades: ";
                std::cin >> id;

                std::cin.ignore();
                std::string grades;
                std::cout << "Enter grades separated by commas: ";
                std::getline(std::cin, grades);

                // Update grades for student in 'students' table
                std::string updateSQL = "UPDATE students SET GRADES = '" + grades + "' WHERE ID = " + std::to_string(id) + ";";
                rc = sqlite3_exec(db, updateSQL.c_str(), callback, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Grades added successfully!" << std::endl;
                }
                break;
            }
            case 4: {
                // Calculate GPA
                int id;
                std::cout << "Enter student ID to calculate GPA: ";
                std::cin >> id;

                // Fetch grades for student from 'students' table
                std::string selectSQL = "SELECT GRADES FROM students WHERE ID = " + std::to_string(id) + ";";
                rc = sqlite3_exec(db, selectSQL.c_str(), displayGPA, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                break;
            }
            case 5: {
                // Display all students
                std::string selectSQL = "SELECT * FROM students;";
                rc = sqlite3_exec(db, selectSQL.c_str(), callback, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Students displayed successfully!" << std::endl;
                }
                break;
            }
            case 6: {
                // Display students with GPA
                std::string selectSQL = "SELECT * FROM students;";
                rc = sqlite3_exec(db, selectSQL.c_str(), displayStudentsWithGPA, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Students displayed successfully!" << std::endl;
                }
                break;
            }
            case 9: {
                // Generate a random student
                int num_students;
                std::cout << "How many students to generate? ";
                std::cin >> num_students;

               for(int z = 0; z < num_students; z++) {
                   std::string name = random_names[rand() % random_names.size()];
                   std::string grades;
                   int num_grades = 10;
                   for (int i = 0; i < num_grades; i++) {
                       grades += std::to_string(rand() % 100);
                       if (i != num_grades - 1) {
                           grades += ",";
                       }
                   }
                   std::string insertSQL = "INSERT INTO students (NAME, GRADES) VALUES ('" + name + "', '" + grades + "');";
                   rc = sqlite3_exec(db, insertSQL.c_str(), callback, 0, &errMsg);
                   if (rc != SQLITE_OK) {
                       std::cerr << "SQL error: " << errMsg << std::endl;
                       sqlite3_free(errMsg);
                   }
                   else {
                       std::cout << "Student "<< z << " added successfully!" << std::endl;
                   }
               }
                break;
            }
            case 666: {
                // Drop Table/Reset
                std::string dropTableSQL = "DROP TABLE students;";
                rc = sqlite3_exec(db, dropTableSQL.c_str(), callback, nullptr, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }
                else {
                    std::cout << "Table dropped successfully!" << std::endl;
                }
                break;
            }
            case 0: {
                // Exit
                std::cout << "Exiting..." << std::endl;
                sqlite3_close(db);
                return 0;
            }
            default: {
                std::cerr << "Invalid choice! Please try again." << std::endl;
                break;
            }
        }
    }
}


// Display a single student's GPA
static int displayGPA(void *NotUsed, int argc, char* argv[], char* azColName[]) {
    // Note that we are assuming that the grades are stored as a comma-separated list of integers, because
    // that's how we defined grades to be stored.
    int sum = 0;
    int num_grades = 0;
    std::string grades_string(argv[0]);
    std::istringstream grades_stream(grades_string);
    std::string grade;
    while(std::getline(grades_stream, grade, ','))
    {
        sum += std::stoi(grade);
        num_grades++;
    }

    std::cout << "-----------------------------------------" << std::endl;
    std::cout << azColName[0] << "= " << sum / num_grades << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    return 0;
}

// Display all students with their GPA
static int displayStudentsWithGPA(void *NotUsed, int argc, char* argv[], char* azColName[]){
    // As you can see we can do basically the same thing as before to calculate the GPA, but we can also
    // display the student's name and ID.
    int sum = 0;
    int num_grades = 0;
    std::string grades_string(argv[2]);
    std::istringstream grades_stream(grades_string);
    std::string grade;
    while(std::getline(grades_stream, grade, ','))
    {
        sum += std::stoi(grade);
        num_grades++;
    }

    std::cout << "-----------------------------------------" << std::endl;
    for(int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << argv[i] << std::endl;
    }
    std::cout << "STUDENT'S GPA = " << sum / num_grades << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    return 0;
}
