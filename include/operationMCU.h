

#ifndef ENCODEURJPEG_C_OPERATIONMCU_H
#define ENCODEURJPEG_C_OPERATIONMCU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structures.h"
#include "algo_matrices.h"



MCUTransform rgbTOycbcr(MCUPixelsRGB mcuPixels);

/**
 * Applique le zigzag sur un MCU
 * @param mcu
 * @param zig_mcu
 */
void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu);

void MCUdct(MCUTransform *mcu, MCUTransform *dct_mcu);

void MCUquantification(MCUTransform *mcu);


#endif //ENCODEURJPEG_C_OPERATIONMCU_H
