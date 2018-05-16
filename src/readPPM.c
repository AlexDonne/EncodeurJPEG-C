#include <stdlib.h>
#include <stdio.h>
#include "../include/readPPM.h"
#include<stdint.h>


ImagePPM* creerImagePPM(char *chemin){
    ImagePPM* image = malloc(sizeof(struct imagePPM));
    image->chemin = chemin;
    lireFichierPPM(image);
    return image;
}

void lireFichierPPM(ImagePPM *image){
    FILE* fichier = fopen(image->chemin, "rb");
    fgetc(fichier);
    char magic_number = fgetc(fichier);

    fgetc(fichier); //enlever saut de ligne

    uint32_t nb_col = 0;
    char a;
    if ((a=fgetc(fichier)) == '#'){ //Passer tous les commentaires
        while ((a=fgetc(fichier)) != '\n'){
            continue;
        }
    }
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
    else if (magic_number == '6'){
        image->type = RGB;
        image->pixelsRGB = malloc(nb_ligne * sizeof(PixelRGB*));
        lireCouleurs(image, fichier);
    }
    fclose(fichier);
}


void lireNoirEtBlanc(ImagePPM* image, FILE* fichier){
    for (int i=0; i < image->hauteur; i++){
        image->pixelsNB[i] = malloc(image->largeur * sizeof(PixelNB));
        for (int j=0; j< image->largeur; j++){
            PixelNB pixel;
            fread(&pixel, sizeof(pixel),1 ,fichier);
            image->pixelsNB[i][j] = pixel;
        }
    }
}

void lireCouleurs(ImagePPM* image, FILE* fichier){
    for (int i=0; i < image->hauteur; i++){
        image->pixelsRGB[i] = malloc(image->largeur * sizeof(PixelRGB));
        for (int j=0; j< image->largeur; j++){
            PixelRGB pixel;
            fread(&pixel.rouge, sizeof(pixel.rouge),1 ,fichier);
            fread(&pixel.vert, sizeof(pixel.vert),1 ,fichier);
            fread(&pixel.bleu, sizeof(pixel.bleu),1 ,fichier);
            image->pixelsRGB[i][j] = pixel;
        }
    }
}

void afficheImageCouleur(ImagePPM* image){
    for (int i=0; i<image->hauteur; i++){
        for(int j=0; j<image->largeur; j++){
            printf("%x%x%x ", image->pixelsRGB[i][j].rouge, image->pixelsRGB[i][j].vert, image->pixelsRGB[i][j].bleu);
        }
        printf("\n");
    }
}