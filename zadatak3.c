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

int meni();
pozicija kreirajosobu();
void unospocetak(pozicija);
void unoskraj(pozicija);
void ispis(pozicija);
void pretrazi(pozicija, char[MAX]);
void brisi(pozicija, char[MAX]);
void izbrisilistu(pozicija);
pozicija prethodni(pozicija, char[MAX]);
void nakonodr(pozicija, char[MAX]);
void prijeodr(pozicija, char[MAX]);
void SortPrez(pozicija);
int upisdat(pozicija);
int citadat(pozicija);


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
        izbor = meni();
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
            printf("unesi prezime osobe koju zelis izbrisati: \n");
            scanf("%s", prezime);
            brisi(head, prezime);
            break;
        case 6:
            printf("unesi prezime osobe nakon koje zelis dodati: \n");
            scanf("%s", prezime);
            nakonodr(head, prezime);
            break;
        case 7:
            printf("unesi prezime osobe prije koje zelis dodati: \n");
            scanf("%s", prezime);
            prijeodr(head, prezime);
            break;
        case 8:
            SortPrez(head);
            break;
        case 9:
            upisdat(head);
            break;
        case 10:
            citadat(head);
            break;
        case 11:

            break;
        default:
            printf("\tGreska\n");
            break;
        }
    } while (izbor != 11);
    izbrisilistu(head);
    return 0;
}
int meni()
{
    int izbor;
    printf("\nIzaberi 1 za dodat novi element na pocetak liste\n");
    printf("Izaberi 2 za ispis liste\n");
    printf("Izaberi 3 za dodat novi element na kraj liste\n");
    printf("Izaberi 4 za pronalazak elementa po prezimenu\n");
    printf("Izaberi 5 za brisanje odredenog elementa \n");
    printf("Izaberi 6 dinamicki dodaje novi element iza odredenog elementa\n");
    printf("Izaberi 7 dinamicki dodaje novi element ispred odredenog elementa\n");
    printf("Izaberi 8 sortira listu po prezimenima osoba\n");
    printf("Izaberi 9 upisuje listu u datoteku\n");
    printf("Izaberi 10 cita listu iz datotekeu\n");
    printf("Izaberi 11 za kraj\n\n");
    scanf("%d", &izbor);
    return izbor;
}
pozicija kreirajosobu()
{
    pozicija os = NULL;
    os = (pozicija)malloc(sizeof(osoba));
    if (!os) return NULL;
    printf("unesi ime: ");
    scanf("%s", os->ime);
    printf("unesi prezime: ");
    scanf("%s", os->prez);
    printf("unesi godinu: ");
    scanf("%d", &os->god);

    return os;


}
pozicija prethodni(pozicija head, char prezime[MAX])
{
    pozicija prev = head;
    while (prev->next != NULL && strcmp(prev->next->prez, prezime))
    {
        prev = prev->next;

    }
    if (prev == NULL)
        return NULL;
    return prev;
}
pozicija trenutno(pozicija head, char prezime[MAX])
{
    pozicija trenutni = head;
    while (trenutni != NULL && strcmp(trenutni->prez, prezime))
    {
        trenutni = trenutni->next;

    }
    if (trenutni == NULL)
        return NULL;
    return trenutni;
}


void unospocetak(pozicija head)
{
    pozicija novi = NULL;
    pozicija prvi = head->next;
    novi = kreirajosobu();
    head->next = novi;
    novi->next = prvi;



}

void unoskraj(pozicija head)
{
    pozicija novi = NULL;
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
    pozicija prev = prethodni(head, prezime);
    pozicija trenutni = NULL;
    if (prev == NULL)
    {
        printf("nema zeljene osobe\n");
    }
    else
    {
        trenutni = prev->next;
        prev->next = trenutni->next;
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

void nakonodr(pozicija head, char prezime[MAX])
{
    pozicija novi = NULL;
    novi = kreirajosobu();
    pozicija trenutni = trenutno(head, prezime);
    pozicija temp = trenutni->next;
    trenutni->next = novi;
    novi->next = temp;

}
void prijeodr(pozicija head, char prezime[MAX])
{
    pozicija novi = NULL;
    novi = kreirajosobu();
    pozicija prev = prethodni(head, prezime);
    pozicija temp = prev->next;
    prev->next = novi;
    novi->next = temp;

}

void SortPrez(pozicija p)
{

    pozicija q = NULL;
    pozicija prev_q = NULL;
    pozicija temp = NULL;
    pozicija end = NULL;

    while (p->next != end) {
        prev_q = p;
        q = p->next;

        while (q->next != end) {
            if (strcmp(q->prez, q->next->prez) > 0) {
                temp = q->next;
                prev_q->next = temp;
                q->next = temp->next;
                temp->next = q;

                q = temp;
            }
            prev_q = q;
            q = q->next;
        }
        end = q;
    }


}
int upisdat(pozicija head)
{
    pozicija trenutni = head->next;
    FILE* ulz = fopen("osobe.txt", "w");
    if (ulz == NULL)
    {
        printf("\n datoteka se nije mogla otvoriti\n");
        return -1;
    }
    while (trenutni != NULL)
    {
        fprintf(ulz, "%s %s %d\n", trenutni->ime, trenutni->prez, trenutni->god);
        trenutni = trenutni->next;
    }
    fclose(ulz);
    printf("uspjesno upisano");
    return 0;

}

int citadat(pozicija head)
{
    izbrisilistu(head);
    FILE* izl = fopen("osobe.txt", "r");
    pozicija trenutni = NULL;
    pozicija prev = head;
    if (izl == NULL)
    {
        printf("\n datoteka se nije mogla otvoriti\n");
        return 0;
    }
    int br = 0, i;
    while (!feof(izl)) {
        if (fgetc(izl) == '\n')
            br++;
    }
    rewind(izl);

    if (br != 0)
    {
        for (i = 0; i < br; i++)
        {

            trenutni = (pozicija)malloc(sizeof(osoba));
            if (!trenutni)
            {
                printf("greska \n");
                return 0;
            }

            fscanf(izl, "%s %s %d", trenutni->ime, trenutni->prez, &trenutni->god);
            trenutni->next = prev->next;
            prev->next = trenutni;
            prev = trenutni;
        }


    }

    fclose(izl);
    ispis(head);
    return 0;


}