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

struct bitstream *bitstream_create(const char *filename);

void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker);

void bitstream_flush(struct bitstream *stream);

void bitstream_destroy(struct bitstream *stream);

#endif //ETU_MY_BITSTREAM_H
