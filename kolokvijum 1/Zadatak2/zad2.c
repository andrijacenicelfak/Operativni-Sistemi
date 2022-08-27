/*
Korišćenjem programskog jezika C kreirati Linux program koji kreira dodatni proces
 i ova dva procesa komuniciraju korišćenjem datavoda. 
 Prvi process generiše dve slučajne celobrojne vrednosti u opsegu od 0 do 99 i 
 ukoliko je zbir te dve generisane vrednosti veći od 100 prosleđuje ih drugom procesu 
 korišćenjem datavoda. Nakon upisa drugi process proverava da li su dobijene vrednosti 
 jednake i na ekranu štampa odgovarajuću poruku. Ovaj process generaisanja slučajnog para vrednosti, 
 slanja drugom procesu i štampe poruke na ekranu se ponavlja 10 puta.

*/
#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int p[2];
    int pid;

    if (pipe(p) == -1) {
        printf("Greska pri kreiranju datavoda!");
        return -1;
    }

    pid = fork();

    if (pid != 0) {
        //RODITELJ
        close(p[0]);
        int broj1, broj2;

        broj1 = random() % 100;
        broj2 = random() % 100;
        if (broj1 + broj2 > 100) {
            write(p[1], &broj1, sizeof(int));
            write(p[1], &broj2, sizeof(int));
        } else {
            printf("Vrednosti nisu vece od 100\n");
        }

        close(p[1]);
    } else {
        //DETE
        close(p[1]);
        int broj1, broj2;
        read(p[0], &broj1, sizeof(int));
        read(p[0], &broj2, sizeof(int));

        if (broj1 == broj2) {
            printf("Brojevi su jednaki : %d \n", broj1);
        } else {
            printf("Brojevi su razliciti : %d i %d\n", broj1, broj2);
        }
        close(p[0]);
    }
}