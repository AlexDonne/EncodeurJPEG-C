#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

#include "structures.h"
#include "test_malloc.h"
#include "affichageStructures.h"
#include "libererStructures.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Transforme la matrice de pixels de la structure image en un tableau de MCUs
 * @param image
 * @return
 */
MCUsMatrice *imageToMCUs(ImagePPM *image,  int l1);

void adapterPourEchantillonageHorizontal(MCUsMatrice * mcusMatrice, TYPE_IMAGE type);

/**
 * Duplique les dernières lignes ou/et dernières colonnes pour avoir une hauteur et une largeur multiple de 8
 * @param image
 */
void adaptationMCU(ImagePPM *image, int *nouvHauteur, int *nouvLargeur);

#endif //ETU_IMAGETOMCUS_H
