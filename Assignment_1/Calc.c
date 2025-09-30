#include <stdio.h>
#include <string.h>
#define MAX 1000

int main()
{
   char expression[MAX];
   int stack[MAX];
   int number = 0;
   char op = '+';
   int top = -1;

   printf("Enter Expression: \n");
   fgets(expression, MAX, stdin);
   int n = strlen(expression);

   for (int i = 0; i < n; i++)
   {
      char ch = expression[i];

      if (ch == ' ')
         continue;

      if (ch >= '0' && ch <= '9')
      {
         number = number * 10 + (ch - '0');
      }

      else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || i == n - 1)
      {

         if (op == '/' && number == 0)
         {
            printf("Error: Division by zero.\n");
            return 1;
         }
 
         if (op == '+')
            stack[++top] = number;
         else if (op == '-')
            stack[++top] = -number;
         else if (op == '*')
            stack[top] *= number;
         else if (op == '/')
            stack[top] /= number;

         op = ch;
         number = 0;
      }

      else
      {
         printf("Invalid Expression. \n");
         return 1;
      }
   }

   int result = 0;
   for (int i = 0; i <= top; i++)
   {
      result += stack[i];
   }

   printf("Result = %d\n", result);
   return 0;
}
