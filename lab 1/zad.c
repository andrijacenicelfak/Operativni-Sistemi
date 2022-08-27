#include <pthread.h>
#include <stdio.h>
#include <string.h>

void * odbrojavaj(void* a){
    int cifra = *(int*)a;
    while(cifra >= 0){
        printf("%d\n", cifra);
        cifra --;
        sleep(3);
    }
}

int main(){
    pthread_t t;

    char broj[20];
    int cifra;
    int gotovo = 0;
    int i;
    while(!gotovo){
        printf("Unesi broj : ");
        scanf("%s", broj);
        if(strcmp("KRAJ", broj) != 0){
            cifra = 0;
            i = 0;
            while(broj[i] != '\0'){
                cifra *=10;
                cifra += broj[i] - '0';
                i++;
            }

            pthread_create(&t, NULL, (void*) odbrojavaj, (void*)&cifra);
            pthread_join(t, NULL);
        }else{
            gotovo = 1;
        }
    }

    return 0;
}