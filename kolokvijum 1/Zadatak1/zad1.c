/*

Korišćenjem programskog jezika C kreirati Linux
 program u kome glavna nit kreira celobrojni niz od N elemenata i 
popunjava ga slučajnim vrednostima iz opsega [0, 10]. 
Glavna nit pokreće dve dodatne niti koje su zadužene za 
sortiranje niza. Prva dodatna nit sortira prvu polovinu 
niza (0-N/2 – 1) u rastući redosled a druga nit sortira 
drugu polovinu niza (N/2 – N-1) u opadajući redosled. 
Kada obe dodatne niti završe sa sortiranjem, glavna nit 
štampa sortirani niz. Za sinhronizaciju niti koristiti mutex-e i 
uslovne promenljive.

*/
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
pthread_mutex_t prvaPolovina;
pthread_mutex_t drugaPolovina;

int upisanPrviDeo = 0;
int upisanDrugiDeo = 0;

pthread_cond_t sortirajPrviDeo;
pthread_cond_t sortirajDrugiDeo;

int niz[N];

void* funcPrvaPolovina(void* arg) {
    int i;
    int max;
    int pom;
    int j;

    pthread_mutex_lock(&prvaPolovina);
    while (!upisanPrviDeo)
        pthread_cond_wait(&sortirajPrviDeo, &prvaPolovina);

    for (i = 0; i < N / 2; i++) {
        max = i;
        for (j = i + 1; j < N / 2; j++)
            if (niz[max] < niz[j])
                max = j;
        if (max != i) {
            pom = niz[i];
            niz[i] = niz[max];
            niz[max] = pom;
        }
    }

    pthread_mutex_unlock(&prvaPolovina);
}
void* funcDrugaPolovina(void* arg) {
    int i;
    int max;
    int pom;
    int j;

    pthread_mutex_lock(&drugaPolovina);
    while (!upisanDrugiDeo)
        pthread_cond_wait(&sortirajDrugiDeo, &drugaPolovina);

    for (i = N / 2; i < N; i++) {
        max = i;
        for (j = i + 1; j < N; j++)
            if (niz[max] < niz[j])
                max = j;
        if (max != i) {
            pom = niz[i];
            niz[i] = niz[max];
            niz[max] = pom;
        }
    }

    pthread_mutex_unlock(&drugaPolovina);
}

int main() {
    pthread_t t1, t2;
    int i = 0;
    //int N = 10;
    pthread_mutex_init(&prvaPolovina, NULL);
    pthread_mutex_init(&drugaPolovina, NULL);

    pthread_cond_init(&sortirajPrviDeo, NULL);
    pthread_cond_init(&sortirajDrugiDeo, NULL);

    pthread_create(&t1, NULL, (void*)funcPrvaPolovina, NULL);
    pthread_create(&t1, NULL, (void*)funcDrugaPolovina, NULL);

    pthread_mutex_lock(&prvaPolovina);
    i = 0;
    while (i < N / 2) {
        niz[i] = random() % 100;
        //printf("Upisujem %d\n", i);
        i++;
    }
    upisanPrviDeo = 1;
    pthread_cond_signal(&sortirajPrviDeo);
    pthread_mutex_unlock(&prvaPolovina);

    pthread_mutex_unlock(&drugaPolovina);
    while (i < N) {
        niz[i] = random() % 100;
        //printf("Upisujem %d\n", i);
        i++;
    }

    upisanDrugiDeo = 1;

    pthread_cond_signal(&sortirajDrugiDeo);
    pthread_mutex_unlock(&drugaPolovina);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    for (i = 0; i < N; i++) {
        printf("%d ", niz[i]);
    }
    printf("\n");

    pthread_cond_destroy(&sortirajPrviDeo);
    pthread_cond_destroy(&sortirajDrugiDeo);

    pthread_mutex_destroy(&prvaPolovina);
    pthread_mutex_destroy(&drugaPolovina);
}