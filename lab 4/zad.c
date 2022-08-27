/*
Korišćenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu (
putanja do direktorijuma se zadaje kao ulazni argument programa) rekurzivno nalazi pet najvećih regularnih datoteka.
Program na kraju rada treba da odštampa putanje i veličine tih datoteka.
*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void poddir(char* path, char fileName[100][255], int* fileSize, int* brojFajla) {
    DIR* dp;
    char ppath[255];
    struct stat st;
    struct dirent* tr;
    // printf("Otvaram %s\n", path);

    if ((dp = opendir(path)) == -1) {
        printf("Nije moguce otvoriti direktorijum %s\n", path);
        exit(-1);
    }

    while ((tr = readdir(dp)) != NULL) {
        if (strcmp(tr->d_name, ".") != 0 && strcmp(tr->d_name, "..") != 0) {
            strcpy(ppath, path);
            strcat(ppath, "/");
            strcat(ppath, tr->d_name);
            if (stat(ppath, &st) == -1) {
                printf("Nije moguce pristupiti %s\n", ppath);
                continue;
            }
            if (S_ISREG(st.st_mode)) {
                fileSize[*brojFajla] = st.st_size;
                strcpy(fileName[*brojFajla], ppath);
                (*brojFajla)++;
            } else if (S_ISDIR(st.st_mode)) {
                poddir(ppath, fileName, fileSize, brojFajla);
            }
        }
    }
    closedir(dp);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Nedovoljno argumenata!\n");
        return -1;
    }

    char fileName[100][255];
    int fileSize[100];
    int brojFajla = 0;

    poddir(argv[1], fileName, fileSize, &brojFajla);

    int i;
    int j;
    int max;
    char tren[255];
    int pom;
    for (i = 0; i < brojFajla; i++) {
        max = i;
        for (j = i + 1; j < brojFajla; j++)
            if (fileSize[max] < fileSize[j])
                max = j;
        if (max != i) {
            strcpy(tren, fileName[i]);
            pom = fileSize[i];
            strcpy(fileName[i], fileName[max]);
            fileSize[i] = fileSize[max];
            strcpy(fileName[max], tren);
            fileSize[max] = pom;
        }
    }

    for (i = 0; i < 5; i++) {
        printf("%d. %s %d\n", i + 1, fileName[i], fileSize[i]);
    }
    return 1;
}