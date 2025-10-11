#include<stdio.h>
#include <string.h>

struct Student{
        int Rollno;
        char Name[50];
        int subject_1;
        int subject_2;
        int subject_3;
        int Total_marks;
        float Avg_marks;
     };

void Input(struct Student s[],int students){
       for(int i=0;i<students;i++){
           int unique=0;
           while(!unique){
             unique=1;
           printf("Enter Roll number :");
            scanf("%d",&s[i].Rollno); 

            for(int j=0;j<i;j++){
                if(s[j].Rollno==s[i].Rollno){
                    printf("User alredy exists,Try again! \n");
                    unique=0;
                    break;
                }
            }
           }
         
            while(getchar() != '\n');         
        printf("Enter Name: ");
        fgets(s[i].Name, sizeof(s[i].Name), stdin);
        s[i].Name[strcspn(s[i].Name, "\n")] = '\0'; 
          
            printf("Enter marks for subject_1 :");
            scanf("%d",&s[i].subject_1);
            printf("Enter marks for subject_2 :");
            scanf("%d",&s[i].subject_2);
            printf("Enter marks for subject_3 :");
            scanf("%d",&s[i].subject_3);
            s[i].Total_marks=s[i].subject_1+s[i].subject_2+s[i].subject_3;
            s[i].Avg_marks=(s[i].subject_1+s[i].subject_2+s[i].subject_3)/3;
       }
       
}
void Output(struct Student s[],int students){
           for(int i=0;i<students;i++){
            printf("Rollno :");
            printf("%d\n",s[i].Rollno); 
         
            printf("Name :");
            printf("%s\n",s[i].Name); 
         
            printf("Total Marks :");
            printf("%d\n",s[i].Total_marks);   
             
            printf("Average Marks :");
            printf("%f\n",s[i].Avg_marks);
         
            if(s[i].Avg_marks>=85){
                printf("Grade : A \n");
                printf("Performance : ***** \n");
            }
            if(s[i].Avg_marks<85 && s[i].Avg_marks>=70){
                 printf("Grade : B \n");
                printf("Performance : **** \n");
            }
            if(s[i].Avg_marks<70 && s[i].Avg_marks>=50){
                 printf("Grade : C \n");
                printf("Performance : *** \n");
            }
            if(s[i].Avg_marks<50 && s[i].Avg_marks>=35){
                 printf("Grade : D \n");
                printf("Performance : ** \n");
            }
            if(s[i].Avg_marks<35 ){
                 printf("Grade : F \n");
            }
            printf("-----------------------------\n");      
        }
}
void printRolls(struct Student s[],int students){
          if(students==0) return;
          printRolls(s,students-1);
           printf("%d \n", s[students-1].Rollno);
}
int main(){
    int  students=0;
    printf("Enter number of Students : ");
    scanf("%d",&students);
     
     struct Student s[students];
     Input(s,students);
     Output(s,students);
     printf("List of Roll numbers: \n");
     printRolls(s,students);
    return 0;
}