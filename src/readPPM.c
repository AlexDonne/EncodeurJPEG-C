#include <stdlib.h>
#include <stdio.h>
#include "../include/readPPM.h"
#include<stdint.h>
#include <string.h>

/**
 * Retourne une structure ImagePPM comprenant toutes les informations sur l'image
 * @param chemin
 * @return
 */
ImagePPM* creerImagePPM(char *chemin){
    ImagePPM* image = malloc(sizeof(struct imagePPM));
    test_malloc(image);
    image->chemin = chemin;
    image->nom = nouveauNom(chemin);
    lireFichierPPM(image);
    return image;
}


char*nouveauNom(char *chemin){
    char lettre;
    int nblettres = 0;
    for (int i = strlen(chemin) - 1; i>=0; i--, nblettres++){
        if ((lettre = chemin[i]) == '/'){
            break;
        }
    }
    char *nom = malloc(nblettres * sizeof(char));
    test_malloc(nom);
    int debut = strlen(chemin) - nblettres;
    for (unsigned long i = debut, j=0; i < strlen(chemin); ++i, ++j) {
        nom[j] = chemin[i];
    }
    char *inter = malloc((nblettres + 9)* sizeof(char));
    test_malloc(inter);
    strcpy(inter, "napoleon-");
    strcat(inter, nom);
    int longueur = strlen(inter);
    inter[longueur - 3] = 'j';
    inter[longueur - 2] = 'p';
    inter[longueur - 1] = 'g';
    free(nom);
    char *final = malloc((strlen(inter) + 7)* sizeof(char));
    test_malloc(final);
    strcpy(final, "images/");
    strcat(final, inter);
    free(inter);
    return final;
}

/**
 * Lit le fichier PPM ou PGM en binaire et remplit la structure ImagePPM
 * @param image
 */
void lireFichierPPM(ImagePPM *image){
    FILE* fichier = fopen(image->chemin, "rb");
    if (fichier == NULL){
        printf("Erreur ouverture fichier");
        exit(EXIT_FAILURE);
    }
    fgetc(fichier);
    char magic_number = fgetc(fichier);

    fgetc(fichier); //enlever saut de ligne

    uint32_t nb_col = 0;
    char a;
    if ((a=fgetc(fichier)) == '#'){ //Passer tous les commentaires
        while ((a=fgetc(fichier)) != '\n'){
            continue;
        }
        a = fgetc(fichier);
    }

    while (a  != ' ') {
        nb_col = 10*nb_col + (a - '0');
        a = fgetc(fichier);
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
        test_malloc(image->pixelsNB);
        lireNoirEtBlanc(image, fichier);
    }
    else if (magic_number == '6'){
        image->type = RGB;
        image->pixelsRGB = malloc(nb_ligne * sizeof(PixelRGB*));
        test_malloc(image->pixelsRGB);
        lireCouleurs(image, fichier);
    }
    fclose(fichier);
}

/**
 * Lit le fichier pgm (un pixel a une seula valeur)
 * @param image
 * @param fichier
 */
void lireNoirEtBlanc(ImagePPM* image, FILE* fichier){
    for (int i=0; i < image->hauteur; i++){
        image->pixelsNB[i] = malloc(image->largeur * sizeof(PixelNB));
        test_malloc(image->pixelsNB[i]);
        for (int j=0; j< image->largeur; j++){
            PixelNB pixel;
            fread(&pixel, sizeof(pixel),1 ,fichier);
            image->pixelsNB[i][j] = pixel;
        }
    }
}

/**
 * Lit le fichier ppm (un pixel a 3 valeurs)
 * @param image
 * @param fichier
 */
void lireCouleurs(ImagePPM* image, FILE* fichier){
    for (int i=0; i < image->hauteur; i++){
        image->pixelsRGB[i] = malloc(image->largeur * sizeof(PixelRGB));
        test_malloc(image->pixelsRGB[i]);
        for (int j=0; j< image->largeur; j++){
            PixelRGB pixel;
            fread(&pixel.rouge, sizeof(pixel.rouge),1 ,fichier);
            fread(&pixel.vert, sizeof(pixel.vert),1 ,fichier);
            fread(&pixel.bleu, sizeof(pixel.bleu),1 ,fichier);
            image->pixelsRGB[i][j] = pixel;
        }
    }
}

/**
 * Affiche en hexa l'image (que couleur)
 * @param image
 */
void afficheImageCouleur(ImagePPM* image){
    for (int i=0; i<image->hauteur; i++){
        for(int j=0; j<image->largeur; j++){
            printf("%x%x%x ", image->pixelsRGB[i][j].rouge, image->pixelsRGB[i][j].vert, image->pixelsRGB[i][j].bleu);
        }
        printf("\n");
    }
}