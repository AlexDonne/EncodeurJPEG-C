#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/mon_bitstream.h"
#include "../include/jpeg_writer.h"
#include "../include/qtables.h"
#include "../include/htables.h"
#include <string.h>


struct jpeg_desc *jpeg_desc_create(void) {
    struct jpeg_desc *jdesc = malloc(sizeof(struct jpeg_desc));
    return jdesc;
}


void jpeg_desc_destroy(struct jpeg_desc *jdesc) {
    bitstream_destroy(jdesc->stream);
    free(jdesc);
}


void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc, char *ppm_filename) {
    jdesc->ppm_filename = ppm_filename;
}


void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc, char *jpeg_filename) {
    jdesc->jpeg_filename = jpeg_filename;
    jdesc->stream = bitstream_create(jpeg_filename);
}


void jpeg_desc_set_comment(struct jpeg_desc *jdesc, char *commentaire) {
    jdesc->commentaire = commentaire;
}


void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc, enum color_component cc, uint8_t *qtable) {
    jdesc->quantization_tables[cc] = qtable;
}

void jpeg_desc_set_image_height(struct jpeg_desc *jdesc, uint16_t image_height) {
    jdesc->height = image_height;
}

void jpeg_desc_set_image_width(struct jpeg_desc *jdesc, uint16_t image_width) {
    jdesc->width = image_width;
}

void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc, uint8_t nb_components) {
    jdesc->nb_components = nb_components;
}

void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc, enum direction dir,
                                   uint8_t sampling_factor) {
    jdesc->sampling_factor[cc][dir] = sampling_factor;
}

void
jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc, uint8_t *htable,
                            uint8_t nb_symbols) {
    /* attention : htable est la concaténation de htables_nb_symb_per_lengths et htables_symbols */
    jdesc->taille_huffman_table[cc][acdc] = nb_symbols + 16;
    jdesc->huffman_tables[cc][acdc] = htable;
}


void ecrire_SOI(struct jpeg_desc *jdesc) {
    bitstream_write_nbits(jdesc->stream, 0xffd8, 16, true);
}

void ecrire_APPx(struct jpeg_desc *jdesc) {
    bitstream_write_nbits(jdesc->stream, 0xffe0, 16, true);
    bitstream_write_nbits(jdesc->stream, 0x10, 16, false); // longueur section
    bitstream_write_nbits(jdesc->stream, 0x4a464946, 32, false); // JFIF
    bitstream_write_nbits(jdesc->stream, 0, 8, false); // '\0'
    bitstream_write_nbits(jdesc->stream, 0x0101, 16, false); // doit valoir 1
    bitstream_write_nbits(jdesc->stream, 0, 32, false); // tout mettre à zéro
    bitstream_write_nbits(jdesc->stream, 0, 24, false); // tout mettre à zéro
}

void ecrire_COM(struct jpeg_desc *jdesc) {
    if (jdesc->commentaire != NULL) {
        bitstream_write_nbits(jdesc->stream, 0xfffe, 16, true);
        int lg = strlen(jdesc->commentaire);
        bitstream_write_nbits(jdesc->stream, lg + 2, 16, false);
        for (int i = 0; i < lg; i++) {
            bitstream_write_nbits(jdesc->stream, jdesc->commentaire[i], 8, false);
        }
    }
}

void ecrire_DQT(struct jpeg_desc *jdesc) {
    int n;
    n = (jdesc->nb_components == 1) ? 1 : 2;
    for (int i = 0; i < n; i++) {
        bitstream_write_nbits(jdesc->stream, 0xffdb, 16, true);
        bitstream_write_nbits(jdesc->stream, 67, 16, false); // longueur section
        bitstream_write_nbits(jdesc->stream, 0, 4, false); // precision sur 8 bits
        bitstream_write_nbits(jdesc->stream, i, 4, false); //indice iq de la table de quantification
        for (int k = 0; k < 64; k++) {
            bitstream_write_nbits(jdesc->stream, jdesc->quantization_tables[i][k], 8, false);
        }

    }
}

void ecrire_SOF(struct jpeg_desc *jdesc) {
    bitstream_write_nbits(jdesc->stream, 0xffc0, 16, true);
    bitstream_write_nbits(jdesc->stream, 8 + 3 * jdesc->nb_components, 16, false); //longueur section
    bitstream_write_nbits(jdesc->stream, 8, 8, false); // toujours 8
    bitstream_write_nbits(jdesc->stream, jdesc->height, 16, false);
    bitstream_write_nbits(jdesc->stream, jdesc->width, 16, false);
    bitstream_write_nbits(jdesc->stream, jdesc->nb_components, 8, false);
    for (int i = 0; i < jdesc->nb_components; i++) {
        bitstream_write_nbits(jdesc->stream, i + 1, 8, false);
        bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[i][H], 4, false);
        bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[i][V], 4, false);
        if (i == 0) {
            bitstream_write_nbits(jdesc->stream, 0, 8, false);
        } else {
            bitstream_write_nbits(jdesc->stream, 1, 8, false);
        }
    }
}


void ecrire_DHT(struct jpeg_desc *jdesc) {
    int n = (jdesc->nb_components == 1) ? 1 : 2;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 2; j++) {
            bitstream_write_nbits(jdesc->stream, 0xffc4, 16, true);
            bitstream_write_nbits(jdesc->stream, jdesc->taille_huffman_table[i][j] + 3, 16, false); // longueur section
            bitstream_write_nbits(jdesc->stream, 0, 3, false); // non utilisé, doit valoir (0 sinon erreur)
            bitstream_write_nbits(jdesc->stream, j, 1, false); // acdc
            bitstream_write_nbits(jdesc->stream, i, 4, false); // indice
            printf("ecrire_DHT : \n");
            for (int k = 0; k < jdesc->taille_huffman_table[i][j]; k++) {
                bitstream_write_nbits(jdesc->stream, jdesc->huffman_tables[i][j][k], 8, false);
                printf("%i ", jdesc->huffman_tables[i][j][k]);
            }
        }
    }
}

void ecrire_EOI(struct jpeg_desc *jdesc) {
    bitstream_write_nbits(jdesc->stream, 0xffd9, 16, true);
}

void ecrire_SOS(struct jpeg_desc *jdesc) {
    bitstream_write_nbits(jdesc->stream, 0xffda, 16, true);
    bitstream_write_nbits(jdesc->stream, 6 + 2 * jdesc->nb_components, 16, false); // longueur section
    bitstream_write_nbits(jdesc->stream, jdesc->nb_components, 8, false);
    int n;
    for (int i = 0; i < jdesc->nb_components; i++) {
        bitstream_write_nbits(jdesc->stream, i + 1, 8, false);
        n = (i == 0) ? 0 : 1;
        bitstream_write_nbits(jdesc->stream, n, 4, false); // indice de la table de huffman pour les coefficients DC
        bitstream_write_nbits(jdesc->stream, n, 4, false); // indice de la table de huffman pour les coefficients AC
    }
    bitstream_write_nbits(jdesc->stream, 0x003f00, 24, false); // doit valoir 0x00 0x3f 0x00
}

uint8_t *concatener(uint8_t *htables_nb_symb_per_lengths, uint8_t *htables_symbols, uint8_t nb_symbols) {
    uint8_t *tab = malloc((16 + nb_symbols) * sizeof(uint8_t));
    for (int k = 0; k < 16; k++) {
        tab[k] = htables_nb_symb_per_lengths[k];
    }
    for (int k = 16; k < (nb_symbols + 16); k++) {
        tab[k] = htables_symbols[k - 16];
    }

    return tab;
}


void jpeg_write_header(struct jpeg_desc *jdesc) {
    ecrire_SOI(jdesc);
    ecrire_APPx(jdesc);
    ecrire_COM(jdesc);
    ecrire_DQT(jdesc);
    ecrire_SOF(jdesc);
    ecrire_DHT(jdesc);
    ecrire_SOS(jdesc);
}


void jpeg_write_footer(struct jpeg_desc *jdesc) {
    ecrire_EOI(jdesc);
}

struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc) {
    return jdesc->stream;
}
