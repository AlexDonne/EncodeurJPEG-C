#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mon_bitstream.h"

/* Retourne un nouveau bitstream prêt à écrire dans le fichier filename. */
struct bitstream *bitstream_create(const char *filename) {
    struct bitstream *stream = malloc(sizeof(struct bitstream));
    stream->fichier = fopen(filename, "wb");
    stream->buffer = 0;
    stream->indice = 0;
    return stream;
};

/*
    Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet). Attention nbits doit être compris entre 1 et 32.
*/
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker) {
    if (is_marker) {
        bitstream_flush(stream);
    }
    uint8_t value1;
    while (nbits + stream->indice > 8) {
        /* on écrit la première partie de value */
        value1 = (value << (32 - nbits)) >> (24 + stream->indice);
        value1 += stream->buffer;

        fwrite(&value1, sizeof(uint8_t), 1, stream->fichier);

        if (value1 == 0xff && !is_marker) {
            value1 = 0;
            fwrite(&value1, sizeof(uint8_t), 1, stream->fichier);
        }

        /* on efface les bits au delà de nbits */
        nbits -= (8 - stream->indice);
        value = (value << (32 - nbits)) >> (32 - nbits);

        stream->buffer = 0;
        stream->indice = 0;
    }
    stream->buffer += (value << (32 - nbits)) >> (24 + stream->indice);
    stream->indice += nbits;
    if (stream->indice == 8) {
        fwrite(&(stream->buffer), sizeof(uint8_t), 1, stream->fichier);
        stream->buffer = 0;
        stream->indice = 0;
    }
};

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
void bitstream_flush(struct bitstream *stream) {
    if (stream->indice != 0) {
        fwrite(&(stream->buffer), sizeof(uint8_t), 1, stream->fichier);
        stream->buffer = 0;
        stream->indice = 0;
    }
};


/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
void bitstream_destroy(struct bitstream *stream) {
    bitstream_flush(stream);
    fclose(stream->fichier);
    free(stream);
};
