#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ERROR_OPENING_FILE -1
#define BUFFER_SIZE 1024
#define MAX_BODOVI 50


typedef struct {
    char ime[20];
    char prezime[20];
    int br;

}student;

int count_rows(char* fileName);
student* ucitaj(int br_studenata, FILE* file);
void Ispis(student* lista, int br_studenata);
float Relativni(int lista);


int main() {
    student* lista = NULL;
    int broj_redaka = count_rows("studenti.txt");
    printf("\nBroj redaka u datoteci: %d\n", broj_redaka);

    FILE* file = fopen("studenti.txt", "r");


    if (file == NULL) {
        return ERROR_OPENING_FILE;
    }

    lista = ucitaj(broj_redaka, file);
    Ispis(lista, broj_redaka);

    fclose(file);

    return 0;
}
int count_rows(char* fileName) {
    FILE* file = NULL;
    int brojac = 0;
    char buffer[BUFFER_SIZE];




    file = fopen(fileName, "r");

    if (file == NULL) {
        return ERROR_OPENING_FILE;
    }
    while (fgetc(file) != '\n') {
        //preskakanje prvog reda
    }
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        brojac++;
    }

    fclose(file);

    return brojac;
}

student* ucitaj(int br_studenata, FILE* file) {
    int i;
    student* lista = NULL;
    lista = (student*)malloc(sizeof(student) * br_studenata);

    if (lista == NULL) {
        return ERROR_OPENING_FILE;
    }
    while (fgetc(file) != '\n') {

    }
    for (i = 0; i < br_studenata; i++) {
        fscanf(file, "%s %s %d", (lista + i)->ime, (lista + i)->prezime, &((lista + i)->br));
    }

    return lista;
}

void Ispis(student* lista, int br_studenata) {
    int i;
    printf("\nPodaci o studentima\n\n");
    for (i = 0; i < br_studenata; i++) {
        printf("%s %s %d", (lista + i)->ime, (lista + i)->prezime, (lista + i)->br);
        printf("\nRelativni broj bodova: %f\n", Relativni((lista + i)->br));
        printf("\n");
    }
    return;

}

float Relativni(int lista) {
    float rel;
    rel = (float)lista / MAX_BODOVI * 100;
    return rel;
}