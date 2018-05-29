#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "qtables.h"
#include "structures.h"
#include "affichageStructures.h"
#include "test_malloc.h"
#include "mon_bitstream.h"
#include "jpeg_writer.h"

#ifndef ETU_CODAGE_HUFFMAN_RLE_H
#define ETU_CODAGE_HUFFMAN_RLE_H

void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc);

void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc);

void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice);

struct bitstream *ecrire_entete(struct jpeg_desc *jdesc, const char *ppm_filename, const char *jpeg_filename,
                                uint32_t image_height, uint32_t image_width, bool couleur, int h1, int l1, int h2, int l2, int h3, int l3);

void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc);

void ecrire_jpeg(ImagePPM *image, MCUsTransformMat *mcusTransform, int h1, int l1, int h2, int l2, int h3, int l3);

#endif //ETU_CODAGE_HUFFMAN_RLE_H
