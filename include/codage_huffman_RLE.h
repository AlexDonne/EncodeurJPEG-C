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

/* Ecrit le codage differentiel DC pour la valeur nombre selon la composante cc */
void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc);

/* Ecrit le codage Huffman de nombre selon les tables de Huffman associées au sample_type acdc et de la color_component cc */
void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc);

/* Ecrit dans magnitude un nombre entre 0 et 11 correspondant à la magnitude de nombre et dans indice son indice correspondant */
void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice);

/* Ecrit toute l'entête jpeg */
struct bitstream *ecrire_entete(struct jpeg_desc *jdesc, const char *ppm_filename, const char *jpeg_filename,
                                uint32_t image_height, uint32_t image_width, bool couleur, int h1, int l1, int h2, int l2, int h3, int l3);

/* Ecrit le codage AC avec RLE pour le bloc 8x8 tab */
void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc);

/* Réalise toutes les étapes pour écrire le jpeg */
void ecrire_jpeg(ImagePPM *image, MCUsTransformMat *mcusTransform, int h1, int l1, int h2, int l2, int h3, int l3);

<<<<<<< HEAD
/* Libère l'espace mémoire d'une ImagePPM */
void libererImage(ImagePPM *image);

/* Libère l'espace mémoire d'une MCUsTransformMat */
void libererMCUsTransform(MCUsTransformMat *mcUsTransform);

/* Libère l'espace mémoire d'une MCUTransform */
void libererMCUTransform(MCUTransform *mcuTransform);

=======
>>>>>>> 6e4a0ee1e777a014d11298382d11acd54b0ef7b0
#endif //ETU_CODAGE_HUFFMAN_RLE_H
