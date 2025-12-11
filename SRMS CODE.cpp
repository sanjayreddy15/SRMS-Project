#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"
#define TEMP_FILE "temp.txt"

char currentRole[20];
char currentUser[50];

struct Student
{
    char roll[32];    // changed to string to accept alpha-numeric rolls
    char name[50];
    float marks;
};

// FUNCTION DECLARATIONS
void mainMenu();
void adminMenu();
void staffMenu();
void userMenu();
void guestMenu();
int loginSystem();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

void ensure_files_exist();   // NEW

int main()
{
    ensure_files_exist();    // create credentials/students if missing

    if (loginSystem())
    {
        mainMenu();
    }
    else
    {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

/* Create credentials.txt with default users (ADMIN/STAFF/USER/GUEST)
   and an empty students.txt if they don't exist.
*/
void ensure_files_exist()
{
    FILE *f;

    f = fopen(CREDENTIAL_FILE, "r");
    if (!f)
    {
        f = fopen(CREDENTIAL_FILE, "w");
        if (f)
        {
            /* username password ROLE */
            fprintf(f, "admin admin123 ADMIN\n");
            fprintf(f, "staff staff123 STAFF\n");
            fprintf(f, "user user123 USER\n");
            fprintf(f, "guest guest123 GUEST\n");
            fclose(f);
            printf("Created default '%s' (admin/admin123).\n", CREDENTIAL_FILE);
        }
    }
    else
    {
        fclose(f);
    }

    f = fopen(STUDENT_FILE, "r");
    if (!f)
    {
        f = fopen(STUDENT_FILE, "w");
        if (f) fclose(f);
    }
    else
    {
        fclose(f);
    }
}

// LOGIN SYSTEM
int loginSystem()
{
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("============= Login Screen ===========\n");
    printf("Username: ");
    if (scanf("%49s", username) != 1) return 0;
    printf("Password: ");
    if (scanf("%49s", password) != 1) return 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp)
    {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %19s", fileUser, filePass, fileRole) == 3)
    {
        // safety trim newline (usually not necessary here)
        fileRole[strcspn(fileRole, "\r\n")] = 0;

        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0)
        {
            strncpy(currentRole, fileRole, sizeof(currentRole) - 1);
            currentRole[sizeof(currentRole) - 1] = '\0';
            strncpy(currentUser, fileUser, sizeof(currentUser) - 1);
            currentUser[sizeof(currentUser) - 1] = '\0';
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// MAIN MENU
void mainMenu()
{
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "GUEST") == 0)
        guestMenu();
    else
        printf("Access Denied! Invalid Role.\n");
}

// ADMIN MENU
void adminMenu()
{
    int choice;
    do
    {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            updateStudent();
            break;
        case 5:
            deleteStudent();
            break;
        case 6:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

// STAFF MENU
void staffMenu()
{
    int choice;
    do
    {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }

        switch (choice)
        {
        case 1:
            displayStudents();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

// USER MENU
void userMenu()
{
    int choice;
    do
    {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }

        switch (choice)
        {
        case 1:
            displayStudents();
            break;
        case 2:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

// GUEST MENU
void guestMenu()
{
    printf("\nWelcome GUEST: %s\n", currentUser);
    printf("You have read-only access.\n");
    displayStudents();
}

// ADD STUDENT
void addStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    if (scanf("%31s", st.roll) != 1) { printf("Invalid roll.\n"); fclose(fp); return; }
    printf("Enter Name (no spaces): ");
    if (scanf("%49s", st.name) != 1) { printf("Invalid name.\n"); fclose(fp); return; }
    printf("Enter Marks: ");
    if (scanf("%f", &st.marks) != 1) st.marks = 0.0f;

    fprintf(fp, "%s %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

// DISPLAY STUDENTS
void displayStudents()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp)
    {
        printf("No data available.\n");
        return;
    }

    printf("\n====== STUDENT LIST ======\n");
    int count = 0;
    while (fscanf(fp, "%31s %49s %f", st.roll, st.name, &st.marks) == 3)
    {
        printf("Roll: %s | Name: %s | Marks: %.2f\n", st.roll, st.name, st.marks);
        count++;
    }

    if (count == 0)
        printf("(No records found)\n");

    fclose(fp);
}

// SEARCH STUDENT
void searchStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    char r[32];
    int found = 0;

    if (!fp)
    {
        printf("No data found.\n");
        return;
    }

    printf("Enter Roll Number to Search: ");
    if (scanf("%31s", r) != 1) { printf("Invalid input.\n"); fclose(fp); return; }

    while (fscanf(fp, "%31s %49s %f", st.roll, st.name, &st.marks) == 3)
    {
        if (strcmp(st.roll, r) == 0)
        {
            printf("\nStudent Found!\n");
            printf("Roll: %s\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");

    fclose(fp);
}

// UPDATE STUDENT  (VERY SIMPLE)
void updateStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    struct Student st;
    char r[32];
    int found = 0;

    if (!fp || !temp)
    {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll Number to Update: ");
    if (scanf("%31s", r) != 1) { printf("Invalid input.\n"); fclose(fp); fclose(temp); return; }

    while (fscanf(fp, "%31s %49s %f", st.roll, st.name, &st.marks) == 3)
    {
        if (strcmp(st.roll, r) == 0)
        {
            found = 1;
            printf("Current Name: %s, Current Marks: %.2f\n", st.name, st.marks);
            printf("Enter New Name (no spaces, or - to keep): ");
            char newName[50];
            if (scanf("%49s", newName) == 1)
            {
                if (strcmp(newName, "-") != 0)
                    strncpy(st.name, newName, sizeof(st.name)-1);
            }
            printf("Enter New Marks (or -1 to keep): ");
            float newMarks;
            if (scanf("%f", &newMarks) == 1)
            {
                if (newMarks >= 0.0f)
                    st.marks = newMarks;
            }
            printf("Record Updated!\n");
        }
        fprintf(temp, "%s %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(STUDENT_FILE);
        rename(TEMP_FILE, STUDENT_FILE);
    }
    else
    {
        printf("Record not found!\n");
        remove(TEMP_FILE);
    }
}

// DELETE STUDENT (SIMPLE)
void deleteStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen(TEMP_FILE, "w");
    struct Student st;
    char r[32];
    int found = 0;

    if (!fp || !temp)
    {
        printf("Error opening file!\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter Roll Number to Delete: ");
    if (scanf("%31s", r) != 1) { printf("Invalid input.\n"); fclose(fp); fclose(temp); return; }

    while (fscanf(fp, "%31s %49s %f", st.roll, st.name, &st.marks) == 3)
    {
        if (strcmp(st.roll, r) == 0)
        {
            found = 1;
            printf("Record Deleted!\n");
            continue; // Skip writing this student
        }
        fprintf(temp, "%s %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(STUDENT_FILE);
        rename(TEMP_FILE, STUDENT_FILE);
    }
    else
    {
        printf("Record not found!\n");
        remove(TEMP_FILE);
    }
}
