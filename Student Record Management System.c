#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define FILENAME "students.dat"
#define VERSION "1.0"

// Student structure
typedef struct Student {
    int rollNo;
    char name[MAX_NAME_LENGTH];
    float marks;
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
Student* createStudent(int rollNo, const char* name, float marks);
void clearInputBuffer();
void clearScreen();
void pauseScreen();
void displayHeader(const char* title);
void displayFooter();
void printLine(char ch, int length);
void displayWelcome();
void displayExitMessage();
int getValidChoice(int min, int max);
int getValidRollNo();
float getValidMarks();
char* toLowerStr(char* str);
int caseInsensitiveSearch(const char* haystack, const char* needle);

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
    printLine('=', 60);
    printf("Press Enter to continue...");
    getchar();
}

void printLine(char ch, int length) {
    int i;
    for(i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}

void displayHeader(const char* title) {
    printLine('=', 60);
    printf("   %s\n", title);
    printLine('=', 60);
}

void displayFooter() {
    printLine('=', 60);
}

void displayWelcome() {
    printLine('=', 120);
    printf("\n");
    printf("%44s%s%44s\n", "", "STUDENT RECORD MANAGEMENT SYSTEM", "");
    printf("%56s%s%56s\n", "", "Version 3.1", "");
    printf("\n");
    printLine('=', 120);
    printf("\n");

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    printf("  System Date: %02d-%02d-%d\n", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
    printf("  System Time: %02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("\n");
}

void displayExitMessage() {
    displayHeader("SYSTEM EXIT");
    printf("\n");
    printf("  Thank you for using Student Record Management System!\n");
    printf("  All changes have been saved successfully.\n");
    printf("\n");
    printf("  Developed by: Your Name\n");
    printf("  Version: %s\n", VERSION);
    printf("\n");
    displayFooter();
}

void displayMenu() {
    displayHeader("                      MAIN MENU");
    printf("\n");
    printf("  [1] Add New Student Record\n");
    printf("  [2] Display All Student Records\n");
    printf("  [3] Search Student Record\n");
    printf("  [4] Update Student Record\n");
    printf("  [5] Delete Student Record\n");
    printf("  [6] Save Records to File\n");
    printf("  [7] Exit System\n");
    printf("\n");
    displayFooter();
}

int getValidChoice(int min, int max) {
    int choice;
    while(1) {
        printf("\n  Enter your choice [%d-%d]: ", min, max);

        if (scanf("%d", &choice) != 1) {
            printf("  [ERROR] Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (choice >= min && choice <= max) {
            return choice;
        } else {
            printf("  [ERROR] Invalid choice! Please select between %d and %d.\n", min, max);
        }
    }
}

int getValidRollNo() {
    int rollNo;
    while(1) {
        printf("  Enter Roll Number: ");
        if (scanf("%d", &rollNo) != 1 || rollNo <= 0) {
            printf("  [ERROR] Invalid roll number! Must be a positive integer.\n");
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
        printf("  Enter Marks (0-100): ");
        if (scanf("%f", &marks) != 1) {
            printf("  [ERROR] Invalid marks! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (marks < 0 || marks > 100) {
            printf("  [ERROR] Marks must be between 0 and 100!\n");
            continue;
        }
        return marks;
    }
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

Student* createStudent(int rollNo, const char* name, float marks) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("  [ERROR] Memory allocation failed!\n");
        return NULL;
    }

    newStudent->rollNo = rollNo;
    strncpy(newStudent->name, name, MAX_NAME_LENGTH - 1);
    newStudent->name[MAX_NAME_LENGTH - 1] = '\0';
    newStudent->marks = marks;
    newStudent->next = NULL;

    return newStudent;
}

void addStudent() {
    int rollNo;
    char name[MAX_NAME_LENGTH];
    float marks;

    displayHeader("ADD NEW STUDENT RECORD");
    printf("\n");

    // Get roll number
    rollNo = getValidRollNo();

    // Check if roll number is unique
    if (!isRollNoUnique(rollNo)) {
        printf("  [ERROR] Roll number %d already exists!\n", rollNo);
        return;
    }

    // Get name
    printf("  Enter Student Name: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
        printf("  [ERROR] Error reading name!\n");
        return;
    }

    name[strcspn(name, "\n")] = 0;

    if (strlen(name) == 0) {
        printf("  [ERROR] Name cannot be empty!\n");
        return;
    }

    // Get marks
    marks = getValidMarks();

    // Create new student
    Student* newStudent = createStudent(rollNo, name, marks);
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
    printf("  [SUCCESS] Student record added successfully!\n");
    printf("\n");
    displayFooter();
}

void displayAllStudents() {
    int count = 0;
    Student* current;

    displayHeader("ALL STUDENT RECORDS");
    printf("\n");

    if (head == NULL) {
        printf("  [INFO] No student records found in the system.\n");
        printf("\n");
        displayFooter();
        return;
    }

    current = head;

    printLine('-', 60);
    printf("  %-10s | %-25s | %-10s\n", "Roll No", "Name", "Marks");
    printLine('-', 60);

    while (current != NULL) {
        printf("  %-10d | %-25s | %-10.2f\n",
               current->rollNo, current->name, current->marks);
        current = current->next;
        count++;
    }

    printLine('-', 60);
    printf("  Total Records: %d\n", count);
    printf("\n");
    displayFooter();
}

void searchStudent() {
    int choice;
    Student* current;
    int found = 0;

    displayHeader("SEARCH STUDENT RECORD");
    printf("\n");

    if (head == NULL) {
        printf("  [INFO] No student records available to search.\n");
        printf("\n");
        displayFooter();
        return;
    }

    printf("  Search Options:\n");
    printf("  [1] Search by Roll Number\n");
    printf("  [2] Search by Name\n");
    printf("\n");

    choice = getValidChoice(1, 2);

    current = head;

    printf("\n");

    switch(choice) {
        case 1: {
            int rollNo = getValidRollNo();

            while (current != NULL) {
                if (current->rollNo == rollNo) {
                    printLine('-', 60);
                    printf("  [FOUND] Student Record:\n");
                    printLine('-', 60);
                    printf("  Roll Number: %d\n", current->rollNo);
                    printf("  Name       : %s\n", current->name);
                    printf("  Marks      : %.2f\n", current->marks);
                    printLine('-', 60);
                    found = 1;
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            char searchName[MAX_NAME_LENGTH];
            printf("  Enter Name to search: ");
            if (fgets(searchName, MAX_NAME_LENGTH, stdin) == NULL) {
                printf("  [ERROR] Error reading name!\n");
                displayFooter();
                return;
            }
            searchName[strcspn(searchName, "\n")] = 0;

            printLine('-', 60);
            printf("  %-10s | %-25s | %-10s\n", "Roll No", "Name", "Marks");
            printLine('-', 60);

            while (current != NULL) {
                if (caseInsensitiveSearch(current->name, searchName)) {
                    printf("  %-10d | %-25s | %-10.2f\n",
                           current->rollNo, current->name, current->marks);
                    found = 1;
                }
                current = current->next;
            }
            printLine('-', 60);
            break;
        }
    }

    if (!found) {
        printf("  [INFO] No matching student record found.\n");
    }

    printf("\n");
    displayFooter();
}

void updateStudent() {
    int rollNo;
    Student* current;

    displayHeader("UPDATE STUDENT RECORD");
    printf("\n");

    if (head == NULL) {
        printf("  [INFO] No student records available to update.\n");
        printf("\n");
        displayFooter();
        return;
    }

    rollNo = getValidRollNo();

    current = head;
    while (current != NULL) {
        if (current->rollNo == rollNo) {
            char newName[MAX_NAME_LENGTH];
            float newMarks;

            printf("\n");
            printLine('-', 60);
            printf("  Current Record Details:\n");
            printLine('-', 60);
            printf("  Roll Number: %d\n", current->rollNo);
            printf("  Name       : %s\n", current->name);
            printf("  Marks      : %.2f\n", current->marks);
            printLine('-', 60);
            printf("\n");

            // Get new name
            printf("  Enter new Name (or press Enter to keep current): ");
            if (fgets(newName, MAX_NAME_LENGTH, stdin) == NULL) {
                printf("  [ERROR] Error reading name!\n");
                displayFooter();
                return;
            }

            if (strlen(newName) > 1) {
                newName[strcspn(newName, "\n")] = 0;
                strncpy(current->name, newName, MAX_NAME_LENGTH - 1);
                current->name[MAX_NAME_LENGTH - 1] = '\0';
            }

            // Get new marks
            printf("  Enter new Marks (or enter -1 to keep current): ");
            if (scanf("%f", &newMarks) != 1) {
                printf("  [INFO] Invalid input. Keeping current marks.\n");
                clearInputBuffer();
            } else {
                clearInputBuffer();
                if (newMarks >= 0 && newMarks <= 100) {
                    current->marks = newMarks;
                } else if (newMarks != -1) {
                    printf("  [INFO] Invalid marks range. Keeping current marks.\n");
                }
            }

            printf("\n");
            printf("  [SUCCESS] Student record updated successfully!\n");
            printf("\n");
            displayFooter();
            return;
        }
        current = current->next;
    }

    printf("  [ERROR] Student with Roll Number %d not found!\n", rollNo);
    printf("\n");
    displayFooter();
}

void deleteStudent() {
    int rollNo;
    Student* current;
    Student* prev = NULL;

    displayHeader("DELETE STUDENT RECORD");
    printf("\n");

    if (head == NULL) {
        printf("  [INFO] No student records available to delete.\n");
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
        printf("  [WARNING] Are you sure you want to delete this record? (y/n): ");
        scanf(" %c", &confirm);
        clearInputBuffer();

        if (confirm == 'y' || confirm == 'Y') {
            head = current->next;
            free(current);
            printf("  [SUCCESS] Student record deleted successfully!\n");
        } else {
            printf("  [INFO] Delete operation cancelled.\n");
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
        printf("  [WARNING] Are you sure you want to delete this record? (y/n): ");
        scanf(" %c", &confirm);
        clearInputBuffer();

        if (confirm == 'y' || confirm == 'Y') {
            prev->next = current->next;
            free(current);
            printf("  [SUCCESS] Student record deleted successfully!\n");
        } else {
            printf("  [INFO] Delete operation cancelled.\n");
        }
    } else {
        printf("  [ERROR] Student with Roll Number %d not found!\n", rollNo);
    }

    printf("\n");
    displayFooter();
}

void saveToFile() {
    FILE* file;
    Student* current;
    int count = 0;

    displayHeader("SAVE RECORDS TO FILE");
    printf("\n");

    file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("  [ERROR] Unable to open file for writing!\n");
        printf("\n");
        displayFooter();
        return;
    }

    current = head;

    printf("  Saving records to file...\n");

    while (current != NULL) {
        if (fwrite(current, sizeof(Student), 1, file) != 1) {
            printf("  [ERROR] Error writing student data!\n");
            fclose(file);
            printf("\n");
            displayFooter();
            return;
        }
        current = current->next;
        count++;
    }

    fclose(file);
    printf("  [SUCCESS] %d record(s) saved to '%s' successfully!\n", count, FILENAME);
    printf("\n");
    displayFooter();
}

void loadFromFile() {
    FILE* file;
    Student temp;
    int count = 0;

    file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("  [INFO] No existing data file found. Starting fresh.\n");
        return;
    }

    freeList();

    printf("  Loading records from file...\n");

    while (fread(&temp, sizeof(Student), 1, file) == 1) {
        Student* newStudent = createStudent(temp.rollNo, temp.name, temp.marks);
        if (newStudent == NULL) {
            printf("  [ERROR] Error loading student data!\n");
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
    printf("  [SUCCESS] Record(s) loaded successfully!\n");
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
