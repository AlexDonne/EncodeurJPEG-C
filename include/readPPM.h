#ifndef IMAGE_TO_MCU_H
#define IMAGE_TO_MCU_H

#include "affichageStructures.h"
#include "structures.h"
#include "test_malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include<stdint.h>
#include <string.h>

/**
 * Retourne une structure ImagePPM comprenant toutes les informations sur l'image
 * @param chemin
 * @return
 */
ImagePPM *creerImagePPM(char *chemin, char *nom);

/**
 * Retourne le futur nom de l'image JPEG
 * @param chemin
 * @return
 */
char *nouveauNom(char *chemin);

/**
 * Lit le fichier PPM ou PGM en binaire et remplit la structure ImagePPM
 * @param image
 */
void lireFichierPPM(ImagePPM *image);


/**
 * Lit le fichier pgm (un pixel a une seula valeur)
 * @param image
 * @param fichier
 */
void lireNoirEtBlanc(ImagePPM *image, FILE *fichier);

/**
 * Lit le fichier ppm (un pixel a 3 valeurs)
 * @param image
 * @param fichier
 */
void lireCouleurs(ImagePPM *image, FILE *fichier);

#endif