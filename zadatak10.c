#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 20
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

// Stablo gradova (sortirano po broju stanovnika pa po imenu)
typedef struct CityTree {
    char name[MAX_NAME];
    int population;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

// Vezana lista drzava
typedef struct CountryList {
    char name[MAX_NAME];
    CityTree* cities;              // stablo gradova
    struct CountryList* next;
} CountryList;

// Stablo drzava
typedef struct CountryTree {
    char name[MAX_NAME];
    CityTree* cities;              // stablo gradova
    struct CountryTree* left;
    struct CountryTree* right;
} CountryTree;

CityTree* addCity(CityTree* root, CityTree* newCity);
int printCityTree(CityTree* root);
int printCitiesGreater(CityTree* root, int limit);
int freeCityTree(CityTree* root);
int addCountryToList(CountryList* head, CountryList* newCountry);
int printCountryList(CountryList* head);
int freeCountryList(CountryList* head);
CountryTree* addCountryToTree(CountryTree* root, CountryTree* newCountry);
CountryTree* findCountry(CountryTree* root, char* name);
int printCountryTree(CountryTree* root);
int freeCountryTree(CountryTree* root);

int main()
{
    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Ne mogu otvoriti drzave.txt\n");
        return FILE_OPEN_ERROR;
    }

    // Glava vezane liste drzava
    CountryList listHead = { "", NULL, NULL };
    CountryTree* treeRoot = NULL;

    char country[MAX_NAME], fileName[30];
    char city[MAX_NAME];
    int population;

    // UCITAVANJE DRZAVA
    while (fscanf(fp, " %[^,], %s", country, fileName) == 2)
    {
        // Alokacija drzave za listu
        CountryList* cl = (CountryList*)malloc(sizeof(CountryList));
        if (!cl) return MALLOC_ERROR;

        // Alokacija drzave za stablo
        CountryTree* ct = (CountryTree*)malloc(sizeof(CountryTree));
        if (!ct) return MALLOC_ERROR;

        strcpy(cl->name, country);
        cl->cities = NULL;
        cl->next = NULL;

        strcpy(ct->name, country);
        ct->cities = NULL;
        ct->left = ct->right = NULL;

        addCountryToList(&listHead, cl);
        treeRoot = addCountryToTree(treeRoot, ct);

        FILE* cf = fopen(fileName, "r");
        if (!cf) continue;

        // UCITAVANJE GRADOVA
        while (fscanf(cf, " %[^,], %d", city, &population) == 2)
        {
            // Grad za listu drzava
            CityTree* c1 = (CityTree*)malloc(sizeof(CityTree));
            // Grad za stablo drzava
            CityTree* c2 = (CityTree*)malloc(sizeof(CityTree));

            if (!c1 || !c2) return MALLOC_ERROR;

            strcpy(c1->name, city);
            c1->population = population;
            c1->left = c1->right = NULL;

            strcpy(c2->name, city);
            c2->population = population;
            c2->left = c2->right = NULL;

            // Dodavanje grada u stabla
            cl->cities = addCity(cl->cities, c1);
            ct->cities = addCity(ct->cities, c2);
        }
        fclose(cf);
    }
    fclose(fp);

    // ISPIS 
    printf("\n--- LISTA DRZAVA ---\n");
    printCountryList(listHead.next);

    printf("\n--- STABLO DRZAVA ---\n");
    printCountryTree(treeRoot);

    // PRETRAGA
    char choice[MAX_NAME];
    int limit;

    while (1) {
        printf("\nUnesite ime drzave (0 za izlaz): ");
        scanf(" %19[^\n]", choice);

        if (strcmp(choice, "0") == 0)
            break;

        CountryTree* found = findCountry(treeRoot, choice);
        if (!found) {
            printf("Drzava ne postoji.\n");
            continue;
        }

        printf("Unesite minimalan broj stanovnika: ");
        scanf("%d", &limit);

        printf("Gradovi s vise od %d stanovnika:\n", limit);
        printCitiesGreater(found->cities, limit);
    }

    // BRISANJE MEMORIJE
    freeCountryList(listHead.next);
    freeCountryTree(treeRoot);

    return EXIT_SUCCESS;
}

// IMPLEMENTACIJE FUNKCIJA

// Dodavanje grada u stablo (sort po stanovnicima pa po imenu)
CityTree* addCity(CityTree* root, CityTree* newCity)
{
    if (!root)
        return newCity;

    if (newCity->population < root->population ||
        (newCity->population == root->population &&
            strcmp(newCity->name, root->name) < 0))
        root->left = addCity(root->left, newCity);
    else
        root->right = addCity(root->right, newCity);

    return root;
}

// Inorder ispis stabla gradova
int printCityTree(CityTree* root)
{
    if (!root) return EXIT_SUCCESS;

    printCityTree(root->left);
    printf("\t%s (%d)\n", root->name, root->population);
    printCityTree(root->right);

    return EXIT_SUCCESS;
}

// Ispis gradova s vise stanovnika od zadanog broja
int printCitiesGreater(CityTree* root, int limit)
{
    if (!root) return EXIT_SUCCESS;

    printCitiesGreater(root->right, limit);
    if (root->population > limit)
        printf("\t%s (%d)\n", root->name, root->population);
    printCitiesGreater(root->left, limit);

    return EXIT_SUCCESS;
}

// Postorder brisanje stabla gradova
int freeCityTree(CityTree* root)
{
    if (!root) return EXIT_SUCCESS;

    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);

    return EXIT_SUCCESS;
}

// Dodavanje drzave u sortiranu vezanu listu
int addCountryToList(CountryList* head, CountryList* newCountry)
{
    while (head->next && strcmp(newCountry->name, head->next->name) > 0)
        head = head->next;

    newCountry->next = head->next;
    head->next = newCountry;

    return EXIT_SUCCESS;
}

// Ispis vezane liste drzava
int printCountryList(CountryList* head)
{
    while (head) {
        printf("%s\n", head->name);
        printCityTree(head->cities);
        head = head->next;
    }
    return EXIT_SUCCESS;
}

// Brisanje vezane liste drzava
int freeCountryList(CountryList* head)
{
    CountryList* temp = NULL;

    while (head) {
        temp = head;
        head = head->next;

        freeCityTree(temp->cities);
        free(temp);
    }
    return EXIT_SUCCESS;
}

// Dodavanje drzave u stablo
CountryTree* addCountryToTree(CountryTree* root, CountryTree* newCountry)
{
    if (!root)
        return newCountry;

    if (strcmp(newCountry->name, root->name) < 0)
        root->left = addCountryToTree(root->left, newCountry);
    else if (strcmp(newCountry->name, root->name) > 0)
        root->right = addCountryToTree(root->right, newCountry);

    return root;
}

// Pretraga drzave u stablu
CountryTree* findCountry(CountryTree* root, char* name)
{
    if (!root) return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp < 0) return findCountry(root->left, name);
    if (cmp > 0) return findCountry(root->right, name);

    return root;
}

// Inorder ispis stabla drzava
int printCountryTree(CountryTree* root)
{
    if (!root) return EXIT_SUCCESS;

    printCountryTree(root->left);
    printf("%s\n", root->name);
    printCityTree(root->cities);
    printCountryTree(root->right);

    return EXIT_SUCCESS;
}

// Brisanje stabla drzava
int freeCountryTree(CountryTree* root)
{
    if (!root) return EXIT_SUCCESS;

    freeCountryTree(root->left);
    freeCountryTree(root->right);

    freeCityTree(root->cities);
    free(root);

    return EXIT_SUCCESS;
}

