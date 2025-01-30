#ifndef SHOP_H
#define SHOP_H
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>

#define ARITHMOS_PROIONTON 20 
#define AGORES 10 
#define PELATES 5

typedef struct {
    char perigrafh[50];
    float timh;
    int apothima;
} katalogos;

void dimiourgiaKatalogou(katalogos proionta[]); 
void epexergasiaParaggelias(katalogos proionta[], int pelatis_syndesi, int* esoda, int* epitiximenes_paraggelies, int* apotiximenes_paraggelies, int apotyxia[][PELATES], int sixnotita[], int pelatis_deiktis); 

#endif // SHOP_H
