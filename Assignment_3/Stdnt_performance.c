#include <stdio.h>
#include <string.h>

struct Student
{
    int rollNo;
    char Name[50];
    int subject1;
    int subject2;
    int subject3;
    int totalMarks;
    float avgMarks;
};

void inputMarks(struct Student s[], int numberOfStudents, int i)
{

    do
    {
        printf("Enter marks for subject1 : ");
        if (scanf("%d", &s[i].subject1) != 1 || s[i].subject1 < 0 || s[i].subject1 > 100)
        {
            printf("Invalid marks! Enter a number between 0 and 100 :");
            while (getchar() != '\n') ;
        }
        else
            break;
    } while (1);

    do
    {
        printf("Enter marks for subject2 : ");
        if (scanf("%d", &s[i].subject2) != 1 || s[i].subject2 < 0 || s[i].subject2 > 100)
        {
            printf("Invalid marks! Enter a number between 0 and 100.\n");
            while (getchar() != '\n') ;
        }
        else
            break;
    } while (1);

    do
    {
        printf("Enter marks for subject3 : ");
        if (scanf("%d", &s[i].subject3) != 1 || s[i].subject3 < 0 || s[i].subject3 > 100)
        {
            printf("Invalid marks! Enter a number between 0 and 100.\n");
            while (getchar() != '\n')  ;
        }
        else
            break;
    } while (1);

    s[i].totalMarks = s[i].subject1 + s[i].subject2 + s[i].subject3;
    s[i].avgMarks = (s[i].totalMarks) / 3.0;
}

void basicDetails(struct Student s[], int numberOfStudents)
{
    int i = 0;
    for (i = 0; i < numberOfStudents; i++)
    {

        int valid = 0;
        while (!valid)
        {
            printf("Enter Roll number : ");
            if (scanf("%d", &s[i].rollNo) != 1 || s[i].rollNo <= 0)
            {
                printf("Invalid input! Enter a positive integer.\n");
                while (getchar() != '\n')    ;
            }

            int duplicate = 1;
            for (int j = 0; j < i; j++)
            {
                if (s[j].rollNo == s[i].rollNo)
                {
                    printf("Roll number already exists! Try again.\n");
                    duplicate = 0;
                }
            }
            if (duplicate)
            {
                valid = 1;
            }
        }
        while (getchar() != '\n') ;

        int validName = 0;
        while (!validName)
        {
            printf("Enter Name: ");
            fgets(s[i].Name, sizeof(s[i].Name), stdin);
            s[i].Name[strcspn(s[i].Name, "\n")] = '\0';

            validName = 1;
            for (int k = 0; s[i].Name[k] != '\0'; k++)
            {
                if (!((s[i].Name[k] >= 'A' && s[i].Name[k] <= 'Z') ||
                      (s[i].Name[k] >= 'a' && s[i].Name[k] <= 'z') ||
                      s[i].Name[k] == ' '))
                {
                    printf("Invalid name! Use only letters.\n");
                    validName = 0;
                    break;
                }
            }
        }
        inputMarks(s, numberOfStudents, i);
    }
}

void Output(struct Student s[], int numberOfStudents)
{
    for (int i = 0; i < numberOfStudents; i++)
    {
        printf("rollNo : %d\n", s[i].rollNo);
        printf("Name : %s\n", s[i].Name);
        printf("Total Marks : %d\n", s[i].totalMarks);
        printf("Average Marks : %.2f\n", s[i].avgMarks);

        if (s[i].avgMarks >= 85)
        {
            printf("Grade : A \n");
            printf("Performance : ***** \n");
        }
        else if (s[i].avgMarks >= 70)
        {
            printf("Grade : B \n");
            printf("Performance : **** \n");
        }
        else if (s[i].avgMarks >= 50)
        {
            printf("Grade : C \n");
            printf("Performance : *** \n");
        }
        else if (s[i].avgMarks >= 35)
        {
            printf("Grade : D \n");
            printf("Performance : ** \n");
        }
        else
        {
            printf("Grade : F \n");
        }
        printf("-----------------------------\n");
    }
}

void printRolls(struct Student s[], int numberOfStudents)
{
    if (numberOfStudents == 0)
        return;
    printRolls(s, numberOfStudents - 1);
    printf("%d\n", s[numberOfStudents - 1].rollNo);
}

int main()
{
    int numberOfStudents = 0;
    printf("Enter number of Students : ");
    scanf("%d", &numberOfStudents);

    struct Student s[numberOfStudents];
    basicDetails(s, numberOfStudents);
    printf("------Report card-------- \n");
    Output(s, numberOfStudents);
    printf("List of Roll numbers: \n");
    printRolls(s, numberOfStudents);
    printf("\n");

    return 0;
}
