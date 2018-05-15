#include <stdio.h>
#include <stdlib.h>
#include "../include/imagetoMCU.h"
int main (int argc, char *argv[]){
    if (argc < 2){
        printf("Argument requis : nom du fichier");
        exit(1);
    }
    ImagePPM* image = creerImagePPM(argv[1]);
}