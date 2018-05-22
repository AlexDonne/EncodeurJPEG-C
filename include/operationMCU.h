

#ifndef ENCODEURJPEG_C_OPERATIONMCU_H
#define ENCODEURJPEG_C_OPERATIONMCU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structures.h"
#include "algo_matrices.h"
#include "test_malloc.h"


MCUTransform rgbTOycbcr(MCUPixels mcuPixels);

MCUsTransformMat* rgbTOycbcrAllMcus(MCUsMatrice *mcusMat);
/**
 * Applique le zigzag sur un MCU
 * @param mcu
 * @param zig_mcu
 */

void libererMCUsMatrice(MCUsMatrice* mcusMat);

void MCUsTransformToQuantif(MCUsTransformMat *mcUsTransformMat);

void MCUToQuantifRGB(MCUTransform *mcu, MCUTransform *dct_mcu, MCUTransform *final);

void MCUToQuantifNB(MCUTransform *mcu, MCUTransform *dct_mcu, MCUTransform *final);

void libererIntermediaire (MCUTransform *intermediaire);

void afficherAllMCUs(MCUsTransformMat *mcusTransformMat);

void afficher_mcu(MCUTransform mcu);


#endif //ENCODEURJPEG_C_OPERATIONMCU_H
