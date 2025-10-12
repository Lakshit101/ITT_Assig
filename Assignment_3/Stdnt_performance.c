#include <stdio.h>
#include <string.h>

struct Student
{
    int Rollno;
    char Name[50];
    int subject_1;
    int subject_2;
    int subject_3;
    int Total_marks;
    float Avg_marks;
};

void Input(struct Student s[], int students)
{
    for (int i = 0; i < students; i++)
    {

        int valid = 0;
        while (!valid)
        {
            printf("Enter Roll number : ");
            if (scanf("%d", &s[i].Rollno) != 1 || s[i].Rollno <= 0)
            {
                printf("Invalid input! Enter a positive integer.\n");
                while (getchar() != '\n');
                 
            }

           int duplicate = 1;
            for (int j = 0; j < i; j++)
            {
                if (s[j].Rollno == s[i].Rollno)
                {
                    printf("Roll number already exists! Try again.\n");
                    duplicate = 0;
               
                }
            }
            if(duplicate){
                valid=1;
            }
        }
        while (getchar() != '\n');

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

        do
        {
            printf("Enter marks for subject_1 : ");
            if (scanf("%d", &s[i].subject_1) != 1 || s[i].subject_1 < 0 || s[i].subject_1 > 100)
            {
                printf("Invalid marks! Enter a number between 0 and 100.\n");
                while (getchar() != '\n');
            }
            else
                break;
        } while (1);

        do
        {
            printf("Enter marks for subject_2 : ");
            if (scanf("%d", &s[i].subject_2) != 1 || s[i].subject_2 < 0 || s[i].subject_2 > 100)
            {
                printf("Invalid marks! Enter a number between 0 and 100.\n");
                while (getchar() != '\n');
            }
            else
                break;
        } while (1);

        do
        {
            printf("Enter marks for subject_3 : ");
            if (scanf("%d", &s[i].subject_3) != 1 || s[i].subject_3 < 0 || s[i].subject_3 > 100)
            {
                printf("Invalid marks! Enter a number between 0 and 100.\n");
                while (getchar() != '\n');
            }
            else
                break;
        } while (1);

        s[i].Total_marks = s[i].subject_1 + s[i].subject_2 + s[i].subject_3;
        s[i].Avg_marks = (s[i].Total_marks) / 3.0;
    }
}

void Output(struct Student s[], int students)
{
    for (int i = 0; i < students; i++)
    {
        printf("Rollno : %d\n", s[i].Rollno);
        printf("Name : %s\n", s[i].Name);
        printf("Total Marks : %d\n", s[i].Total_marks);
        printf("Average Marks : %.2f\n", s[i].Avg_marks);

        if (s[i].Avg_marks >= 85)
        {
            printf("Grade : A \n");
            printf("Performance : ***** \n");
        }
        else if (s[i].Avg_marks >= 70)
        {
            printf("Grade : B \n");
            printf("Performance : **** \n");
        }
        else if (s[i].Avg_marks >= 50)
        {
            printf("Grade : C \n");
            printf("Performance : *** \n");
        }
        else if (s[i].Avg_marks >= 35)
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

void printRolls(struct Student s[], int students)
{
    if (students == 0)
        return;
    printRolls(s, students - 1);
    printf("%d\n", s[students - 1].Rollno);
}

int main()
{
    int students = 0;
    printf("Enter number of Students : ");
    scanf("%d", &students);

    struct Student s[students];
    Input(s, students);
    printf("------Report card-------- \n");
    Output(s, students);
    printf("List of Roll numbers: \n");
    printRolls(s, students);
    printf("\n");

    return 0;
}
