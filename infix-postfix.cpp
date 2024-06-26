#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 100

struct Stack {
    int top;
    unsigned capacity;
    int* array;
};

Stack* createStack(unsigned capacity) {
    Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = 0;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isFull(Stack* stack) { return stack->top == stack->capacity; }
int isEmpty(Stack* stack) { return stack->top == 0; }

void push(Stack* stack, int value) {
    if (isFull(stack)) return;
    stack->array[++stack->top] = value;
}
int pop(Stack* stack) {
    if (isEmpty(stack)) return;
    return stack->array[stack->top--];
}
int peek(struct Stack* stack) {
    if (isEmpty(stack)) return;
    return stack->array[stack->top];
}

int precedence(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}
int isOperations(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }
void err() { printf("Error in Expression"); exit(0); }

void infixToPostfix(char* infix, char* postfix) {
    Stack* stack = createStack(MAX_SIZE);
    int i, j;
    for (i = 0, j = 0; infix[i]; i++) {
        char c = infix[i];
        if (isspace(c)) continue;
        else if (isdigit(c)) {
            while (isdigit(infix[i])) postfix[j++] = infix[i++];
            postfix[j++] = ' ';
            i--;
        }
        else if (c == '(') {
            push(stack, c);
        }
        else if (c == ')') {
            while (!isEmpty(stack) && peek(stack) != '(')
                postfix[j++] = pop(stack);
            if (isEmpty(stack) || peek(stack) != '(') err();
            pop(stack);
        }
        else if (isOperations(c)) {
            if (isOperations(infix[i]) && isOperations(infix[i + 1])) err();
            while (!isEmpty(stack) && precedence(c) <= precedence(peek(stack)))
                postfix[j++] = pop(stack);
            push(stack, c);
        }
        else err();
    }
    while (!isEmpty(stack)) {
        if (peek(stack) == '(') err();
        postfix[j++] = pop(stack);
    }
    postfix[j] = '\0';
}

int evaluatePostfix(char* postfix) {
    Stack* stack = createStack(MAX_SIZE);
    int i, operand1, operand2;
    for (i = 0; postfix[i]; i++) {
        char c = postfix[i];
        if (isdigit(c)) {
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            push(stack, num);
            i--;
        }
        else if (isspace(c)) continue;
        else if (isOperations(c)) {
            operand2 = pop(stack);
            operand1 = pop(stack);
            switch (c) {
            case '+':
                push(stack, operand1 + operand2);
                break;
            case '-':
                push(stack, operand1 - operand2);
                break;
            case '*':
                push(stack, operand1 * operand2);
                break;
            case '/':
                if (operand2 == 0) {
                    printf("Error: Division by zero\n");
                    return;
                }
                push(stack, operand1 / operand2);
                break;
            default:
                printf("Error: Invalid operator\n");
                return;
            }
        }
        else err();
    }
    return pop(stack);
}

int main() {
    char infix[MAX_SIZE], postfix[MAX_SIZE];
    printf("Enter an infix expression: ");
    fgets(infix, MAX_SIZE, stdin);
    infixToPostfix(infix, postfix);
    printf("Postfix expression: %s\n", postfix);
    printf("Result of evaluation: %d\n", evaluatePostfix(postfix));
    return 0;
}
