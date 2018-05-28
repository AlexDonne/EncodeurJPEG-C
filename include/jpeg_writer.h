//
// Created by donnea on 5/23/18.
//

#ifndef ETU_JPEG_WRITER_H
#define ETU_JPEG_WRITER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mon_bitstream.h"
#include "qtables.h"
#include "test_malloc.h"
#include <string.h>

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component {
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant les types de composantes fréquentielles (DC ou
    AC).
*/
enum sample_type {
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction {
    H,
    V,
    NB_DIRECTIONS
};

/*
    Type opaque contenant l'intégralité des informations nécessaires à
    l'écriture de l'entête JPEG.
*/
struct jpeg_desc {
    const char *ppm_filename;
    const char *jpeg_filename;
    char *commentaire;
    uint16_t height;
    uint16_t width;
    uint8_t nb_components;
    uint8_t sampling_factor[3][2];
    uint8_t *quantization_tables[3];
    uint8_t *huffman_tables[3][2];
    uint8_t taille_huffman_table[3][2];
    struct bitstream *stream;
};

struct jpeg_desc *jpeg_desc_create(void);

void jpeg_desc_destroy(struct jpeg_desc *jdesc);

void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc, const char *ppm_filename);

void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc, const char *jpeg_filename);

void jpeg_desc_set_comment(struct jpeg_desc *jdesc, char *commentaire);

void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc, enum color_component cc, uint8_t *qtable);

void jpeg_desc_set_image_height(struct jpeg_desc *jdesc, uint16_t image_height);

void jpeg_desc_set_image_width(struct jpeg_desc *jdesc, uint16_t image_width);

void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc, uint8_t nb_components);

void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc, enum direction dir,
                                   uint8_t sampling_factor);

void jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc,
                                 uint8_t *htable, uint8_t nb_symbols);

void ecrire_SOI(struct jpeg_desc *jdesc);

void ecrire_APPx(struct jpeg_desc *jdesc);

void ecrire_COM(struct jpeg_desc *jdesc);

void ecrire_DQT(struct jpeg_desc *jdesc);

void ecrire_SOF(struct jpeg_desc *jdesc);

void ecrire_SOF(struct jpeg_desc *jdesc);

void ecrire_EOI(struct jpeg_desc *jdesc);

void ecrire_SOS(struct jpeg_desc *jdesc);

uint8_t *concatener(uint8_t *htables_nb_symb_per_lengths, uint8_t *htables_symbols, uint8_t nb_symbols);

void jpeg_write_header(struct jpeg_desc *jdesc);

void jpeg_write_footer(struct jpeg_desc *jdesc);

struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc);


#endif //ETU_JPEG_WRITER_H
