#include <stdio.h>
#include <stdlib.h>
#include "../include/readPPM.h"
#include "../include/imageToMCUs.h"
#include "../include/operationMCU.h"
#include "../include/codage_huffman_RLE.h"
#include "../include/echantillonnage.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Argument requis : nom du fichier");
        exit(1);
    }
    ImagePPM *image = creerImagePPM(argv[1]);
    MCUsMatrice *mat = imageToMCUs(image);
    MCUsTransformMat *mcusTransform = rgbTOycbcrAllMcus(mat);
    MCUsTransformMat *nouv = malloc(sizeof(MCUsTransformMat));
    echantillonnage(mcusTransform, nouv, 2, 2, 1, 1, 1, 1);
   afficherAllMCUs2(nouv);
//    MCUsTransformToQuantif(mcusTransform);
//    ecrire_jpeg(image, mcusTransform);
}