/*
ZAD
Korišćenjem programskog jezika C kreirati tri Linux
 procesa koja komuniciraju korišćenjem datavoda.
Prvi proces generiše niz proizvoljnih celih brojeva
(dužina niza je fiksna i iznosi 15 karaktera).
Ukoliko je prvi generisani broj paran, generisani niz brojeva
se korišćenjem datavoda šalje drugom procesu koji ih upisuje u
datoteku PARNI.txt.
Ukoliko je prvi generisani broj neparan, generisani niz brojeva
se korišćenjem datavoda šalje trećem procesu koji ih štampa na
standardnom izlazu.
*/
#include <math.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int pid1, pid2;
    int p1[2], p2[2];
    int genNiz[15];

    if (pipe(p1) == -1) {
        printf("Greska pri kreiranju datavoda p1!\n");
        return -1;
    }

    if (pipe(p2) == -1) {
        printf("Greska pri kreiranju datavoda p2!\n");
        return -1;
    }
    pid1 = fork();

    if (pid1 == 0) {
        // PROCES1
        int i;
        close(p2[0]);
        close(p2[1]);
        close(p1[1]);
        FILE* f = fopen("PARNI.txt", "w");
        while (read(p1[0], &i, sizeof(int))) {
            fprintf(f, "%d ", i);
        }
        fprintf(f, "\n");
        fclose(f);
        close(p1[0]);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            // PROCES2
            int i;
            close(p1[0]);
            close(p1[1]);
            close(p2[1]);
            while (read(p2[0], &i, sizeof(int))) {
                printf("%d ", i);
            }
            printf("\n");
            close(p2[0]);

        } else {
            // RODITELJ
            close(p1[0]);
            close(p2[0]);
            int i;
            for (i = 0; i < 15; i++) {
                genNiz[i] = random() % 100;
            }
            genNiz[0] = 2;  // ovo je da vidim da li se upisuje u fajl kada je
            // parni broj, jer random uvek daje iste brojeve

            if (genNiz[0] % 2 == 0) {
                write(p1[1], genNiz, 15 * sizeof(int));
            } else {
                write(p2[1], genNiz, 15 * sizeof(int));
            }

            close(p1[1]);
            close(p2[1]);
        }
    }
    return 0;
}