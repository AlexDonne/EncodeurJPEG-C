#ifndef IMAGE_TO_MCU_H
#define IMAGE_TO_MCU_H

#include "structures.h"

ImagePPM* creerImagePPM(char *chemin);

void lireFichierPPM(ImagePPM *image);

void lireNoirEtBlanc(ImagePPM* image, FILE* fichier);

void lireCouleurs(ImagePPM* image, FILE* fichier);

void afficheImageCouleur(ImagePPM* image);

#endif