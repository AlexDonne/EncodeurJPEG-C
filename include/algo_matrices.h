#ifndef _ALGO_MATRICE_H_
#define _ALGO_MATRICE_H_


/* Matrice de la page 37 du poly (utile pour vérifier nos algorithmes) */
//

void zigzag(int16_t *matrice, int16_t *zig_matrice);

void discrete_cosinus_transform(int16_t *matrice, int16_t *dct_matrice);

void quantificationY(int16_t *matrice);

void quantificationCbCr(int16_t *matrice);
#endif
