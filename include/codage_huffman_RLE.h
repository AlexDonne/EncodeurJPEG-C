#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "htables.h"
#include "qtables.h"
#include "huffman.h"
#include "structures.h"

#ifndef ETU_CODAGE_HUFFMAN_RLE_H
#define ETU_CODAGE_HUFFMAN_RLE_H

void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc);

void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc);

void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice);

void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc, int num);

struct bitstream *ecrire_entete_gris(struct jpeg_desc *jdesc, const char *jpeg_filename, uint32_t image_height, uint32_t image_width);

void ecrire_jpeg(ImagePPM *image, MCUsTransformMat *mcusTransform);

void libererImage(ImagePPM *image);

void libererMCUsTransform(MCUsTransformMat *mcUsTransform);

void libererMCUTransform(MCUTransform mcuTransform);

#endif //ETU_CODAGE_HUFFMAN_RLE_H
