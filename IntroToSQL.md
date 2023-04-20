# C++: Using SQLite

This exercise will introduce you to using SQLite, a lightweight and popular relational database management system, in C++.

## Introduction to Databases

A database is a structured collection of data that is organized and stored in a way that makes it easy to access, manage, and retrieve information. Databases are widely used in modern software applications for storing and managing data, such as user information, product details, or financial records.

There are several types of databases, including relational, hierarchical, and NoSQL databases. In this exercise, we will focus on relational databases, which organize data in tables with rows and columns, and use SQL (Structured Query Language) for managing data.

## SQLite

SQLite is a self-contained, serverless, and zero-configuration relational database engine that is embedded in many software applications. It is widely used in mobile and desktop applications due to its simplicity, small footprint, and fast performance. SQLite databases are stored as files on disk and can be easily distributed along with the application.

SQLite uses SQL for creating, modifying, and retrieving data from databases. SQL is a domain-specific language used for managing relational databases, and it provides a set of commands, or queries, for performing operations such as creating tables, inserting data, updating data, and retrieving data from tables.

SQLite is also convenient because it comes installed pre-installed on most Linux and macOS systems, and it is available as a single C source code file that can be easily embedded in other applications. SQLite is also available as a standalone command-line tool for managing databases. You can jump right into SQLite in most languages including Python, Ruby, and C/C++, without having to install any additional libraries.

### Queries, Cursors, and Results
To extract, add or modify data in a database, you need to use SQL queries. A query is a statement that you use to interact with the database. 
In the process of executing a query, the database engine will create a cursor, which is a pointer to the current row of data in the result set. The cursor will move to the next row of data in the result set after each query execution. The result set is a collection of rows and columns that is returned by the query. The result set can be empty if the query does not return any data.

## Basics of Tables and Queries

In SQLite, data is organized in tables, which are similar to spreadsheets with rows and columns. Each row represents a single record or data entry, and each column represents a specific attribute or property of the data.
In C++, you can use the SQLite C/C++ API to execute SQL queries and interact with the database.

### Creating Tables
To create a table in SQLite, you need to define its structure using the CREATE TABLE statement, which specifies the table name, column names, and their data types. For example, the following SQL query creates a table named `users` with three columns: `id`, `name`, and `age`.
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    name TEXT,
    age INTEGER
);
```

### Inserting Data
You can insert data into a table using the INSERT INTO statement, which specifies the table name, column names, and their corresponding values. For example, the following SQL query inserts a new user with id of 1, name of "John", and age of 30 into the users table.
```sql
INSERT INTO users (id, name, age)
VALUES (1, 'John', 30);
```
In C++, you can use SQLite C/C++ API functions such as sqlite3_exec or sqlite3_prepare_v2 to execute INSERT INTO statements and insert data into the database.

### Retrieving Data
You can retrieve data from a table using the SELECT statement, which specifies the column names or * to retrieve all columns, and the table name. You can also use conditions and filters to retrieve specific data based on certain criteria. For example, the following SQL query retrieves all data from the users table.
```sql
SELECT * FROM users;
```
Again, you can use SQLite C/C++ API functions such as sqlite3_exec or sqlite3_prepare_v2 to execute SELECT statements and retrieve data from the database.

### Updating Data
You can update data in a table using the UPDATE statement, which specifies the table name, column names to update, and their new values. You can also use conditions and filters to update specific data based on certain criteria. For example, the following SQL query updates the age of the user with id of 1

```sql
UPDATE users SET age = 31 WHERE id = 1;
```

## Example in C++
All the general concepts of SQLite are the same in C++, but the syntax is slightly different. In C++, you can must use the SQLite C/C++ API to execute SQL queries and interact with the database. The SQLite C/C++ API is a wrapper around the SQLite C API, and it provides a C++ interface for executing SQL queries and retrieving data from the database.
So things will look a little different. I have prepared some examples but to understand things better it is imperative that eventually you go through the official documentation: [SQLite C/C++ API](https://www.sqlite.org/capi3ref.html)