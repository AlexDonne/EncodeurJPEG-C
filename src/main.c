#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/readPPM.h"
#include "../include/imageToMCUs.h"
#include "../include/operationMCU.h"
#include "../include/codage_huffman_RLE.h"

void paramEchantillonage (char* echantillon, int *h1, int *l1, int *h2, int *l2, int *h3, int *l3){
    if (strlen(echantillon) != 11){
        printf("L'échantillonage doit être de la forme : 2x2,2x2,2x1");
        exit(EXIT_FAILURE);
    }
    for (unsigned long i = 0; i < strlen(echantillon); ++i) {
        if (echantillon[i] == ' '){
            printf("L'échantillonage doit être de la forme : 2x2,2x2,2x1");
            exit(EXIT_FAILURE);
        }
    }
    *h1 = echantillon[0] - '0';
    *l1 = echantillon[2] - '0';
    *h2 = echantillon[4] - '0';
    *l2 = echantillon[6] - '0';
    *h3 = echantillon[8] - '0';
    *l3 = echantillon[10] - '0';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Argument requis : nom du fichier\n");
        exit(EXIT_FAILURE);
    }
    char *chemin = NULL;
    char *echantillon = NULL;
    char *nom = NULL;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--sample")){
            echantillon = argv[i+1];
            i++;
        }

        else if (!strcmp(argv[i], "--output-file")){
            nom = argv[i+1];
            i++;
        }
        else {
            chemin = argv[i];
        }
    }
    if (chemin == NULL){
        printf("Argument requis : nom du fichier\n");
        exit(EXIT_FAILURE);
    }
    ImagePPM *image = creerImagePPM(chemin, nom);
    int h1 = 1, l1 = 1, h2 = 1, l2 = 1, h3 = 1, l3 = 1;
    if (echantillon != NULL) {
        paramEchantillonage(echantillon, &h1, &l1, &h2, &l2, &h3, &l3);
    }
    MCUsMatrice *mat = imageToMCUs(image, l1);
    MCUsTransformMat *mcusTransform = rgbTOycbcrAllMcus(mat);
    MCUsTransformToQuantif(mcusTransform);
    ecrire_jpeg(image, mcusTransform);
}