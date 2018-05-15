#include <stdlib.h>
#include <stdio.h>
#include "../include/imagetoMCU.h"
#include<stdint.h>

struct pixelRGB{
    int rouge;
    int bleu;
    int vert;
};

struct pixelNB{
    int valeur;
};


enum TYPE_IMAGE{
    NB,
    RGB
};

struct imagePPM{
    TYPE_IMAGE type;
    char* nom;
    char* chemin;
    int hauteur;
    int largeur;
    PixelRGB **pixelsRGB;
};

ImagePPM* creerImagePPM(char *chemin){
    ImagePPM* image = malloc(sizeof(struct imagePPM));
    image->chemin = chemin;
    lireFichierPPM(image);
}

void lireFichierPPM(ImagePPM *image){
    FILE* fichier = fopen(image->chemin,"rb");
    if (fichier == NULL){
        printf("Erreur ouverture fichier");
        exit(1);
    }
    char magic_number[2];
    uint8_t saut;
    fread(&magic_number, sizeof(magic_number), 1, fichier);
    fread(&saut, sizeof(saut),1, fichier);
    fread(&(image->largeur), )
    magic_number[1] == 5 ? lireNoirEtBlanc(image, fichier): lireCouleur(image, fichier);
}

void lireNoirEtBlanc(ImagePPM* image, FILE* fichier){

}