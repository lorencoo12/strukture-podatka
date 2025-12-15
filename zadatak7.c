#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50              

#define EMPTY_STACK 1              
#define EXIT_SUCCESS 0             
#define MALLOC_ERROR -1            
#define SCANF_ERROR -2             


// Struktura koja opisuje direktorij
struct directory;
typedef struct directory* DirectoryPosition;
typedef struct directory {
	char name[MAX_LENGTH];                  // ime direktorija
	DirectoryPosition subdirectoryPosition; // prvi poddirektorij
	DirectoryPosition next;                 // sljede?i direktorij na istoj razini
} Directory;

// Struktura stoga za pam?enje putanje (cd i cd..)
struct levelStack;
typedef struct levelStack* LevelStackPosition;
typedef struct levelStack {
	DirectoryPosition directoryLevel; // direktorij na toj razini
	LevelStackPosition next;           // sljede?i element stoga
} LevelStack;


DirectoryPosition createDirectory(char* directoryName);  
int push(LevelStackPosition, DirectoryPosition);          // dodaje direktorij na stog
int pop(LevelStackPosition);                              // skida direktorij sa stoga
int addDirectory(DirectoryPosition, DirectoryPosition);   
int changeDirectory(LevelStackPosition, char*);           
int printDirectory(DirectoryPosition, int);               
int deleteDirectory(DirectoryPosition);                   
int deleteStack(LevelStackPosition);                      


int main()
{
	char command = 0;                          // korisni?ka naredba (1-5)
	char directoryName[MAX_LENGTH] = { 0 };    // ime direktorija

	// lažna glava za direktorije
	Directory headDirectory = { .name = {0}, .subdirectoryPosition = NULL, .next = NULL };

	// lažna glava stoga
	LevelStack headStack = { .directoryLevel = NULL, .next = NULL };

	// stvaranje root direktorija
	DirectoryPosition root = createDirectory("C:");
	if (root == NULL) return MALLOC_ERROR;

	headDirectory.next = root;     // root je prvi direktorij
	push(&headStack, root);        // root ide na stog

	// ispis izbornika
	printf("1 - make directory\n");
	printf("2 - change directory\n");
	printf("3 - go to parent directory\n");
	printf("4 - view directory content\n");
	printf("5 - end program\n");

	do {
		printf("Enter command: ");
		if (scanf(" %c", &command) != 1) return SCANF_ERROR;

		switch (command) {

		case '1':   // md - make directory
			printf("md ");
			if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;

			DirectoryPosition newDir = createDirectory(directoryName);
			if (newDir == NULL) return MALLOC_ERROR;

			// dodavanje direktorija u trenutni direktorij
			addDirectory(headStack.next->directoryLevel, newDir);
			printf("Created directory %s\n", directoryName);
			break;

		case '2':   // cd <dir>
			printf("cd ");
			if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;

			changeDirectory(&headStack, directoryName);
			break;

		case '3':   // cd..
			if (pop(&headStack))
				printf("Already in root directory.\n");
			else
				printf("Moved to parent directory.\n");
			break;

		case '4':   // dir - ispis sadržaja
			printf("Content of %s:\n", headStack.next->directoryLevel->name);
			printDirectory(headStack.next->directoryLevel, 0);
			break;

		case '5':   // kraj programa
			printf("End of program.\n");
			break;

		default:
			printf("Invalid command.\n");
			break;
		}

	} while (command != '5');

	// osloba?anje memorije
	deleteDirectory(&headDirectory);
	deleteStack(&headStack);

	return EXIT_SUCCESS;
}


// Stvara i inicijalizira novi direktorij
DirectoryPosition createDirectory(char* directoryName)
{
	DirectoryPosition newDir = malloc(sizeof(Directory));
	if (newDir == NULL) return NULL;

	strcpy(newDir->name, directoryName);          // kopiranje imena
	newDir->subdirectoryPosition = NULL;          // nema poddirektorija
	newDir->next = NULL;                          // nema sljede?eg

	return newDir;
}

// Dodaje direktorij na vrh stoga
int push(LevelStackPosition head, DirectoryPosition current)
{
	LevelStackPosition newElement = malloc(sizeof(LevelStack));
	if (newElement == NULL) return MALLOC_ERROR;

	newElement->directoryLevel = current;         // spremi direktorij
	newElement->next = head->next;                // poveži sa starim vrhom
	head->next = newElement;                      // novi vrh stoga

	return EXIT_SUCCESS;
}

// Skida jedan element sa stoga
int pop(LevelStackPosition head)
{
	if (head->next == NULL)
		return EMPTY_STACK;                         // nema više direktorija

	LevelStackPosition temp = head->next;         // spremi vrh
	head->next = temp->next;                      // pomakni vrh
	free(temp);                                   // oslobodi memoriju

	return EXIT_SUCCESS;
}

// Dodaje poddirektorij u nad-direktorij
int addDirectory(DirectoryPosition parent, DirectoryPosition child)
{
	child->next = parent->subdirectoryPosition;   // veži postoje?e
	parent->subdirectoryPosition = child;         // novi je prvi

	return EXIT_SUCCESS;
}

// Ulazak u poddirektorij (cd)
int changeDirectory(LevelStackPosition head, char* directoryName)
{
	DirectoryPosition current = head->next->directoryLevel->subdirectoryPosition;

	// traženje direktorija po imenu
	while (current != NULL && strcmp(current->name, directoryName) != 0)
		current = current->next;

	if (current != NULL) {
		push(head, current);                        // dodaj na stog
		printf("Changed directory to %s\n", directoryName);
	}
	else {
		printf("Directory not found.\n");
	}

	return EXIT_SUCCESS;
}

// Rekurzivni ispis direktorija
int printDirectory(DirectoryPosition current, int depth)
{
	DirectoryPosition temp = current->subdirectoryPosition;

	while (temp != NULL) {
		for (int i = 0; i < depth; i++)
			printf(" ");                          // uvlaka

		printf("%s\n", temp->name);            // ispis imena
		printDirectory(temp, depth + 1);           // ispis poddirektorija
		temp = temp->next;                         // sljede?i
	}

	return EXIT_SUCCESS;
}

// Briše sve direktorije
int deleteDirectory(DirectoryPosition head)
{
	while (head->next != NULL) {
		DirectoryPosition temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

// Briše cijeli stog
int deleteStack(LevelStackPosition head)
{
	while (head->next != NULL) {
		LevelStackPosition temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

