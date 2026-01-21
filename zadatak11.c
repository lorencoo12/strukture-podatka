#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 20
#define TABLE_SIZE 11
#define FILE_ERROR -1
#define MALLOC_ERROR -2
#define SUCCESS 0

// Struktura stabla gradova
typedef struct _City {
    char name[MAX_NAME];
    int population;
    struct _City* left;
    struct _City* right;
} City;

// Struktura drzave u hash tablici (vezana lista)
typedef struct _Country {
    char name[MAX_NAME];
    City* cities;
    struct _Country* next;
} Country;

// Hash tablica
typedef Country* HashTable[TABLE_SIZE];

// Prototipi funkcija
int hashFunction(char* name);

City* createCity(char* name, int population);
City* insertCity(City* root, City* newCity);
int printCityTree(City* root);
int printCitiesAboveLimit(City* root, int limit);
int freeCityTree(City* root);

Country* createCountry(char* name);
int insertCountry(HashTable table, Country* newCountry);
Country* findCountry(HashTable table, char* name);
int printHashTable(HashTable table);
int freeHashTable(HashTable table);

int main()
{
    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Greska pri otvaranju drzave.txt\n");
        return FILE_ERROR;
    }

    HashTable table = { NULL };

    char country[MAX_NAME];
    char city[MAX_NAME];
    char fileName[30];
    int population;

    // Ucitavanje drzava
    while (fscanf(fp, " %[^,], %s", country, fileName) == 2) {

        Country* c = createCountry(country);
        if (!c)
            return MALLOC_ERROR;

        insertCountry(table, c);

        FILE* cf = fopen(fileName, "r");
        if (!cf)
            continue;

        // Ucitavanje gradova
        while (fscanf(cf, " %[^,], %d", city, &population) == 2) {

            City* newCity = createCity(city, population);
            if (!newCity)
                return MALLOC_ERROR;

            c->cities = insertCity(c->cities, newCity);
        }

        fclose(cf);
    }

    fclose(fp);

    // Ispis hash tablice
    printf("\n--- HASH TABLICA DRZAVA ---\n");
    printHashTable(table);

    // Pretraga
    char search[MAX_NAME];
    int limit;

    while (1) {
        printf("\nUnesite ime drzave (0 za izlaz): ");
        scanf(" %19[^\n]", search);

        if (!strcmp(search, "0"))
            break;

        Country* found = findCountry(table, search);
        if (!found) {
            printf("Drzava ne postoji.\n");
            continue;
        }

        printf("Unesite minimalan broj stanovnika: ");
        scanf("%d", &limit);

        printCitiesAboveLimit(found->cities, limit);
    }

    // Oslobadanje memorije
    freeHashTable(table);

    return SUCCESS;
}

// Hash funkcija
int hashFunction(char* name)
{
    int sum = 0;
    int i = 0;

    while (name[i] && i < 5) {
        sum += (int)name[i];
        i++;
    }

    return sum % TABLE_SIZE;
}

// Kreiranje grada
City* createCity(char* name, int population)
{
    City* c = (City*)malloc(sizeof(City));
    if (!c)
        return NULL;

    strcpy(c->name, name);
    c->population = population;
    c->left = c->right = NULL;

    return c;
}

// Dodavanje grada u stablo
City* insertCity(City* root, City* newCity)
{
    if (!root)
        return newCity;

    if (newCity->population < root->population ||
        (newCity->population == root->population &&
            strcmp(newCity->name, root->name) < 0))
        root->left = insertCity(root->left, newCity);
    else
        root->right = insertCity(root->right, newCity);

    return root;
}

// Inorder ispis stabla gradova
int printCityTree(City* root)
{
    if (!root)
        return SUCCESS;

    printCityTree(root->left);
    printf("\t%s (%d)\n", root->name, root->population);
    printCityTree(root->right);

    return SUCCESS;
}

// Ispis gradova iznad granice
int printCitiesAboveLimit(City* root, int limit)
{
    if (!root)
        return SUCCESS;

    printCitiesAboveLimit(root->right, limit);

    if (root->population > limit)
        printf("\t%s (%d)\n", root->name, root->population);

    printCitiesAboveLimit(root->left, limit);

    return SUCCESS;
}

// Brisanje stabla gradova
int freeCityTree(City* root)
{
    if (!root)
        return SUCCESS;

    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);

    return SUCCESS;
}

// Kreiranje drzave
Country* createCountry(char* name)
{
    Country* c = (Country*)malloc(sizeof(Country));
    if (!c)
        return NULL;

    strcpy(c->name, name);
    c->cities = NULL;
    c->next = NULL;

    return c;
}

// Dodavanje drzave u hash tablicu (sortirana lista)
int insertCountry(HashTable table, Country* newCountry)
{
    int index = hashFunction(newCountry->name);
    Country* current = table[index];

    if (!current || strcmp(newCountry->name, current->name) < 0) {
        newCountry->next = current;
        table[index] = newCountry;
        return SUCCESS;
    }

    while (current->next &&
        strcmp(newCountry->name, current->next->name) > 0)
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;

    return SUCCESS;
}

// Pretraga drzave u hash tablici
Country* findCountry(HashTable table, char* name)
{
    int index = hashFunction(name);
    Country* current = table[index];

    while (current) {
        if (!strcmp(current->name, name))
            return current;
        current = current->next;
    }

    return NULL;
}

// Ispis hash tablice
int printHashTable(HashTable table)
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("[%d]\n", i);

        Country* current = table[i];
        while (current) {
            printf("%s\n", current->name);
            printCityTree(current->cities);
            current = current->next;
        }
    }
    return SUCCESS;
}

// Brisanje hash tablice
int freeHashTable(HashTable table)
{
    for (int i = 0; i < TABLE_SIZE; i++) {

        Country* current = table[i];
        Country* temp = NULL;

        while (current) {
            temp = current;
            current = current->next;
            freeCityTree(temp->cities);
            free(temp);
        }
    }
    return SUCCESS;
}
