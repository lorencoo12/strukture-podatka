#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024
struct Osoba;
typedef struct Osoba* pozicija;


typedef struct Osoba
{
    char ime[MAX];
    char prez[MAX];
    int god;
    pozicija next;
}osoba;


pozicija kreirajosobu();
void unospocetak(pozicija);
void unoskraj(pozicija);
void ispis(pozicija);
void pretrazi(pozicija, char[MAX]);
void brisi(pozicija, char[MAX]);
void izbrisilistu(pozicija);


int main()
{
    int izbor;
    char prezime[MAX];
    pozicija head;
    head = (pozicija)malloc(sizeof(osoba));

    if (head == NULL) {
        printf("Error allocating memory \n");
        return -1;
    }
    head->next = NULL;
    do
    {
        printf("Izaberi 1 za dodat novi element na pocetak liste\n");
        printf("Izaberi 2 za ispis liste\n");
        printf("Izaberi 3 za dodat novi element na kraj liste\n");
        printf("Izaberi 4 za pronalazak elementa po prezimenu\n");
        printf("Izaberi 5 za brisanje odredenog elementa\n Izaberi 6 za kraj \n");
        scanf("%d", &izbor);

        switch (izbor)
        {
        case 1:
            unospocetak(head);
            break;
        case 2:
            ispis(head);
            break;
        case 3:
            unoskraj(head);
            break;
        case 4:
            printf("unesi trazeno prezime: \n");
            scanf("%s", prezime);
            pretrazi(head, prezime);
            break;
        case 5:
            printf("unesi prezime osobe koju zelis izbrisati \n ");
            scanf("%s", prezime);
            brisi(head, prezime);
            break;
        case 6:

            break;
        default:
            printf("\tGreska\n");
            break;
        }
    } while (izbor != 6);

    izbrisilistu(head);


    return 0;
}

pozicija kreirajosobu()
{
    pozicija os = NULL;
    os = (pozicija)malloc(sizeof(osoba));
    if (!os) return NULL;
    printf("unesi ime ");
    scanf("%s", os->ime);
    printf("unesi prezime ");
    scanf("%s", os->prez);
    printf("unesi godinu ");
    scanf("%d", &os->god);

    return os;


}


void unospocetak(pozicija head)
{
    pozicija novi;
    pozicija prvi = head->next;
    novi = kreirajosobu();
    head->next = novi;
    novi->next = prvi;



}

void unoskraj(pozicija head)
{
    pozicija novi;
    pozicija zadnji = head->next;
    while (zadnji->next != NULL)
    {
        zadnji = zadnji->next;
    }
    novi = kreirajosobu();

    zadnji->next = novi;
    novi->next = NULL;



}

void ispis(pozicija head)
{
    pozicija trenutni = head->next;
    while (trenutni != NULL)
    {
        printf("ime %s prezime %s godina %d \n", trenutni->ime, trenutni->prez, trenutni->god);
        trenutni = trenutni->next;
    }
}
void pretrazi(pozicija head, char prezime[MAX])
{
    int a = 0;
    pozicija temp = head->next;
    while (temp != NULL)
    {

        if (strcmp(prezime, temp->prez) == 0)
        {
            printf("%s %s %d \n", temp->ime, temp->prez, temp->god);
            a = 1;
            break;
        }
        temp = temp->next;
    }
    if (a == 0)
        printf("nema trazenog prezimena \n");
}
void brisi(pozicija head, char prezime[MAX])
{

    pozicija prethodni = head;
    pozicija trenutni = NULL;
    while (prethodni->next != NULL && strcmp(prethodni->next->prez, prezime))
    {
        prethodni = prethodni->next;

    }
    trenutni = prethodni->next;
    if (trenutni == NULL)
    {
        printf("nema zeljene osobe\n");
    }
    else
    {
        prethodni->next = trenutni->next;
        free(trenutni);
        printf(" \nizbrisana je osoba sa prezimenom %s \n", prezime);
    }
}
void izbrisilistu(pozicija head)
{
    pozicija temp = NULL;
    while (head->next != NULL)
    {
        temp = head->next;
        head->next = head->next->next;
        free(temp);
    }
}