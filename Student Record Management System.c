#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define MAX_PHONE_LENGTH 20
#define MAX_DEPT_LENGTH 50
#define FILENAME "students.dat"
#define VERSION "2.0"

// ANSI Color Codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

// Text Colors
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Bright Colors
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// Background Colors
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

// Student structure
typedef struct Student {
    int rollNo;
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char department[MAX_DEPT_LENGTH];
    float marks;
    char grade[3];  // A+, A, B+, etc.
    struct Student* next;
} Student;

// Global head pointer for linked list
Student* head = NULL;

// Function prototypes
void displayMenu();
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void saveToFile();
void loadFromFile();
void freeList();
int isRollNoUnique(int rollNo);
Student* createStudent(int rollNo, const char* name, const char* email,
                       const char* phone, const char* department, float marks);
void calculateGrade(Student* student);
void clearInputBuffer();
void clearScreen();
void pauseScreen();
void displayHeader(const char* title, const char* color);
void displayFooter();
void printLine(char ch, int length, const char* color);
void displayWelcome();
void displayExitMessage();
int getValidChoice(int min, int max);
int getValidRollNo();
float getValidMarks();
char* toLowerStr(char* str);
int caseInsensitiveSearch(const char* haystack, const char* needle);
int isValidEmail(const char* email);
int isValidPhone(const char* phone);

int main() {
    clearScreen();
    displayWelcome();

    // Load existing data from file
    loadFromFile();

    pauseScreen();

    int choice;
    do {
        clearScreen();
        displayMenu();
        choice = getValidChoice(1, 7);

        switch(choice) {
            case 1:
                clearScreen();
                addStudent();
                pauseScreen();
                break;
            case 2:
                clearScreen();
                displayAllStudents();
                pauseScreen();
                break;
            case 3:
                clearScreen();
                searchStudent();
                pauseScreen();
                break;
            case 4:
                clearScreen();
                updateStudent();
                pauseScreen();
                break;
            case 5:
                clearScreen();
                deleteStudent();
                pauseScreen();
                break;
            case 6:
                clearScreen();
                saveToFile();
                pauseScreen();
                break;
            case 7:
                clearScreen();
                displayExitMessage();
                break;
        }
    } while(choice != 7);

    // Save data and free memory before exiting
    saveToFile();
    freeList();

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    printf("\n");
    printLine('=', 80, BRIGHT_BLACK);
    printf("%s%sPress Enter to continue...%s", BOLD, YELLOW, RESET);
    getchar();
}

void printLine(char ch, int length, const char* color) {
    printf("%s", color);
    int i;
    for(i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("%s\n", RESET);
}

void displayHeader(const char* title, const char* color) {
    printLine('=', 80, color);
    printf("%s%s   %s%s\n", BOLD, color, title, RESET);
    printLine('=', 80, color);
}

void displayFooter() {
    printLine('=', 80, BRIGHT_BLACK);
}

void displayWelcome() {
    printLine('=', 120, BRIGHT_CYAN);
    printf("\n");
    printf("%s%s%44s%s%44s%s\n", BOLD, BRIGHT_MAGENTA, "", "STUDENT RECORD MANAGEMENT SYSTEM", "", RESET);
    printf("%s%s%56s%s%56s%s\n", BOLD, CYAN, "", "Version 4.1", "", RESET);
    printf("\n");
    printLine('=', 120, BRIGHT_CYAN);
    printf("\n");

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    printf("%s%s  System Date: %s%02d-%02d-%d%s\n", BOLD, YELLOW, BRIGHT_YELLOW, tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900, RESET);
    printf("%s%s  System Time: %s%02d:%02d:%02d%s\n", BOLD, YELLOW, BRIGHT_YELLOW, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, RESET);
    printf("\n");
}

void displayExitMessage() {
    displayHeader("SYSTEM EXIT", BRIGHT_RED);
    printf("\n");
    printf("%s%s  Thank you for using Student Record Management System!%s\n", BOLD, GREEN, RESET);
    printf("%s  All changes have been saved successfully.%s\n", BRIGHT_GREEN, RESET);
    printf("\n");
    printf("%s  Developed by: Masud%s\n", CYAN, RESET);
    printf("%s  Version 4.1%s\n", CYAN, RESET);
    printf("\n");
    displayFooter();
}

void displayMenu() {
    displayHeader("                      MAIN MENU", BRIGHT_CYAN);
    printf("\n");
    printf("%s%s  [1]%s %sAdd New Student Record%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [2]%s %sDisplay All Student Records%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [3]%s %sSearch Student Record%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [4]%s %sUpdate Student Record%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [5]%s %sDelete Student Record%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [6]%s %sSave Records to File%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("%s%s  [7]%s %sExit System%s\n", BOLD, YELLOW, RESET, BRIGHT_WHITE, RESET);
    printf("\n");
    displayFooter();
}

int getValidChoice(int min, int max) {
    int choice;
    while(1) {
        printf("\n%s%s  Enter your choice [%d-%d]: %s", BOLD, CYAN, min, max, RESET);

        if (scanf("%d", &choice) != 1) {
            printf("%s%s  [ERROR]%s Invalid input! Please enter a number.%s\n", BOLD, RED, RESET, RESET);
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (choice >= min && choice <= max) {
            return choice;
        } else {
            printf("%s%s  [ERROR]%s Invalid choice! Please select between %d and %d.%s\n", BOLD, RED, RESET, min, max, RESET);
        }
    }
}

int getValidRollNo() {
    int rollNo;
    while(1) {
        printf("%s  Enter Roll Number: %s", CYAN, RESET);
        if (scanf("%d", &rollNo) != 1 || rollNo <= 0) {
            printf("%s%s  [ERROR]%s Invalid roll number! Must be a positive integer.%s\n", BOLD, RED, RESET, RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        return rollNo;
    }
}

float getValidMarks() {
    float marks;
    while(1) {
        printf("%s  Enter Marks (0-100): %s", CYAN, RESET);
        if (scanf("%f", &marks) != 1) {
            printf("%s%s  [ERROR]%s Invalid marks! Please enter a number.%s\n", BOLD, RED, RESET, RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (marks < 0 || marks > 100) {
            printf("%s%s  [ERROR]%s Marks must be between 0 and 100!%s\n", BOLD, RED, RESET, RESET);
            continue;
        }
        return marks;
    }
}

int isValidEmail(const char* email) {
    int atCount = 0;
    int dotAfterAt = 0;
    int i;
    int len = strlen(email);

    if (len < 5) return 0;  // Minimum email: a@b.c

    for(i = 0; i < len; i++) {
        if (email[i] == '@') {
            atCount++;
            if (i == 0 || i == len - 1) return 0;
        }
        if (atCount == 1 && email[i] == '.' && i > 0) {
            dotAfterAt = 1;
        }
    }

    return (atCount == 1 && dotAfterAt);
}

int isValidPhone(const char* phone) {
    int i;
    int len = strlen(phone);
    int digitCount = 0;

    if (len < 10 || len > 15) return 0;

    for(i = 0; i < len; i++) {
        if (isdigit(phone[i])) {
            digitCount++;
        } else if (phone[i] != '-' && phone[i] != ' ' && phone[i] != '+' && phone[i] != '(' && phone[i] != ')') {
            return 0;
        }
    }

    return (digitCount >= 10);
}

char* toLowerStr(char* str) {
    int i;
    for(i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

int caseInsensitiveSearch(const char* haystack, const char* needle) {
    char haystackLower[MAX_NAME_LENGTH];
    char needleLower[MAX_NAME_LENGTH];

    strncpy(haystackLower, haystack, MAX_NAME_LENGTH - 1);
    haystackLower[MAX_NAME_LENGTH - 1] = '\0';

    strncpy(needleLower, needle, MAX_NAME_LENGTH - 1);
    needleLower[MAX_NAME_LENGTH - 1] = '\0';

    toLowerStr(haystackLower);
    toLowerStr(needleLower);

    return strstr(haystackLower, needleLower) != NULL;
}

void calculateGrade(Student* student) {
    float marks = student->marks;

    if (marks >= 90) {
        strcpy(student->grade, "A+");
    } else if (marks >= 85) {
        strcpy(student->grade, "A");
    } else if (marks >= 80) {
        strcpy(student->grade, "A-");
    } else if (marks >= 75) {
        strcpy(student->grade, "B+");
    } else if (marks >= 70) {
        strcpy(student->grade, "B");
    } else if (marks >= 65) {
        strcpy(student->grade, "B-");
    } else if (marks >= 60) {
        strcpy(student->grade, "C+");
    } else if (marks >= 55) {
        strcpy(student->grade, "C");
    } else if (marks >= 50) {
        strcpy(student->grade, "C-");
    } else if (marks >= 45) {
        strcpy(student->grade, "D");
    } else {
        strcpy(student->grade, "F");
    }
}

Student* createStudent(int rollNo, const char* name, const char* email,
                       const char* phone, const char* department, float marks) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("%s%s  [ERROR]%s Memory allocation failed!%s\n", BOLD, RED, RESET, RESET);
        return NULL;
    }

    newStudent->rollNo = rollNo;
    strncpy(newStudent->name, name, MAX_NAME_LENGTH - 1);
    newStudent->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(newStudent->email, email, MAX_EMAIL_LENGTH - 1);
    newStudent->email[MAX_EMAIL_LENGTH - 1] = '\0';
    strncpy(newStudent->phone, phone, MAX_PHONE_LENGTH - 1);
    newStudent->phone[MAX_PHONE_LENGTH - 1] = '\0';
    strncpy(newStudent->department, department, MAX_DEPT_LENGTH - 1);
    newStudent->department[MAX_DEPT_LENGTH - 1] = '\0';
    newStudent->marks = marks;
    calculateGrade(newStudent);
    newStudent->next = NULL;

    return newStudent;
}

void addStudent() {
    int rollNo;
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char department[MAX_DEPT_LENGTH];
    float marks;

    displayHeader("     ADD NEW STUDENT RECORD", BRIGHT_GREEN);
    printf("\n");

    // Get roll number
    rollNo = getValidRollNo();

    // Check if roll number is unique
    if (!isRollNoUnique(rollNo)) {
        printf("%s%s  [ERROR]%s Roll number %d already exists!%s\n", BOLD, RED, RESET, rollNo, RESET);
        return;
    }

    // Get name
    printf("%s  Enter Student Name: %s", CYAN, RESET);
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
        printf("%s%s  [ERROR]%s Error reading name!%s\n", BOLD, RED, RESET, RESET);
        return;
    }
    name[strcspn(name, "\n")] = 0;
    if (strlen(name) == 0) {
        printf("%s%s  [ERROR]%s Name cannot be empty!%s\n", BOLD, RED, RESET, RESET);
        return;
    }

    // Get email
    while(1) {
        printf("%s  Enter Email: %s", CYAN, RESET);
        if (fgets(email, MAX_EMAIL_LENGTH, stdin) == NULL) {
            printf("%s%s  [ERROR]%s Error reading email!%s\n", BOLD, RED, RESET, RESET);
            return;
        }
        email[strcspn(email, "\n")] = 0;

        if (isValidEmail(email)) {
            break;
        } else {
            printf("%s%s  [ERROR]%s Invalid email format! Please try again.%s\n", BOLD, RED, RESET, RESET);
        }
    }

    // Get phone
    while(1) {
        printf("%s  Enter Phone Number: %s", CYAN, RESET);
        if (fgets(phone, MAX_PHONE_LENGTH, stdin) == NULL) {
            printf("%s%s  [ERROR]%s Error reading phone!%s\n", BOLD, RED, RESET, RESET);
            return;
        }
        phone[strcspn(phone, "\n")] = 0;

        if (isValidPhone(phone)) {
            break;
        } else {
            printf("%s%s  [ERROR]%s Invalid phone number! Must be 10-15 digits.%s\n", BOLD, RED, RESET, RESET);
        }
    }

    // Get department
    printf("%s  Enter Department/Class: %s", CYAN, RESET);
    if (fgets(department, MAX_DEPT_LENGTH, stdin) == NULL) {
        printf("%s%s  [ERROR]%s Error reading department!%s\n", BOLD, RED, RESET, RESET);
        return;
    }
    department[strcspn(department, "\n")] = 0;
    if (strlen(department) == 0) {
        printf("%s%s  [ERROR]%s Department cannot be empty!%s\n", BOLD, RED, RESET, RESET);
        return;
    }

    // Get marks
    marks = getValidMarks();

    // Create new student
    Student* newStudent = createStudent(rollNo, name, email, phone, department, marks);
    if (newStudent == NULL) {
        return;
    }

    // Add to linked list
    if (head == NULL) {
        head = newStudent;
    } else {
        newStudent->next = head;
        head = newStudent;
    }

    printf("\n");
    printf("%s%s  [SUCCESS]%s Student record added successfully!%s\n", BOLD, GREEN, RESET, RESET);
    printf("%s  Assigned Grade: %s%s%s%s\n", CYAN, BOLD, BRIGHT_GREEN, newStudent->grade, RESET);
    printf("\n");
    displayFooter();
}

void displayAllStudents() {
    int count = 0;
    Student* current;

    displayHeader("ALL STUDENT RECORDS", BRIGHT_BLUE);
    printf("\n");

    if (head == NULL) {
        printf("%s%s  [INFO]%s No student records found in the system.%s\n", BOLD, YELLOW, RESET, RESET);
        printf("\n");
        displayFooter();
        return;
    }

    current = head;

    printLine('-', 140, BRIGHT_MAGENTA);
    printf("%s%s  %-8s | %-20s | %-25s | %-15s | %-20s | %-6s | %-5s%s\n",
           BOLD, CYAN, "Roll No", "Name", "Email", "Phone", "Department", "Marks", "Grade", RESET);
    printLine('-', 140, BRIGHT_MAGENTA);

    while (current != NULL) {
        printf("%s  %-8d%s | %s%-20s%s | %s%-25s%s | %s%-15s%s | %s%-20s%s | %s%-6.2f%s | %s%-5s%s\n",
               BRIGHT_YELLOW, current->rollNo, RESET,
               BRIGHT_WHITE, current->name, RESET,
               BRIGHT_CYAN, current->email, RESET,
               BRIGHT_BLUE, current->phone, RESET,
               MAGENTA, current->department, RESET,
               BRIGHT_GREEN, current->marks, RESET,
               BOLD, current->grade, RESET);
        current = current->next;
        count++;
    }

    printLine('-', 140, BRIGHT_MAGENTA);
    printf("%s%s  Total Records: %d%s\n", BOLD, MAGENTA, count, RESET);
    printf("\n");
    displayFooter();
}

void searchStudent() {
    int choice;
    Student* current;
    int found = 0;

    displayHeader("SEARCH STUDENT RECORD", BRIGHT_YELLOW);
    printf("\n");

    if (head == NULL) {
        printf("%s%s  [INFO]%s No student records available to search.%s\n", BOLD, YELLOW, RESET, RESET);
        printf("\n");
        displayFooter();
        return;
    }

    printf("%s  Search Options:%s\n", CYAN, RESET);
    printf("%s%s  [1]%s Search by Roll Number%s\n", BOLD, YELLOW, RESET, RESET);
    printf("%s%s  [2]%s Search by Name%s\n", BOLD, YELLOW, RESET, RESET);
    printf("%s%s  [3]%s Search by Department%s\n", BOLD, YELLOW, RESET, RESET);
    printf("\n");

    choice = getValidChoice(1, 3);

    current = head;

    printf("\n");

    switch(choice) {
        case 1: {
            int rollNo = getValidRollNo();

            while (current != NULL) {
                if (current->rollNo == rollNo) {
                    printLine('-', 80, BRIGHT_GREEN);
                    printf("%s%s  [FOUND] Student Record:%s\n", BOLD, GREEN, RESET);
                    printLine('-', 80, BRIGHT_GREEN);
                    printf("%s  Roll Number :%s %s%d%s\n", CYAN, RESET, BRIGHT_YELLOW, current->rollNo, RESET);
                    printf("%s  Name        :%s %s%s%s\n", CYAN, RESET, BRIGHT_WHITE, current->name, RESET);
                    printf("%s  Email       :%s %s%s%s\n", CYAN, RESET, BRIGHT_CYAN, current->email, RESET);
                    printf("%s  Phone       :%s %s%s%s\n", CYAN, RESET, BRIGHT_BLUE, current->phone, RESET);
                    printf("%s  Department  :%s %s%s%s\n", CYAN, RESET, MAGENTA, current->department, RESET);
                    printf("%s  Marks       :%s %s%.2f%s\n", CYAN, RESET, BRIGHT_GREEN, current->marks, RESET);
                    printf("%s  Grade       :%s %s%s%s%s\n", CYAN, RESET, BOLD, BRIGHT_GREEN, current->grade, RESET);
                    printLine('-', 80, BRIGHT_GREEN);
                    found = 1;
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            char searchName[MAX_NAME_LENGTH];
            printf("%s  Enter Name to search: %s", CYAN, RESET);
            if (fgets(searchName, MAX_NAME_LENGTH, stdin) == NULL) {
                printf("%s%s  [ERROR]%s Error reading name!%s\n", BOLD, RED, RESET, RESET);
                displayFooter();
                return;
            }
            searchName[strcspn(searchName, "\n")] = 0;

            printLine('-', 140, BRIGHT_MAGENTA);
            printf("%s%s  %-8s | %-20s | %-25s | %-15s | %-20s | %-6s | %-5s%s\n",
                   BOLD, CYAN, "Roll No", "Name", "Email", "Phone", "Department", "Marks", "Grade", RESET);
            printLine('-', 140, BRIGHT_MAGENTA);

            while (current != NULL) {
                if (caseInsensitiveSearch(current->name, searchName)) {
                    printf("%s  %-8d%s | %s%-20s%s | %s%-25s%s | %s%-15s%s | %s%-20s%s | %s%-6.2f%s | %s%-5s%s\n",
                           BRIGHT_YELLOW, current->rollNo, RESET,
                           BRIGHT_WHITE, current->name, RESET,
                           BRIGHT_CYAN, current->email, RESET,
                           BRIGHT_BLUE, current->phone, RESET,
                           MAGENTA, current->department, RESET,
                           BRIGHT_GREEN, current->marks, RESET,
                           BOLD, current->grade, RESET);
                    found = 1;
                }
                current = current->next;
            }
            printLine('-', 140, BRIGHT_MAGENTA);
            break;
        }
        case 3: {
            char searchDept[MAX_DEPT_LENGTH];
            printf("%s  Enter Department to search: %s", CYAN, RESET);
            if (fgets(searchDept, MAX_DEPT_LENGTH, stdin) == NULL) {
                printf("%s%s  [ERROR]%s Error reading department!%s\n", BOLD, RED, RESET, RESET);
                displayFooter();
                return;
            }
            searchDept[strcspn(searchDept, "\n")] = 0;

            printLine('-', 140, BRIGHT_MAGENTA);
            printf("%s%s  %-8s | %-20s | %-25s | %-15s | %-20s | %-6s | %-5s%s\n",
                   BOLD, CYAN, "Roll No", "Name", "Email", "Phone", "Department", "Marks", "Grade", RESET);
            printLine('-', 140, BRIGHT_MAGENTA);

            while (current != NULL) {
                if (caseInsensitiveSearch(current->department, searchDept)) {
                    printf("%s  %-8d%s | %s%-20s%s | %s%-25s%s | %s%-15s%s | %s%-20s%s | %s%-6.2f%s | %s%-5s%s\n",
                           BRIGHT_YELLOW, current->rollNo, RESET,
                           BRIGHT_WHITE, current->name, RESET,
                           BRIGHT_CYAN, current->email, RESET,
                           BRIGHT_BLUE, current->phone, RESET,
                           MAGENTA, current->department, RESET,
                           BRIGHT_GREEN, current->marks, RESET,
                           BOLD, current->grade, RESET);
                    found = 1;
                }
                current = current->next;
            }
            printLine('-', 140, BRIGHT_MAGENTA);
            break;
        }
    }

    if (!found) {
        printf("%s%s  [INFO]%s No matching student record found.%s\n", BOLD, YELLOW, RESET, RESET);
    }

    printf("\n");
    displayFooter();
}

void updateStudent() {
    int rollNo;
    Student* current;

    displayHeader("UPDATE STUDENT RECORD", BRIGHT_MAGENTA);
    printf("\n");

    if (head == NULL) {
        printf("%s%s  [INFO]%s No student records available to update.%s\n", BOLD, YELLOW, RESET, RESET);
        printf("\n");
        displayFooter();
        return;
    }

    rollNo = getValidRollNo();

    current = head;
    while (current != NULL) {
        if (current->rollNo == rollNo) {
            char buffer[100];
            float newMarks;

            printf("\n");
            printLine('-', 80, BRIGHT_CYAN);
            printf("%s  Current Record Details:%s\n", CYAN, RESET);
            printLine('-', 80, BRIGHT_CYAN);
            printf("%s  Roll Number :%s %s%d%s\n", CYAN, RESET, BRIGHT_YELLOW, current->rollNo, RESET);
            printf("%s  Name        :%s %s%s%s\n", CYAN, RESET, BRIGHT_WHITE, current->name, RESET);
            printf("%s  Email       :%s %s%s%s\n", CYAN, RESET, BRIGHT_CYAN, current->email, RESET);
            printf("%s  Phone       :%s %s%s%s\n", CYAN, RESET, BRIGHT_BLUE, current->phone, RESET);
            printf("%s  Department  :%s %s%s%s\n", CYAN, RESET, MAGENTA, current->department, RESET);
            printf("%s  Marks       :%s %s%.2f%s\n", CYAN, RESET, BRIGHT_GREEN, current->marks, RESET);
            printf("%s  Grade       :%s %s%s%s%s\n", CYAN, RESET, BOLD, BRIGHT_GREEN, current->grade, RESET);
            printLine('-', 80, BRIGHT_CYAN);
            printf("\n");

            // Update name
            printf("%s  Enter new Name (or press Enter to keep current): %s", CYAN, RESET);
            if (fgets(buffer, MAX_NAME_LENGTH, stdin) != NULL && strlen(buffer) > 1) {
                buffer[strcspn(buffer, "\n")] = 0;
                strncpy(current->name, buffer, MAX_NAME_LENGTH - 1);
                current->name[MAX_NAME_LENGTH - 1] = '\0';
            }

            // Update email
            printf("%s  Enter new Email (or press Enter to keep current): %s", CYAN, RESET);
            if (fgets(buffer, MAX_EMAIL_LENGTH, stdin) != NULL && strlen(buffer) > 1) {
                buffer[strcspn(buffer, "\n")] = 0;
                if (isValidEmail(buffer)) {
                    strncpy(current->email, buffer, MAX_EMAIL_LENGTH - 1);
                    current->email[MAX_EMAIL_LENGTH - 1] = '\0';
                } else {
                    printf("%s%s  [INFO]%s Invalid email format. Keeping current email.%s\n", BOLD, YELLOW, RESET, RESET);
                }
            }

            // Update phone
            printf("%s  Enter new Phone (or press Enter to keep current): %s", CYAN, RESET);
            if (fgets(buffer, MAX_PHONE_LENGTH, stdin) != NULL && strlen(buffer) > 1) {
                buffer[strcspn(buffer, "\n")] = 0;
                if (isValidPhone(buffer)) {
                    strncpy(current->phone, buffer, MAX_PHONE_LENGTH - 1);
                    current->phone[MAX_PHONE_LENGTH - 1] = '\0';
                } else {
                    printf("%s%s  [INFO]%s Invalid phone format. Keeping current phone.%s\n", BOLD, YELLOW, RESET, RESET);
                }
            }

            // Update department
            printf("%s  Enter new Department (or press Enter to keep current): %s", CYAN, RESET);
            if (fgets(buffer, MAX_DEPT_LENGTH, stdin) != NULL && strlen(buffer) > 1) {
                buffer[strcspn(buffer, "\n")] = 0;
                strncpy(current->department, buffer, MAX_DEPT_LENGTH - 1);
                current->department[MAX_DEPT_LENGTH - 1] = '\0';
            }

            // Update marks
            printf("%s  Enter new Marks (or enter -1 to keep current): %s", CYAN, RESET);
            if (scanf("%f", &newMarks) != 1) {
                printf("%s%s  [INFO]%s Invalid input. Keeping current marks.%s\n", BOLD, YELLOW, RESET, RESET);
                clearInputBuffer();
            } else {
                clearInputBuffer();
                if (newMarks >= 0 && newMarks <= 100) {
                    current->marks = newMarks;
                    calculateGrade(current);
                } else if (newMarks != -1) {
                    printf("%s%s  [INFO]%s Invalid marks range. Keeping current marks.%s\n", BOLD, YELLOW, RESET, RESET);
                }
            }

            printf("\n");
            printf("%s%s  [SUCCESS]%s Student record updated successfully!%s\n", BOLD, GREEN, RESET, RESET);
            printf("%s  Updated Grade: %s%s%s%s\n", CYAN, BOLD, BRIGHT_GREEN, current->grade, RESET);
            printf("\n");
            displayFooter();
            return;
        }
        current = current->next;
    }

    printf("%s%s  [ERROR]%s Student with Roll Number %d not found!%s\n", BOLD, RED, RESET, rollNo, RESET);
    printf("\n");
    displayFooter();
}

void deleteStudent() {
    int rollNo;
    Student* current;
    Student* prev = NULL;

    displayHeader("DELETE STUDENT RECORD", BRIGHT_RED);
    printf("\n");

    if (head == NULL) {
        printf("%s%s  [INFO]%s No student records available to delete.%s\n", BOLD, YELLOW, RESET, RESET);
        printf("\n");
        displayFooter();
        return;
    }

    rollNo = getValidRollNo();

    current = head;

    // If head node itself needs to be deleted
    if (current != NULL && current->rollNo == rollNo) {
        char confirm;
        printf("\n");
        printLine('-', 80, BRIGHT_RED);
        printf("%s  Student: %s (Roll: %d)%s\n", RED, current->name, current->rollNo, RESET);
        printLine('-', 80, BRIGHT_RED);
        printf("%s%s  [WARNING]%s Are you sure you want to delete this record? (y/n): %s", BOLD, RED, RESET, RESET);
        scanf(" %c", &confirm);
        clearInputBuffer();

        if (confirm == 'y' || confirm == 'Y') {
            head = current->next;
            free(current);
            printf("%s%s  [SUCCESS]%s Student record deleted successfully!%s\n", BOLD, GREEN, RESET, RESET);
        } else {
            printf("%s%s  [INFO]%s Delete operation cancelled.%s\n", BOLD, YELLOW, RESET, RESET);
        }
        printf("\n");
        displayFooter();
        return;
    }

    // Search for the student to be deleted
    while (current != NULL && current->rollNo != rollNo) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        char confirm;
        printf("\n");
        printLine('-', 80, BRIGHT_RED);
        printf("%s  Student: %s (Roll: %d)%s\n", RED, current->name, current->rollNo, RESET);
        printLine('-', 80, BRIGHT_RED);
        printf("%s%s  [WARNING]%s Are you sure you want to delete this record? (y/n): %s", BOLD, RED, RESET, RESET);
        scanf(" %c", &confirm);
        clearInputBuffer();

        if (confirm == 'y' || confirm == 'Y') {
            prev->next = current->next;
            free(current);
            printf("%s%s  [SUCCESS]%s Student record deleted successfully!%s\n", BOLD, GREEN, RESET, RESET);
        } else {
            printf("%s%s  [INFO]%s Delete operation cancelled.%s\n", BOLD, YELLOW, RESET, RESET);
        }
    } else {
        printf("%s%s  [ERROR]%s Student with Roll Number %d not found!%s\n", BOLD, RED, RESET, rollNo, RESET);
    }

    printf("\n");
    displayFooter();
}

void saveToFile() {
    FILE* file;
    Student* current;
    int count = 0;

    displayHeader("SAVE RECORDS TO FILE", BRIGHT_GREEN);
    printf("\n");

    file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("%s%s  [ERROR]%s Unable to open file for writing!%s\n", BOLD, RED, RESET, RESET);
        printf("\n");
        displayFooter();
        return;
    }

    current = head;

    printf("%s  Saving records to file...%s\n", CYAN, RESET);

    while (current != NULL) {
        if (fwrite(current, sizeof(Student), 1, file) != 1) {
            printf("%s%s  [ERROR]%s Error writing student data!%s\n", BOLD, RED, RESET, RESET);
            fclose(file);
            printf("\n");
            displayFooter();
            return;
        }
        current = current->next;
        count++;
    }

    fclose(file);
    printf("%s%s  [SUCCESS]%s %d record(s) saved successfully!%s\n", BOLD, GREEN, RESET, count, RESET);
    printf("\n");
    displayFooter();
}

void loadFromFile() {
    FILE* file;
    Student temp;
    int count = 0;

    file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("%s%s  [INFO]%s No existing data file found. Starting fresh.%s\n", BOLD, YELLOW, RESET, RESET);
        return;
    }

    freeList();

    printf("%s  Loading records from file...%s\n", CYAN, RESET);

    while (fread(&temp, sizeof(Student), 1, file) == 1) {
        Student* newStudent = createStudent(temp.rollNo, temp.name, temp.email,
                                           temp.phone, temp.department, temp.marks);
        if (newStudent == NULL) {
            printf("%s%s  [ERROR]%s Error loading student data!%s\n", BOLD, RED, RESET, RESET);
            fclose(file);
            return;
        }

        if (head == NULL) {
            head = newStudent;
        } else {
            newStudent->next = head;
            head = newStudent;
        }
        count++;
    }

    fclose(file);
    printf("%s%s  [SUCCESS]%s %d record(s) loaded successfully!%s\n", BOLD, GREEN, RESET, count, RESET);
}

void freeList() {
    Student* current = head;
    Student* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    head = NULL;
}

int isRollNoUnique(int rollNo) {
    Student* current = head;
    while (current != NULL) {
        if (current->rollNo == rollNo) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
