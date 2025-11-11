#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define INVALID_INPUT 2
#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2
#define SCANF_ERROR -3

typedef struct stackItem {
	float value;
	struct stackItem* next;
} StackItem;

typedef struct {
	StackItem* top;
} Stack;

int push(Stack* stack, float x);
int pop(Stack* stack, float* x);

int main()
{
	int size = 0;
	float firstOperand = 0, secondOperand = 0, result = 0;
	char currItem[10] = { 0 };
	char postfix[MAX_LINE] = { 0 };
	char* postfixPointer = postfix;
	FILE* filePointer = NULL;
	Stack stack = { .top = NULL };

	filePointer = fopen("postfix.txt", "r");
	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_OPEN_ERROR;
	}

	fgets(postfix, MAX_LINE, filePointer);
	printf("Postfix: %s\n", postfix);

	while (strlen(postfixPointer) > 0)
	{
		if (sscanf(postfixPointer, " %s %n ", currItem, &size) != 1) {
			printf("sscanf error.\n");
			return SCANF_ERROR;
		}

		// currItem is a number
		if (currItem[0] >= '0' && currItem[0] <= '9') {
			if (push(&stack, atof(currItem))) return MALLOC_ERROR;
		}

		else if (currItem[0] == '+' || currItem[0] == '-' || currItem[0] == '*' || currItem[0] == '/')
		{
			if (pop(&stack, &secondOperand) || pop(&stack, &firstOperand)) {
				printf("Trying to read from empty stack.\n");
				return EMPTY_STACK;
			}

			if (currItem[0] == '+') {
				if (push(&stack, firstOperand + secondOperand)) return MALLOC_ERROR;
			}

			else if (currItem[0] == '-') {
				if (push(&stack, firstOperand - secondOperand)) return MALLOC_ERROR;
			}

			else if (currItem[0] == '*') {
				if (push(&stack, firstOperand * secondOperand)) return MALLOC_ERROR;
			}

			else {
				if (push(&stack, firstOperand / secondOperand)) return MALLOC_ERROR;
			}
		}

		else {
			printf("Invalid character in postfix.\n");
			return INVALID_INPUT;
		}

		postfixPointer += size;
	}

	if (pop(&stack, &result)) return EMPTY_STACK;

	printf("Result: %g\n", result);

	return 0;
}

int push(Stack* stack, float x)
{
	StackItem* item = NULL;
	item = (StackItem*)malloc(sizeof(StackItem));
	if (item == NULL) {
		printf("Malloc error!\n");
		return MALLOC_ERROR;
	}

	item->value = x;

	item->next = stack->top;
	stack->top = item;

	return EXIT_SUCCESS;
}

int pop(Stack* stack, float* x)
{
	StackItem* top;
	top = stack->top;
	if (top == NULL) {
		printf("Trying to read from empty stack.\n");
		return EMPTY_STACK;
	}

	*x = top->value;

	stack->top = top->next;
	free(top);

	return EXIT_SUCCESS;
}