#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 1000

int compute_stack(int stack[], int top) {
    int result = 0;
    for (int i = 0; i <= top; i++) {
        result += stack[i];
    }
    return result;
}

int evaluate_expression(char expression[]) {
    int stack[MAX_LENGTH];
    int number = 0;
    char op = '+';
    int top = -1;
    int length = strlen(expression);

    for (int i = 0; i < length; i++) {
        char ch = expression[i];

        switch (ch) {
            case ' ':
                continue;

            case '+':
            case '-':
            case '*':
            case '/':
            case '\n':
            {
                if (op == '/' && number == 0) {
                    printf("Error: Division by zero.\n");
                    return 0;
                }

                switch (op) {
                    case '+': stack[++top] = number; break;
                    case '-': stack[++top] = -number; break;
                    case '*': stack[top] *= number; break;
                    case '/': stack[top] /= number; break;
                }

                op = ch;
                number = 0;
                break;
            }

            default:
                if (ch >= '0' && ch <= '9') {
                    number = number * 10 + (ch - '0');
                } else {
                    printf("Invalid Expression.\n");
                    return 0;
                }
                break;
        }
    }

    return compute_stack(stack, top);
}

void process_input() {
    char expression[MAX_LENGTH];
    while (1) {
        printf("Enter Expression: \n");
        if (fgets(expression, MAX_LENGTH, stdin) == NULL)
            break;

        int result = evaluate_expression(expression);
        if (result != 0 || (expression[0] >= '0' && expression[0] <= '9'))
            printf("Result = %d\n", result);
    }
}

int main() {
    process_input();
    return 0;
}
