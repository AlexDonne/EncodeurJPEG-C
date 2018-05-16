//
// Created by monsinjs on 5/16/18.
//

#ifndef ENCODEURJPEG_C_OPERATIONMCU_H
#define ENCODEURJPEG_C_OPERATIONMCU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include/structures.h"

MCUTransform* rgbTOycbcr(MCUPixelsRGB mcuPixels);

/**
 * Applique le zigzag sur un MCU
 * @param mcu
 * @param zig_mcu
 */
void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu);





#endif //ENCODEURJPEG_C_OPERATIONMCU_H
