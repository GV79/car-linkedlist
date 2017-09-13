#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*******************************************
CIS2520 Assignment 2
Part: Question 2
Gia Vinh Lam
0925571
glam01@mail.uoguelph.ca
October 2016
This program evaluates a post-fix expression
********************************************/

char stackPop(int stack[100], int * ceil);
void stackPush(int stack[100], int character, int max, int * ceil);

// Compiler starts at this function: int main takes in arguments and runs program from beginning to end
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Need to add argument for expression.\n");
        exit(0);
    }

    char array[100];
    char character; //will allow us to determine if a character in argument is digit/operand
    int stack[100]; //array for stack of size 100
    int i, j; //counters
    int operandOne, operandTwo;
    int ceil;
    int const max = 100;
    i = 0;
    ceil = -1;
    j = 0;
    operandOne = 0;
    operandTwo = 0;

    strcpy(array, argv[1]);
    while (i != strlen(array))
    {
        character = array[j];
        if (isdigit(character)) //if it is a number
        {
            stackPush(stack, character - '0', max, &ceil);
        }
        else //if operand
        {
            operandTwo = stackPop(stack, &ceil); //operandTwo first because otherwise evaluation will be negative
            operandOne = stackPop(stack, &ceil);

            switch(character)
            {
                 case '+':
                     stackPush(stack, operandOne + operandTwo, max, &ceil);
                     break;
                 case '-':
                     stackPush(stack, operandOne - operandTwo, max, &ceil);
                     break;
                 case '*':
                     stackPush(stack, operandOne * operandTwo, max, &ceil);
                     break;
                 case '/':
                     stackPush(stack, operandOne / operandTwo, max, &ceil);
                     break;
            }
        }

        j++;
        i++;
    }
    printf("Expression is equal to: %d.00\n", stack[ceil]);
    return 0;
}

void stackPush(int stack[51], int character, int max, int * ceil)   //puts value at the top of the data structure
{
    if(*ceil != max)    //if the max size is not reached, continue adding
    {
        *ceil = *ceil + 1;
        stack[*ceil] = character;
    }
    else
    {
        printf("Max limit reached for stack.\n");
    }
}

char stackPop(int stack[51], int * ceil)    //gives us the last value in the data structure
{
    int character = stack[*ceil];
    *ceil = *ceil - 1;
    return character;
}
