#include "structures.h"
#include <stdio.h>

#ifndef ETU_AFFICHAGESTRUCTURES_H
#define ETU_AFFICHAGESTRUCTURES_H

/**
 * Affiche en hexa l'image (que couleur)
 * @param image
 */
void afficheImageCouleur(ImagePPM *image);

/**
 * Affiche en hexa une image en niveau de gris
 * @param image
 * @param hauteur
 * @param largeur
 */
void afficheImageNB(ImagePPM *image, int hauteur, int largeur);

/**
 * Afiche tous les mcus d'une MCUsMAtrice
 * @param tabMcus
 */
void afficherMCUsMatrice(MCUsMatrice *tabMcus);

/**
 * Affiche une MCU comprenant des pixels RGB
 * @param mcu
 */
void afficherMCUMatrice_RGB(MCUPixels mcu);

/**
 * Affiche une MCU comprenant des pixels nb
 * @param mcu
 */
void afficherMCUMatrice_NB(MCUPixels mcu);

/**
 * Affiche tous les MCUs Transform
 * @param mcusTransformMat
 */
void afficherAllMCUsTransform(MCUsTransformMat *mcusTransformMat);

/**
 * Affiche un MCU Transform
 * @param mcu
 */
void afficher_mcu_Transform(MCUTransform mcu);

#endif //ETU_AFFICHAGESTRUCTURES_H
