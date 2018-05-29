

#ifndef ENCODEURJPEG_C_ECHANTILLONNAGE_H
#define ENCODEURJPEG_C_ECHANTILLONNAGE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "structures.h"
#include "libererStructures.h"
#include "test_malloc.h"


/**
 * fusionne les MCUs suivants les paramètres de sampling
 * @param mcus_rgb
 * @param h1
 * @param l1
 * @return
 */
MCUsMatrice *fusion_RGB(MCUsMatrice *mcus_rgb, int h1, int l1);


/**
 * echantillonne une matrice horizontalement
 * @param mat1
 * @param mat2
 * @param echan_mat
 */
void echan_h(int16_t *mat1, int16_t *mat2, int16_t *echan_mat);


/**
* echantillonne une matrice verticalement
* @param mat1
* @param mat2
* @param echan_mat
*/

void echan_v(int16_t *mat1, int16_t *mat2, int16_t *echan_mat);

/**
* echantillonne une matrice horizontalement et verticalement
* @param mat1
* @param mat2
* @param echan_mat
*/
void echan_h_v(int16_t *mat1, int16_t *mat2, int16_t *mat3, int16_t *mat4, int16_t *echan_mat);


/**
* echantillonne le mcu rentré en paramètres et modifie echan_mcu
* @param mcu
* @param echan_mcu
* @param parametres
*/
void mcu_echantillonnage(MCUTransform *mcu, MCUTransform *echan_mcu, int parametres[6]);


/**
 * echantillonne tous les MCUs de la matrice matmcu
 * @param matmcu
 * @param echan_matmcu
 * @param parametres
 */
void MATmcu_echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int parametres[6]);


/**
 * renvoie les matrices de MCUs echantillonnés
 * @param matmcu
 * @param parametres
 * @return
 */
MCUsTransformMat *echantillonnage(MCUsTransformMat *matmcu, int parametres[6]);


#endif //ENCODEURJPEG_C_ECHANTILLONNAGE_H
