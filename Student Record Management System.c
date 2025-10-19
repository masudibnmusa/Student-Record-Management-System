#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define FILENAME "students.dat"

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

int main() {
    clearScreen();
    printf("=== Student Record Management System ===\n");

    // Load existing data from file
    loadFromFile();

    pauseScreen();

    int choice;
    do {
        clearScreen();
        displayMenu();
        printf("Enter your choice (1-7): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            pauseScreen();
            continue;
        }

        clearInputBuffer(); // Clear the input buffer

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
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                pauseScreen();
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
    printf("\nPress Enter to continue...");
    getchar();
}

void displayMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Add Student\n");
    printf("2. Display All Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student\n");
    printf("5. Delete Student\n");
    printf("6. Save to File\n");
    printf("7. Exit\n");
    printf("============\n");
}

Student* createStudent(int rollNo, const char* name, float marks) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    newStudent->rollNo = rollNo;
    strncpy(newStudent->name, name, MAX_NAME_LENGTH - 1);
    newStudent->name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null termination
    newStudent->marks = marks;
    newStudent->next = NULL;

    return newStudent;
}

void addStudent() {
    int rollNo;
    char name[MAX_NAME_LENGTH];
    float marks;

    printf("\n=== Add New Student ===\n");

    // Get roll number
    printf("Enter Roll Number: ");
    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid roll number!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Check if roll number is unique
    if (!isRollNoUnique(rollNo)) {
        printf("Error: Roll number %d already exists!\n", rollNo);
        return;
    }

    // Get name
    printf("Enter Name: ");
    if (fgets(name, MAX_NAME_LENGTH, stdin) == NULL) {
        printf("Error reading name!\n");
        return;
    }

    // Remove newline character from name
    name[strcspn(name, "\n")] = 0;

    // Validate name (should not be empty)
    if (strlen(name) == 0) {
        printf("Error: Name cannot be empty!\n");
        return;
    }

    // Get marks
    printf("Enter Marks: ");
    if (scanf("%f", &marks) != 1) {
        printf("Invalid marks!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Validate marks
    if (marks < 0 || marks > 100) {
        printf("Error: Marks should be between 0 and 100!\n");
        return;
    }

    // Create new student
    Student* newStudent = createStudent(rollNo, name, marks);
    if (newStudent == NULL) {
        return;
    }

    // Add to linked list (insert at beginning for simplicity)
    if (head == NULL) {
        head = newStudent;
    } else {
        newStudent->next = head;
        head = newStudent;
    }

    printf("\nStudent added successfully!\n");
}

void displayAllStudents() {
    printf("\n=== All Students ===\n");

    if (head == NULL) {
        printf("No students found!\n");
        return;
    }

    Student* current = head;
    int count = 0;

    printf("%-10s %-20s %-10s\n", "Roll No", "Name", "Marks");
    printf("----------------------------------------\n");

    while (current != NULL) {
        printf("%-10d %-20s %-10.2f\n",
               current->rollNo, current->name, current->marks);
        current = current->next;
        count++;
    }

    printf("----------------------------------------\n");
    printf("Total students: %d\n", count);
}

void searchStudent() {
    if (head == NULL) {
        printf("No students to search!\n");
        return;
    }

    int choice;
    printf("\n=== Search Student ===\n");
    printf("1. Search by Roll Number\n");
    printf("2. Search by Name\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Student* current = head;
    int found = 0;

    switch(choice) {
        case 1: {
            int rollNo;
            printf("Enter Roll Number to search: ");
            if (scanf("%d", &rollNo) != 1) {
                printf("Invalid roll number!\n");
                clearInputBuffer();
                return;
            }
            clearInputBuffer();

            while (current != NULL) {
                if (current->rollNo == rollNo) {
                    printf("\nStudent Found:\n");
                    printf("Roll No: %d\n", current->rollNo);
                    printf("Name: %s\n", current->name);
                    printf("Marks: %.2f\n", current->marks);
                    found = 1;
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            char searchName[MAX_NAME_LENGTH];
            printf("Enter Name to search: ");
            if (fgets(searchName, MAX_NAME_LENGTH, stdin) == NULL) {
                printf("Error reading name!\n");
                return;
            }
            searchName[strcspn(searchName, "\n")] = 0;

            printf("\nSearch Results:\n");
            printf("%-10s %-20s %-10s\n", "Roll No", "Name", "Marks");
            printf("----------------------------------------\n");

            while (current != NULL) {
                if (strstr(current->name, searchName) != NULL) {
                    printf("%-10d %-20s %-10.2f\n",
                           current->rollNo, current->name, current->marks);
                    found = 1;
                }
                current = current->next;
            }
            break;
        }
        default:
            printf("Invalid choice!\n");
            return;
    }

    if (!found) {
        printf("\nNo student found with the given criteria.\n");
    }
}

void updateStudent() {
    if (head == NULL) {
        printf("No students to update!\n");
        return;
    }

    int rollNo;
    printf("\n=== Update Student ===\n");
    printf("Enter Roll Number to update: ");

    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid roll number!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Student* current = head;
    while (current != NULL) {
        if (current->rollNo == rollNo) {
            printf("\nStudent Found:\n");
            printf("Current Name: %s\n", current->name);
            printf("Current Marks: %.2f\n", current->marks);

            // Get new name
            char newName[MAX_NAME_LENGTH];
            printf("\nEnter new Name (press enter to keep current): ");
            if (fgets(newName, MAX_NAME_LENGTH, stdin) == NULL) {
                printf("Error reading name!\n");
                return;
            }

            // If user entered something, update the name
            if (strlen(newName) > 1) { // More than just newline
                newName[strcspn(newName, "\n")] = 0;
                strncpy(current->name, newName, MAX_NAME_LENGTH - 1);
                current->name[MAX_NAME_LENGTH - 1] = '\0';
            }

            // Get new marks
            printf("Enter new Marks (enter -1 to keep current): ");
            float newMarks;
            if (scanf("%f", &newMarks) != 1) {
                printf("Invalid marks! Keeping current marks.\n");
                clearInputBuffer();
            } else {
                clearInputBuffer();
                if (newMarks >= 0 && newMarks <= 100) {
                    current->marks = newMarks;
                } else if (newMarks != -1) {
                    printf("Invalid marks range! Keeping current marks.\n");
                }
            }

            printf("\nStudent record updated successfully!\n");
            return;
        }
        current = current->next;
    }

    printf("\nStudent with Roll Number %d not found!\n", rollNo);
}

void deleteStudent() {
    if (head == NULL) {
        printf("No students to delete!\n");
        return;
    }

    int rollNo;
    printf("\n=== Delete Student ===\n");
    printf("Enter Roll Number to delete: ");

    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid roll number!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    Student* current = head;
    Student* prev = NULL;

    // If head node itself needs to be deleted
    if (current != NULL && current->rollNo == rollNo) {
        head = current->next;
        free(current);
        printf("\nStudent with Roll Number %d deleted successfully!\n", rollNo);
        return;
    }

    // Search for the student to be deleted
    while (current != NULL && current->rollNo != rollNo) {
        prev = current;
        current = current->next;
    }

    // If student was found
    if (current != NULL) {
        prev->next = current->next;
        free(current);
        printf("\nStudent with Roll Number %d deleted successfully!\n", rollNo);
    } else {
        printf("\nStudent with Roll Number %d not found!\n", rollNo);
    }
}

void saveToFile() {
    FILE* file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    Student* current = head;
    int count = 0;

    while (current != NULL) {
        if (fwrite(current, sizeof(Student), 1, file) != 1) {
            printf("Error writing student data!\n");
            fclose(file);
            return;
        }
        current = current->next;
        count++;
    }

    fclose(file);
    printf("\nData saved to file successfully! (%d records)\n", count);
}

void loadFromFile() {
    FILE* file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("No existing data file found. Starting with empty records.\n");
        return;
    }

    // Clear existing list
    freeList();

    Student temp;
    int count = 0;

    while (fread(&temp, sizeof(Student), 1, file) == 1) {
        // Create a new student with the loaded data
        Student* newStudent = createStudent(temp.rollNo, temp.name, temp.marks);
        if (newStudent == NULL) {
            printf("Error loading student data!\n");
            fclose(file);
            return;
        }

        // Add to linked list
        if (head == NULL) {
            head = newStudent;
        } else {
            newStudent->next = head;
            head = newStudent;
        }
        count++;
    }

    fclose(file);
    printf("Data loaded from file successfully! (%d records)\n", count);
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
            return 0; // Not unique
        }
        current = current->next;
    }
    return 1; // Unique
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
