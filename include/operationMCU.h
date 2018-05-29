

#ifndef ENCODEURJPEG_C_OPERATIONMCU_H
#define ENCODEURJPEG_C_OPERATIONMCU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "structures.h"
#include "affichageStructures.h"
#include "libererStructures.h"
#include "algo_matrices.h"
#include "test_malloc.h"


/**
 * Transforme une MCU avec pixels RGB en MCUTransforme (avec Y, Cb, Cr)
 * @param mcuPixels
 * @return
 */
MCUTransform rgbTOycbcr(MCUPixels mcuPixels, int h1, int l1);

/**
 * Transforme une structure MCUsMatrice (tableau avec tous les MCUPixels) en MCUsTransformMat (tableau avec les MCUS Y, Cb, Cr)
 * @param mcusMat
 * @return
 */
MCUsTransformMat *rgbTOycbcrAllMcus(MCUsMatrice *mcusMat, int h1, int l1);

/**
 * Transforme une MCU avec pixels Noirs et blanc en MCUTransform avec Y, et CB et Cr qui sont NULL
 * @param mcuPixels
 * @return
 */
MCUTransform nbTOy(MCUPixels mcuPixels);

/**
 * Applique le DCT, le zigzag et la quantification pour tous les MCUS
 * @param mcusTransformMat
 */
void MCUsTransformToQuantif(MCUsTransformMat *mcUsTransformMat);

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image RGB
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifRGB(MCUTransform *mcu, MCUTransform *dct_mcu);

/**
 * Applique le DCT, le zigzag et la quantification pour un MCU d'une image noir et blanc
 * @param mcu
 * @param dct_mcu
 */
void MCUToQuantifNB(MCUTransform *mcu, MCUTransform *dct_mcu);

/**
 * Libère la MCUTransform intermédiaire utilisée
 * @param intermediaire
 */
void libererIntermediaire(MCUTransform *intermediaire);


#endif //ENCODEURJPEG_C_OPERATIONMCU_H
