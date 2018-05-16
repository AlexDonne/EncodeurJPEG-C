//
// Created by monsinjs on 5/16/18.
//

#ifndef ENCODEURJPEG_C_OPERATIONMCU_H
#define ENCODEURJPEG_C_OPERATIONMCU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include/structures.h"



//Affiche le MCU
void afficher_mcu(MCUTransform mcu);

//Applique le zigzag sur un MCU
void MCUzigzag(MCUTransform *mcu, MCUTransform *zig_mcu);





#endif //ENCODEURJPEG_C_OPERATIONMCU_H
