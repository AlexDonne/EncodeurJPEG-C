#ifndef ETU_IMAGETOMCUS_H
#define ETU_IMAGETOMCUS_H

/**
 * Créer le tableau de MCU pour l'image donnée (en couleur)
 * @param image
 * @return
 */
MCUPixelsRGB *imageCouleurToMCUs(ImagePPM *image, int *taille);

/**
 * Créer le tableau de MCU pour l'image donnée (en noir et blanc)
 * @param image
 * @return
 */
MCUPixelsNB* imageNBToMCUs(ImagePPM* image, int *taille);

void afficherMCUs(MCUPixelsRGB *tabMcus, int taille);

void afficherMCU(MCUPixelsRGB mcu);

#endif //ETU_IMAGETOMCUS_H
