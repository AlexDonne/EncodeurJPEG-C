

#ifndef ENCODEURJPEG_C_ECHANTILLONNAGE_H
#define ENCODEURJPEG_C_ECHANTILLONNAGE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "structures.h"

MCUsTransformMat *fusion_mcu(MCUsTransformMat *matmcu, int h1, int l1);

void echan_h(int16_t *mat1, int16_t *mat2, int16_t *echan_mat);

void echan_h_v(int16_t *mat1, int16_t *mat2, int16_t *mat3, int16_t *mat4, int16_t *echan_mat);

void mcu_echan_h(MCUTransform *mcu, MCUTransform *echan_mcu);

void mcu_echan_h_v(MCUTransform *mcu, MCUTransform *echan_mcu);

void MATmcu_echan_h_v(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu);

void MATmcu_echan_h(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu);


void echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int h1, int l1, int h2, int l2);

void afficherAllMCUs2(MCUsTransformMat *mcusTransformMat);

void afficher_mcu2(MCUTransform mcu);


#endif //ENCODEURJPEG_C_ECHANTILLONNAGE_H