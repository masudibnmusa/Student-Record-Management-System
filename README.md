# Student Record Management System

A console-based application written in C for efficiently managing student records using linked lists and file operations.

## 🚀 Features

- **📝 Add Students** - Create new student records with validation
- **👀 View All Students** - Display all records in formatted table
- **🔍 Search Students** - Find by roll number or name
- **✏️ Update Records** - Modify existing student information
- **🗑️ Delete Students** - Remove records by roll number
- **💾 Data Persistence** - Automatic save/load from binary file
- **🎯 Input Validation** - Robust error handling and validation

## 🛠️ Technical Stack

- **Programming Language**: C
- **Data Structure**: Singly Linked List
- **Memory Management**: Dynamic Allocation (malloc/free)
- **File Handling**: Binary I/O Operations
- **Storage**: Persistent data in `students.dat`

## 📁 Project Structure
## 🏗️ Data Structure

```c
typedef struct Student {
    int rollNo;
    char name[50];
    float marks;
    struct Student* next;
} Student;
