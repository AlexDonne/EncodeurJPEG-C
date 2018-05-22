#ifndef IMAGE_TO_MCU_H
#define IMAGE_TO_MCU_H

#include "structures.h"
#include "test_malloc.h"

ImagePPM* creerImagePPM(char *chemin);

char* nouveauNom (char *chemin);

void lireFichierPPM(ImagePPM *image);

void lireNoirEtBlanc(ImagePPM* image, FILE* fichier);

void lireCouleurs(ImagePPM* image, FILE* fichier);

void afficheImageCouleur(ImagePPM* image);

#endif