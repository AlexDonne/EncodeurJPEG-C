#ifndef IMAGE_TO_MCU_H
#define IMAGE_TO_MCU_H

typedef struct imagePPM ImagePPM;

typedef enum TYPE_IMAGE TYPE_IMAGE;

ImagePPM* creerImagePPM(char *chemin);

void lireFichierPPM(ImagePPM *image);

void lireNoirEtBlanc(ImagePPM* image, FILE* fichier);

void lireCouleurs(ImagePPM* image, FILE* fichier);

#endif