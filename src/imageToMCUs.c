#include "../include/structures.h"
#include "../include/imageToMCUs.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

MCUPixelsRGB *imageCouleurToMCUs(ImagePPM *image) {

    if (image->largeur % 8 != 0 || image->hauteur % 8 != 0) {
        //Fonction qui règle problème
        exit(1);
    }
    MCUPixelsRGB *tableauMCUs;
    int taille = image->hauteur * image->largeur / 64;
    tableauMCUs = malloc(taille * sizeof(MCUPixelsRGB));
    int debcolbase = 0;
    int deblignebase = 0;
    int debligne;
    int finligne = 8;
    int debcol;
    int fincol = 8;
    for (int indTab = 0; indTab < taille; indTab++){
        MCUPixelsRGB mcu;
        for (int i=0, debligne = deblignebase; debligne<finligne; debligne++, i++){
            for(int j=0,debcol = debcolbase; debcol<fincol; debcol++, j++){
                mcu.blocsRGB[i * 8 + j] = image->pixelsRGB[debligne][debcol];
            }
        }
        if (fincol == image->largeur){
            finligne += 8;
            deblignebase += 8;
            fincol = 8;
            debcolbase = 0;
        }
        else{
            fincol += 8;
            debcolbase += 8;
        }
        tableauMCUs[indTab] = mcu;
    }

    //afficherMCUs(tableauMCUs, taille);
    return tableauMCUs;
}

void afficherMCUs(MCUPixelsRGB *tabMcus, int taille) {
    for (int i = 0; i < taille; i++) {
        printf("MCU #%d\n", i);
        afficherMCU(tabMcus[i]);
        printf("\n\n");

    }
}

void afficherMCU(MCUPixelsRGB mcu) {
    for (int i = 0; i < 64; i++) {
            printf("%x%x%x ", mcu.blocsRGB[i].rouge, mcu.blocsRGB[i].vert,mcu.blocsRGB[i].bleu);
        }

}