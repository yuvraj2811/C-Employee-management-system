#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Employee {
    int id;
    char name[50];
    char department[50];
    char designation[50];
    float salary;
    char contact[15];
} Employee;

Employee emp;
FILE *fp;

// Function Declarations
void addEmployee();
void viewAllEmployees();
void searchEmployee();
void updateEmployee();
void deleteEmployee();
void manageSalary();
void generateReports();
void sortEmployees();
void departmentWiseReport();
void salaryWiseReport();

int main() {
    int ch;
    while (1) {
        system("cls");
        printf("<== Employee Management System ==>\n");
        printf("1. Add Employee\n");
        printf("2. View All Employees\n");
        printf("3. Search Employee by ID/Name/Department\n");
        printf("4. Update Employee Details\n");
        printf("5. Delete Employee\n");
        printf("6. Manage Employee Salaries\n");
        printf("7. Generate Reports\n");
        printf("8. Sort Employees\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addEmployee(); break;
            case 2: viewAllEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: manageSalary(); break;
            case 7: generateReports(); break;
            case 8: sortEmployees(); break;
            case 9: exit(0); break;
            default: printf("Invalid choice! Please try again.\n"); break;
        }

        printf("\nPress any key to continue...\n");
        getchar(); // to consume the newline character
        getchar(); // wait for user to press a key
    }

    return 0;
}

void addEmployee() {
    fp = fopen("EmployeeData.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Employee ID: ");
    scanf("%d", &emp.id);
    getchar(); // consume newline character

    printf("Enter Employee Name: ");
    fgets(emp.name, sizeof(emp.name), stdin);
    emp.name[strcspn(emp.name, "\n")] = 0; // remove trailing newline

    printf("Enter Department: ");
    fgets(emp.department, sizeof(emp.department), stdin);
    emp.department[strcspn(emp.department, "\n")] = 0;

    printf("Enter Designation: ");
    fgets(emp.designation, sizeof(emp.designation), stdin);
    emp.designation[strcspn(emp.designation, "\n")] = 0;

    printf("Enter Salary: ");
    scanf("%f", &emp.salary);
    getchar(); // consume newline character

    printf("Enter Contact Number: ");
    fgets(emp.contact, sizeof(emp.contact), stdin);
    emp.contact[strcspn(emp.contact, "\n")] = 0;

    fprintf(fp, "%d\t%s\t%s\t%s\t%.2f\t%s\n", emp.id, emp.name, emp.department, emp.designation, emp.salary, emp.contact);

    printf("Employee added successfully!\n");
    fclose(fp);
}

void viewAllEmployees() {
    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n                                 <== All Employee Details ==>\n");
    //printf("ID\tName\t\tDepartment\tDesignation\tSalary\tContact\n");

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

void searchEmployee() {
    int id;
    char name[50], department[50];
    char line[200];

    printf("Search Employee by ID, Name or Department\n");
    printf("Enter ID (0 if searching by Name/Department): ");
    scanf("%d", &id);
    getchar(); // consume newline character

    if (id == 0) {
        printf("Enter Name or Department to search: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
    } else {
        name[0] = '\0'; // if searching by ID, clear name
    }

    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int empId;
        sscanf(line, "%d", &empId);

        if (id != 0 && empId == id) {
            printf("Employee found:\n%s", line);
            found = 1;
            break;
        }

        if (id == 0) {
            if (strstr(line, name)) {
                printf("Employee found:\n%s", line);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("Employee not found.\n");
    }

    fclose(fp);
}

void updateEmployee() {
    int id;
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    getchar(); // consume newline character

    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    FILE *temp = fopen("TempData.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file!\n");
        fclose(fp);
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int empId;
        sscanf(line, "%d", &empId);

        if (empId == id) {
            found = 1;
            printf("Enter new details for employee ID %d:\n", id);
            printf("Enter Name: ");
            fgets(emp.name, sizeof(emp.name), stdin);
            emp.name[strcspn(emp.name, "\n")] = 0;

            printf("Enter Department: ");
            fgets(emp.department, sizeof(emp.department), stdin);
            emp.department[strcspn(emp.department, "\n")] = 0;

            printf("Enter Designation: ");
            fgets(emp.designation, sizeof(emp.designation), stdin);
            emp.designation[strcspn(emp.designation, "\n")] = 0;

            printf("Enter Salary: ");
            scanf("%f", &emp.salary);
            getchar(); // consume newline character

            printf("Enter Contact Number: ");
            fgets(emp.contact, sizeof(emp.contact), stdin);
            emp.contact[strcspn(emp.contact, "\n")] = 0;

            fprintf(temp, "%d\t%s\t%s\t%s\t%.2f\t%s\n", emp.id, emp.name, emp.department, emp.designation, emp.salary, emp.contact);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("EmployeeData.txt");
        rename("TempData.txt", "EmployeeData.txt");
        printf("Employee details updated successfully!\n");
    } else {
        remove("TempData.txt");
        printf("Employee ID not found!\n");
    }
}

void deleteEmployee() {
    int id;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    getchar(); // consume newline character

    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    FILE *temp = fopen("TempData.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file!\n");
        fclose(fp);
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int empId;
        sscanf(line, "%d", &empId);

        if (empId != id) {
            fprintf(temp, "%s", line);
        } else {
            found = 1;
            printf("Employee ID %d deleted successfully!\n", id);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("EmployeeData.txt");
        rename("TempData.txt", "EmployeeData.txt");
    } else {
        remove("TempData.txt");
        printf("Employee ID not found!\n");
    }
}

void manageSalary() {
    int id;
    printf("Enter Employee ID to manage salary: ");
    scanf("%d", &id);
    getchar(); // consume newline character

    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    FILE *temp = fopen("TempData.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file!\n");
        fclose(fp);
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        int empId;
        sscanf(line, "%d", &empId);

        if (empId == id) {
            found = 1;
            printf("Enter new salary for employee ID %d: ", id);
            scanf("%f", &emp.salary);

            fprintf(temp, "%d\t%s\t%s\t%s\t%.2f\t%s\n", empId, emp.name, emp.department, emp.designation, emp.salary, emp.contact);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("EmployeeData.txt");
        rename("TempData.txt", "EmployeeData.txt");
        printf("Employee salary updated successfully!\n");
    } else {
        remove("TempData.txt");
        printf("Employee ID not found!\n");
    }
}

void generateReports() {
    int choice;
    printf("1. Department-wise report\n");
    printf("2. Salary-wise report\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: departmentWiseReport(); break;
        case 2: salaryWiseReport(); break;
        default: printf("Invalid choice!\n"); break;
    }
}

void departmentWiseReport() {
    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n<== Department-wise Report ==>\n");
    printf("ID\tName\t\tDepartment\tDesignation\tSalary\tContact\n");

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

void salaryWiseReport() {
    fp = fopen("EmployeeData.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n<== Salary-wise Report ==>\n");
    printf("ID\tName\t\tDepartment\tDesignation\tSalary\tContact\n");

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

void sortEmployees() {
    
    printf("some error founnd this function is not working please try it later\n");
    
}
