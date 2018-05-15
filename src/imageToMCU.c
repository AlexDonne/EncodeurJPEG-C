#include <stdlib.h>
#include <stdio.h>
#include "../include/imagetoMCU.h"
#include "../include/structures.h"
#include<stdint.h>


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
    PixelNB **pixelsNB;
};

ImagePPM* creerImagePPM(char *chemin){
    ImagePPM* image = malloc(sizeof(struct imagePPM));
    image->chemin = chemin;
    lireFichierPPM(image);
    return image;
}

void lireFichierPPM(ImagePPM *image){
    FILE* fichier = fopen(image->chemin, "rb");
    char lettre = fgetc(fichier);
    char magic_number = fgetc(fichier);

    fgetc(fichier); //enlever l'espace

    uint32_t nb_col = 0;
    char a;
    while ((a = fgetc(fichier)) != ' ') {
        nb_col = 10*nb_col + (a - '0');
    }

    uint32_t nb_ligne = 0;
    while ((a = fgetc(fichier)) != '\n') {
        nb_ligne = 10*nb_ligne + (a - '0');
    }

    fgetc(fichier);
    fgetc(fichier);
    fgetc(fichier); //Passer le 255 (toujours le cas)
    fgetc(fichier); //0x0a

    image->largeur = nb_col;
    image->hauteur = nb_ligne;

    if (magic_number == '5') {
        image->type = NB;
        image->pixelsNB = malloc(nb_ligne * sizeof(PixelNB*));
        lireNoirEtBlanc(image, fichier);
    }
    else {
        image->type = RGB;
        image->pixelsRGB = malloc(nb_ligne * sizeof(PixelRGB));
        lireCouleurs(image, fichier);
    }
    fclose(fichier);
}


void lireNoirEtBlanc(ImagePPM* image, FILE* fichier){
    for (int i=0; i < image->hauteur; i++){
        image->pixelsNB[i] = malloc(image->largeur * sizeof(PixelNB));
        for (int j=0; j< image->largeur; j++){
            PixelNB pixel;
            fread(&pixel.valeur, sizeof(pixel.valeur),1 ,fichier);
            image->pixelsNB[i][j] = pixel;
        }
    }
}

void lireCouleurs(ImagePPM* image, FILE* fichier){

}