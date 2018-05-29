#ifndef ETU_MY_BITSTREAM_H
#define ETU_MY_BITSTREAM_H

#include "test_malloc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct bitstream {
    FILE *fichier;
    uint8_t buffer;
    uint8_t indice;
};


/**
 * Retourne un nouveau bitstream prêt à écrire dans le fichier filename.
 * @param filename
 * @return
 */
struct bitstream *bitstream_create(const char *filename);

/**
 * Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet). Attention nbits doit être compris entre 1 et 32.
 * @param stream
 * @param value
 * @param nbits
 * @param is_marker
 */
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker);

/**
 *
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
 * @param stream
 */
void bitstream_flush(struct bitstream *stream);

/**
 *  Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
 * @param stream
 */
void bitstream_destroy(struct bitstream *stream);

#endif //ETU_MY_BITSTREAM_H
