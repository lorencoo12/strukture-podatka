#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struktura čvora binarnog stabla pretraživanja (BST)
typedef struct _node {
    int value;              // Vrijednost pohranjena u čvoru
    struct _node* left;     // Pokazivač na lijevo dijete
    struct _node* right;    // Pokazivač na desno dijete
} Node;

Node* insert(Node* root, int value);
int replace(Node* root);
int inorder(Node* root, FILE* fp);
int freeTree(Node* root);

// a) INSERT – umetanje elementa u BST
Node* insert(Node* root, int value)
{
    // Ako je stablo prazno, stvara se novi čvor
    if (root == NULL) {
        root = (Node*)malloc(sizeof(Node));   // Alokacija memorije
        root->value = value;                  // Postavljanje vrijednosti
        root->left = NULL;                    // Nema djece
        root->right = NULL;
    }
    // Ako je vrijednost manja, ide u lijevo podstablo
    else if (value < root->value) {
        root->left = insert(root->left, value);
    }
    // Inače ide u desno podstablo
    else {
        root->right = insert(root->right, value);
    }

    // Vraća (potencijalno novi) korijen stabla
    return root;
}

// b) REPLACE – zamjenjuje vrijednost čvora sa sumom njegovih podstabala
// Funkcija vraća ukupnu sumu podstabla uključujući originalnu vrijednost čvora
int replace(Node* root)
{
    int leftSum = 0;     // Suma lijevog podstabla
    int rightSum = 0;    // Suma desnog podstabla
    int oldValue = 0;    // Originalna vrijednost čvora

    // Ako je čvor NULL, suma je 0
    if (root == NULL)
        return 0;

    // Rekurzivno računanje suma lijevog i desnog podstabla
    leftSum = replace(root->left);
    rightSum = replace(root->right);

    // Spremanje stare vrijednosti čvora
    oldValue = root->value;

    // Zamjena vrijednosti čvora sumom podstabala
    root->value = leftSum + rightSum;

    // Vraća ukupnu sumu (nova vrijednost + stara vrijednost čvora)
    return root->value + oldValue;
}

// Inorder obilazak stabla (lijevo – korijen – desno)
// Istovremeno ispisuje vrijednosti u datoteku
int inorder(Node* root, FILE* fp)
{
    int count = 0;   // Brojač ispisanih elemenata

    // Ako je stablo prazno, nema ispisa
    if (root == NULL)
        return 0;

    // Obilazak lijevog podstabla
    count += inorder(root->left, fp);

    // Ispis trenutnog čvora u datoteku
    fprintf(fp, "%d ", root->value);
    count++;

    // Obilazak desnog podstabla
    count += inorder(root->right, fp);

    // Vraća broj ispisanih elemenata
    return count;
}

// Oslobađanje memorije zauzete stablom (postorder obilazak)
int freeTree(Node* root)
{
    // Ako je čvor NULL, nema što oslobađati
    if (root == NULL)
        return 0;

    // Prvo se oslobađaju djeca
    freeTree(root->left);
    freeTree(root->right);

    // Oslobađanje trenutnog čvora
    free(root);

    return 1;
}

int main()
{
    Node* root = NULL;   // Korijen stabla
    int i, n = 10;       // Broj elemenata
    int numbers[10];     // Polje slučajnih brojeva
    FILE* fp = NULL;    // Pokazivač na datoteku

    // Inicijalizacija generatora slučajnih brojeva
    srand((unsigned)time(NULL));

    // c) Generiranje 10 slučajnih brojeva u rasponu <10, 90>
    // i njihovo umetanje u BST
    for (i = 0; i < n; i++) {
        numbers[i] = rand() % 81 + 10;
        root = insert(root, numbers[i]);
    }

    // Otvaranje datoteke za pisanje
    fp = fopen("inorder.txt", "w");
    if (!fp) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    // Inorder ispis nakon umetanja elemenata
    fprintf(fp, "Inorder nakon inserta:\n");
    inorder(root, fp);

    // Poziv replace funkcije
    replace(root);

    // Inorder ispis nakon zamjene vrijednosti čvorova
    fprintf(fp, "\nInorder nakon replace:\n");
    inorder(root, fp);

    // Zatvaranje datoteke i oslobađanje memorije
    fclose(fp);
    freeTree(root);

    return 0;
}
