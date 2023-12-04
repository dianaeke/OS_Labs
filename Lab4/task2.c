#include <stdio.h>
#include <stdlib.h>

/* IMPLEMENT ME: Declare your functions here */
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

int main(void)
{
    /* IMPLEMENT ME: Insert your algorithm here */
    int a, b;
    char op_char;
    printf("Operand 'a' : ");
    scanf("%d", &a);
    printf("Operand 'b' : ");
    scanf("%d", &b);
    printf("Specify the operation to perform (0 : add | 1 : subtract | 2 : Multiply | 3 : divide | 4 : exit): ");
    scanf(" %c", &op_char);
    if (op_char == '4') {
        printf("Exiting program\n");
        return 0;
    }
    int result = 0;
    if (op_char >= '0' && op_char <= '3') {
        // Check if the entered operation is valid
        int (*operations[])(int, int) = {add, subtract, multiply, divide};
        result = operations[op_char - '0'](a, b);
        printf("x = %d\n", result);
    } else {
        printf("Invalid operation\n");
    }
    return 0;
}

/* IMPLEMENT ME: Define your functions here */
int add(int a, int b)
{
    printf("Adding a and b\n");
    return a + b;
}

int subtract(int a, int b)
{
    printf("Subtracting b from a\n");
    return a - b;
}

int multiply(int a, int b)
{
    printf("Multiplying a and b\n");
    return a * b;
}

int divide(int a, int b)
{
    if (b != 0) {
        printf("Dividing a by b\n");
        return a / b;
    } else {
        printf("Error: Division by zero\n");
        return 0; // You may want to handle division by zero differently
    }
}