#include <stdio.h>
#include <stdlib.h>
#include "../include/readPPM.h"
#include "../include/imageToMCUs.h"
int main (int argc, char *argv[]){
    if (argc < 2){
        printf("Argument requis : nom du fichier");
        exit(1);
    }
    ImagePPM* image = creerImagePPM(argv[1]);

    if (image->type == RGB) {
        //afficheImageCouleur(image);
        imageCouleurToMCUs(image);
    }
    else {

    }
}