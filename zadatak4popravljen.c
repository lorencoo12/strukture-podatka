#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Struktura jednog ?lana polinoma
typedef struct Poly {
    int coef;            // koeficijent ?lana
    int exp;             // eksponent ?lana
    struct Poly* next;   // pokaziva? na sljede?i ?lan
} Poly;

// Funkcije koje koristimo
Poly* createNode(int coef, int exp);
void insertSorted(Poly* head, int coef, int exp);
void readFromFile(Poly* head, const char* filename);
void printPoly(Poly* head);
void sumPoly(Poly* head, Poly* p1, Poly* p2);
void multiplyPoly(Poly* head, Poly* p1, Poly* p2);

int main() {
    // Kreiramo "glave" polinoma (dummy head) da lakše radimo s listom
    Poly head1 = { 0, 0, NULL };       // polinom 1
    Poly head2 = { 0, 0, NULL };       // polinom 2
    Poly sumHead = { 0, 0, NULL };     // polinom suma
    Poly productHead = { 0, 0, NULL }; // polinom produkt

    // ?itanje polinoma iz datoteka
    readFromFile(&head1, "pol1.txt");
    readFromFile(&head2, "pol2.txt");

    // Ispis polinoma
    printf("Polynomial 1: ");
    printPoly(&head1);
    printf("Polynomial 2: ");
    printPoly(&head2);

    // Zbrajanje polinoma
    sumPoly(&sumHead, &head1, &head2);
    printf("Sum: ");
    printPoly(&sumHead);

    // Množenje polinoma
    multiplyPoly(&productHead, &head1, &head2);
    printf("Product: ");
    printPoly(&productHead);

    return 0;
}

// Funkcija koja stvara novi ?vor polinoma
Poly* createNode(int coef, int exp) {
    Poly* node = (Poly*)malloc(sizeof(Poly)); // alociramo memoriju
    node->coef = coef;    // postavljamo koeficijent
    node->exp = exp;      // postavljamo eksponent
    node->next = NULL;    // next pokaziva? je NULL
    return node;
}

// Funkcija za umetanje ?lana u listu sortiran po eksponentu (rastuci)
void insertSorted(Poly* head, int coef, int exp) {
    Poly* temp = head;

    // Prona?i mjesto gdje eksponent sljede?eg ?lana >= novog eksponenta
    while (temp->next != NULL && temp->next->exp < exp)
        temp = temp->next;

    // Ako postoji ?lan sa istim eksponentom ? saberi koeficijente
    if (temp->next != NULL && temp->next->exp == exp) {
        temp->next->coef += coef;

        // Ako koeficijent postane 0 ? briši ?lan
        if (temp->next->coef == 0) {
            Poly* toDelete = temp->next;
            temp->next = temp->next->next;
            free(toDelete);
        }
    }
    else {
        // Ina?e umetni novi ?lan
        Poly* node = createNode(coef, exp);
        node->next = temp->next;
        temp->next = node;
    }
}

// Funkcija za ?itanje polinoma iz datoteke
// Datoteka format: koef eks koef eks ...
void readFromFile(Poly* head, const char* filename) {
    FILE* fp = fopen(filename, "r");   // otvaramo datoteku
    if (!fp) {                         // provjera da li je otvorena
        printf("Cannot open file %s\n", filename);
        return;
    }

    int coef, exp;
    // ?itamo parove (koeficijent i eksponent) dok ih ima
    while (fscanf(fp, "%d %d", &coef, &exp) == 2) {
        insertSorted(head, coef, exp); // umetni svaki ?lan u listu
    }

    fclose(fp); // zatvaramo datoteku
}

// Funkcija za ispis polinoma
void printPoly(Poly* head) {
    Poly* temp = head->next;  // preska?emo dummy head
    if (!temp) {               // ako je polinom prazan
        printf("0\n");
        return;
    }

    while (temp) {
        printf("%d^%d", temp->coef, temp->exp); // ispiši ?lan
        if (temp->next) printf(" + ");          // dodaj + ako ima sljede?i
        temp = temp->next;
    }
    printf("\n"); // novi red nakon ispisa
}

// Funkcija za zbrajanje dvaju polinoma
void sumPoly(Poly* head, Poly* p1, Poly* p2) {
    Poly* t1 = p1->next;
    Poly* t2 = p2->next;

    // Dodaj sve ?lanove prvog polinoma
    while (t1) {
        insertSorted(head, t1->coef, t1->exp);
        t1 = t1->next;
    }

    // Dodaj sve ?lanove drugog polinoma
    while (t2) {
        insertSorted(head, t2->coef, t2->exp);
        t2 = t2->next;
    }
}

// Funkcija za množenje dvaju polinoma
void multiplyPoly(Poly* head, Poly* p1, Poly* p2) {
    Poly* t1 = p1->next;

    while (t1) {            // za svaki ?lan prvog polinoma
        Poly* t2 = p2->next;
        while (t2) {        // množi ga sa svim ?lanovima drugog
            insertSorted(head, t1->coef * t2->coef, t1->exp + t2->exp);
            t2 = t2->next;
        }
        t1 = t1->next;
    }
}

