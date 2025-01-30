#include "inventory.h"

// Dimiourgia katalogou proiontwn
void dimiourgiaKatalogou(inventory proionta[]) {
    for (int i = 0; i < ARITHMOS_PROIONTON; i++) {
        // Dimiourgia perigrafhs gia kathe proion
        sprintf(proionta[i].perigrafh, "Proion %d", i + 1);
        proionta[i].timh = rand() % 100 + 1;
        proionta[i].apothima = 2;  // Arxiko diathesimo apothima gia kathe proion
    } 
}

// Xeirismos paraggelion apo pelati
void epexergasiaParaggelias(inventory proionta[], int pelatis_syndesi, int* esoda, int* epitiximenes_paraggelies, int* apotiximenes_paraggelies, int apotyxia[][PELATES], int sixnotita[], int pelatis_deiktis) {
    for (int i = 0; i < AGORES; i++) {
        int proion;
        read(pelatis_syndesi, &proion, sizeof(int)); // Diavazei ton kodiko tou proiontos apo ton pelati
        usleep(500000);  // Mikri kathysterisi gia na prosomoiwthei o xronos epexergasias

        int vrethike = 0;
        char minima[130];
        for (int j = 0; j < ARITHMOS_PROIONTON; j++) {
            if (j + 1 == proion) {
                sixnotita[j]++; // Auxisi tou plithous ton ziteitai gia to sigkekrimeno proion
                if (proionta[j].apothima > 0) { // Elegxos an to proion einai diathesimo
                    proionta[j].apothima--; // Meiwsh tou apothimatos tou proiontos
                    (*epitiximenes_paraggelies)++; // Auxisi twn epitiximenwn paraggelion
                    *esoda += proionta[j].timh; // Prosthiki tis timhs tou proiontos sta esoda
                    sprintf(minima, "H paraggelia %d gia %s itan epitixis\n", proion, proionta[j].perigrafh);
                    write(pelatis_syndesi, minima, strlen(minima) + 1);
                } else {
                    // An den uparxei diathesimo proion, auxisi twn apotiximenwn paraggelion
                    (*apotiximenes_paraggelies)++;
                    apotyxia[j][pelatis_deiktis]++;
                    sprintf(minima, "H paraggelia %d gia %s apeftixe\n", proion, proionta[j].perigrafh);
                    write(pelatis_syndesi, minima, strlen(minima) + 1);
                }
                vrethike = 1;
                break;
            }
        }
        // An o pelatis zitise lathos kodiko proiontos
        if (!vrethike) {
            sprintf(minima, "Mi egkyros kodikos proiontos: %d\n", proion);
            write(pelatis_syndesi, minima, strlen(minima) + 1);
        }
    }
}

int main() {
    srand(time(NULL)); // Arxikopoihsh tuxaioy arithmou
    inventory proionta[ARITHMOS_PROIONTON];
    dimiourgiaKatalogou(proionta); 

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // Dimiourgia tou server
    struct sockaddr_in dieythynsi;
    dieythynsi.sin_family = AF_INET;
    dieythynsi.sin_addr.s_addr = INADDR_ANY;
    dieythynsi.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr *)&dieythynsi, sizeof(dieythynsi));
    listen(server_fd, 5);

    int epitiximenes_paraggelies = 0, apotiximenes_paraggelies = 0, esoda = 0;
    int apotyxia[ARITHMOS_PROIONTON][PELATES];
    memset(apotyxia, 0, sizeof(apotyxia));
    int sixnotita[ARITHMOS_PROIONTON];
    memset(sixnotita, 0, sizeof(sixnotita));

    // Dimiourgia pelatwn me xrhsh fork
    for (int j = 0; j < PELATES; j++) {
        pid_t pid = fork();
        if (pid == 0) { // Paidiaki diergasia - pelatis
            sleep(1); // Kathysterisi gia na einai etoimos o server
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in dieythynsi_diakomisti;
            dieythynsi_diakomisti.sin_family = AF_INET;
            dieythynsi_diakomisti.sin_port = htons(8080);
            inet_pton(AF_INET, "127.0.0.1", &dieythynsi_diakomisti.sin_addr);

            // Syndesi me ton server
            if (connect(sock, (struct sockaddr *)&dieythynsi_diakomisti, sizeof(dieythynsi_diakomisti)) < 0) {
                perror("Apotixia syndesis");
                exit(EXIT_FAILURE);
            }

            // O pelatis kanei paraggelies
            for (int i = 0; i < AGORES; i++) {
                sleep(1); // 1 defterolepto metaksi paraggelion
                int proion = rand() % ARITHMOS_PROIONTON + 1; 
                write(sock, &proion, sizeof(int));
                char apokriisi[130];
                read(sock, apokriisi, sizeof(apokriisi));
                printf("Pelatis %d elave: %s", j + 1, apokriisi);
            }
            close(sock);
            exit(0);
        } else { // Goniki diergasia - server
            int pelatis_syndesi;
            struct sockaddr_in pelatis_dieythynsi;
            socklen_t pelatis_dieythynsi_mege8os = sizeof(pelatis_dieythynsi);
            pelatis_syndesi = accept(server_fd, (struct sockaddr *)&pelatis_dieythynsi, &pelatis_dieythynsi_mege8os);
            epexergasiaParaggelias(proionta, pelatis_syndesi, &esoda, &epitiximenes_paraggelies, &apotiximenes_paraggelies, apotyxia, sixnotita, j);
            close(pelatis_syndesi);
            wait(NULL); 
        }
    }

    printf("\n"); // Ektipwsi apotelesmatwn
    for (int i = 0; i < ARITHMOS_PROIONTON; i++) {
        printf("%s zhtithike: %d fores\n", proionta[i].perigrafh, sixnotita[i]);
        printf("kai pouli8hke: %d fores\n", 2 - proionta[i].apothima);
        for (int j = 0; j < PELATES; j++) {
            if (apotyxia[i][j] > 0) {
                printf("Pelatis %d apetyxe %d fores gia %s\n", j + 1, apotyxia[i][j], proionta[i].perigrafh);
            }
        }
        printf("\n");
    }
    printf("Synolo epitiximenwn paraggelion: %d\n", epitiximenes_paraggelies);
    printf("Synolo apotiximenwn paraggelion: %d\n", apotiximenes_paraggelies);
    printf("Synolo esodon: %.2f\n", (float)esoda);
    printf("\n");

    close(server_fd); 
    return 0;
}
