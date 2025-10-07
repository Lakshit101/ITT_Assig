#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "Database.txt"   
 
typedef struct {
    int id;
    char name[50];
    int age;
} User;

void create() {
    User u;
    printf("Enter ID: ");
    scanf("%d", &u.id);
    getchar();  

    printf("Enter Name: ");
    fgets(u.name, sizeof(u.name), stdin);
    u.name[strcspn(u.name, "\n")] = 0;  

    printf("Enter Age: ");
    scanf("%d", &u.age);

    FILE *fp = fopen(FILE_NAME, "a");  
    if (fp == NULL) {
        printf("Error !\n");
        return;
    }
    fprintf(fp, "%d,%s,%d\n", u.id, u.name, u.age); 
    fclose(fp);
    printf("User added  !\n");
}

 
void read() {
    FILE *fp = fopen(FILE_NAME, "r");  
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    User u;
    printf("\n Users List \n");
    while (fscanf(fp, "%d,%49[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", u.id, u.name, u.age);
    }
    fclose(fp);
}

 
void update() {
    int id, found = 0;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    getchar(); 

    FILE *fp = fopen(FILE_NAME, "r");
    FILE *other = fopen("other.txt", "w");
    if (fp == NULL || other == NULL) {
        printf("Error  !\n");
        return;
    }

    User u;
    while (fscanf(fp, "%d,%49[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            printf("Enter new Name: ");
            fgets(u.name, sizeof(u.name), stdin);
            u.name[strcspn(u.name, "\n")] = 0;

            printf("Enter new Age: ");
            scanf("%d", &u.age);
            getchar();
        }
        fprintf(other, "%d,%s,%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(other);

    remove(FILE_NAME);            
    rename("other.txt", FILE_NAME);  

    if (found)
        printf("User updated  !\n");
    else
        printf("User  %d not found!\n", id);
}

 
void delete() {
    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILE_NAME, "r");
    FILE *other = fopen("other.txt", "w");
    if (fp == NULL || other == NULL) {
        printf("Error opening file!\n");
        return;
    }

    User u;
    while (fscanf(fp, "%d,%49[^,],%d\n", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            continue; 
        }
        fprintf(other, "%d,%s,%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(other);

    remove(FILE_NAME);
    rename("other.txt", FILE_NAME);

    if (found)
        printf("User deleted successfully!\n");
    else
        printf("User with ID %d not found!\n", id);
}

 
int main() {
    int choice;
  

    while(1) {
        printf("\n----Choose Option----\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  

        switch (choice) {
            case 1: create(); break;
            case 2: read(); break;
            case 3: update(); break;
            case 4: delete(); break;
            case 5: printf("GOODBYE\n");exit(1);
            default: printf("Invalid choice!\n");
        }
    } 

    return 0;
}
