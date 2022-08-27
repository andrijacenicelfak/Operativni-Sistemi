/*

Korišćenjem programskog jezika C napisati Linux/UNIX program koji se ne moze prekinuti jednim pritiskom na kombinaciju tastera Ctrl-C,
već sa 3 uzastopna pritiska ove kombinacije tastera u roku od 3 sekunde. Ukoliko od prvog pritiska kmbinacije tastera Ctrl-C protekne više
od 3 sekunde a korisnik nije 3 puta pritisnuo ovu kombinaciju tastera program na ekranu štampa poruku da se prekid programa resetuje i cela procedura
detekcije 3 pritiska u roku od 3 sekunde počinje ispočetka.

*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int brojPuta;

void alarmhandler() {
    brojPuta = 0;
    printf("Isteklo 3 sekunde resetuje se prekid programa!\n");
}
void ctrlchandler() {
    if (brojPuta == 0)
        alarm(3);
    brojPuta++;
    if (brojPuta == 3)
        exit(1);
}

int main() {
    signal(SIGALRM, alarmhandler);
    signal(SIGINT, ctrlchandler);
    while (1) {
        pause();
    }
}