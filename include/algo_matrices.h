#ifndef _ALGO_MATRICE_H_
#define _ALGO_MATRICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "qtables.h"

/**
 * Applique le zig zag sur une matrice
 * @param matrice
 * @param zig_matrice
 */
void zigzag(int16_t *matrice, int16_t *zig_matrice);

/**
 * Applique le DCT sur une matrice, en évitant les calculs de cosinus
 * @param matrice
 * @param dct_matrice
 */
void fast_discrete_cosinus_transform(int16_t *matrice);

/**
 * Applique la quantification pour Y sur une matrice
 * @param matrice
 */
void quantificationY(int16_t *matrice);

/**
 * Applique la quantification pour Cb ou Cr sur une matrice
 * @param matrice
 */
void quantificationCbCr(int16_t *matrice);

#endif
