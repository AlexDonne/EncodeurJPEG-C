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
MCUsMatrice *imageToMCUs(ImagePPM *image, int h1, int l1);


/**
 * Si l'échantillonage est par deux (par exemple 1x2) mais que le nombre de lignes de la matrice des MCUs n'est pas pair, on rajoute une ligne de MCUs en bas.
 * @param mcusMatrice
 */
void adapterPourEchantillonageHorizontal(MCUsMatrice *mcusMatrice);


/**
 * Si l'échantillonage est par deux (exemple 2x1) mais que le nombre de colonnes de la matrice des MCUs n'est pas pair, on rajoute une colonne de MCUs à droite.
 * @param mcusMatrice
 */
void adapterPourEchantillonageVertical(MCUsMatrice *mcusMatrice);

/**
 * Duplique les dernières lignes ou/et dernières colonnes pour avoir une hauteur et une largeur multiple de 8
 * @param image
 */
void adaptationMCU(ImagePPM *image, int *nouvHauteur, int *nouvLargeur);

#endif //ETU_IMAGETOMCUS_H
