#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

// Struktura za jedan artikal
typedef struct article {
    char name[20];
    int quantity;
    int price;
    struct article* next;
} Article;

// Struktura za jedan račun
typedef struct receipt {
    char date[20];
    Article* headArticle;    // pokazivač na prvu stavku artikala
    struct receipt* next;
} Receipt;

// Deklaracije svih funkcija
int addArticle(Article* head, Article* article);
int addReceipt(Receipt* head, Receipt* receipt);
int readReceipt(char* fileName, Receipt* headReceipt);
int printArticles(Article* article);
int printReceipts(Receipt* receipt);
int getArticleInPeriod(Receipt* currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);
int deleteArticles(Article* articleHead);
int deleteReceipts(Receipt* receiptHead);

int main()
{
    int income = 0, count = 0;
    char articleName[50];
    char startDate[20], endDate[20];
    char fileLine[MAX_LINE];
    FILE* fp = NULL;

    // Glava liste računa
    Receipt headReceipt = { .date = "", .headArticle = NULL, .next = NULL };

    // Otvaranje glavne datoteke s popisom računa
    fp = fopen("racuni.txt", "r");
    if (!fp) {
        printf("Ne mogu otvoriti racuni.txt\n");
        return FILE_OPEN_ERROR;
    }

    // Svaka linija predstavlja ime datoteke jednog računa
    while (fgets(fileLine, MAX_LINE, fp)) {
        fileLine[strcspn(fileLine, "\n")] = 0;   // makni \n
        readReceipt(fileLine, &headReceipt);
    }
    fclose(fp);

    // Ispis svih računa
    printReceipts(headReceipt.next);

    // Unos kriterija
    printf("Naziv artikla: ");
    scanf("%s", articleName);

    printf("Početni datum (YYYY-MM-DD): ");
    scanf("%s", startDate);

    printf("Završni datum (YYYY-MM-DD): ");
    scanf("%s", endDate);

    // Računanje prihoda artikla u zadanom periodu
    getArticleInPeriod(headReceipt.next, articleName, startDate, endDate, &income, &count);

    printf("Ukupni prihod za %d prodanih '%s': %d\n", count, articleName, income);

    // Brisanje memorije
    deleteReceipts(&headReceipt);

    return 0;
}


// Dodaje artikal sortirano po imenu
int addArticle(Article* head, Article* article)
{
    Article* current = head;

    // Traženje mjesta za umetanje
    while (current->next != NULL && strcmp(current->next->name, article->name) < 0)
        current = current->next;

    // Umetanje artikla
    article->next = current->next;
    current->next = article;

    return 0;
}


// Dodaje račun sortirano po datumu
int addReceipt(Receipt* head, Receipt* receipt)
{
    Receipt* current = head;

    // Pronalaženje mjesta za umetanje
    while (current->next != NULL && strcmp(current->next->date, receipt->date) < 0)
        current = current->next;

    receipt->next = current->next;
    current->next = receipt;

    return 0;
}


// Učitava jedan račun iz njegove datoteke
int readReceipt(char* fileName, Receipt* headReceipt)
{
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("Ne mogu otvoriti %s\n", fileName);
        return FILE_OPEN_ERROR;
    }

    // Alokacija novog računa
    Receipt* newReceipt = malloc(sizeof(Receipt));
    if (!newReceipt) return MALLOC_ERROR;

    // Prvi red = datum
    fgets(newReceipt->date, 20, fp);
    newReceipt->date[strcspn(newReceipt->date, "\n")] = 0;

    Article articleHead = { .next = NULL };
    Article* newArticle = NULL;

    // Čitanje artikala
    while (!feof(fp)) {
        newArticle = malloc(sizeof(Article));
        if (!newArticle) return MALLOC_ERROR;

        if (fscanf(fp, "%[^,], %d, %d ",
            newArticle->name,
            &newArticle->quantity,
            &newArticle->price) != 3)
        {
            free(newArticle);
            break;
        }

        addArticle(&articleHead, newArticle);
    }

    fclose(fp);

    newReceipt->headArticle = articleHead.next;
    newReceipt->next = NULL;

    // Dodaj račun u sortiranu listu
    addReceipt(headReceipt, newReceipt);

    return EXIT_SUCCESS;
}


// Ispis svih artikala u jednom računu
int printArticles(Article* current)
{
    while (current) {
        printf("\t%s, %d kom, %d EUR\n", current->name, current->quantity, current->price);
        current = current->next;
    }
    return 0;
}


// Ispis svih računa
int printReceipts(Receipt* current)
{
    while (current) {
        printf("\nRačun od %s:\n", current->date);
        printArticles(current->headArticle);
        current = current->next;
    }
    return 0;
}


// Računa prihod određenog artikla u zadanom periodu
int getArticleInPeriod(Receipt* current, char* article, char* start, char* end, int* income, int* count)
{
    while (current) {

		//provjera da li je datum računa u zadanom periodu
        if (strcmp(current->date, start) >= 0 && strcmp(current->date, end) <= 0) {

            Article* a = current->headArticle;

            while (a) {
                if (strcmp(a->name, article) == 0) {
                    *income += a->quantity * a->price;
                    *count += a->quantity;
                }
                a = a->next;
            }
        }

        current = current->next;
    }

    return EXIT_SUCCESS;
}


// Briše sve artikle iz jedne liste artikala
int deleteArticles(Article* articleHead)
{
    Article* temp = NULL;

    while (articleHead->next != NULL) {
        temp = articleHead->next;
        articleHead->next = temp->next;
        free(temp);
    }
    return 0;
}


// Briše sve račune i sve njihove artikle
int deleteReceipts(Receipt* receiptHead)
{
    Receipt* temp = NULL;

    while (receiptHead->next != NULL) {
        temp = receiptHead->next;
        receiptHead->next = temp->next;

        // Brisanje artikala tog računa
        Article dummy = { .next = temp->headArticle };
        deleteArticles(&dummy);

        free(temp);
    }
    return 0;
}
