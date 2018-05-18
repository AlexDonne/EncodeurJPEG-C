#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

#include "structures.h"
/**
 * Créer le tableau de MCU pour l'image donnée (en couleur)
 * @param image
 * @return
 */
MCUsMatrice* imageToMCUs(ImagePPM *image);

/**
 * Créer le tableau de MCU pour l'image donnée (en noir et blanc)
 * @param image
 * @return
 */

void libererImage(ImagePPM *image);

void afficherMCUs(MCUsMatrice *tabMcus);

void afficherMCURGB(MCUPixels mcu);

void afficherMCUNB(MCUPixels mcu);

#endif //ETU_IMAGETOMCUS_H
