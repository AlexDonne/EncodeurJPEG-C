

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct bitstream {
  FILE *fichier;
  uint64_t buffer;
  uint8_t indice;
};

/* Retourne un nouveau bitstream prêt à écrire dans le fichier filename. */
struct bitstream *bitstream_create(const char *filename) {
  struct bitstream *stream = malloc(sizeof(struct bitstream));
  stream->fichier = fopen(filename, "wb");
  stream->buffer = 0;
  stream->indice = 0;
  return stream;
};


/*
    Détruit le bitstream passé en paramètre, en libérant la mémoire qui lui est
    associée.
*/
void bitstream_destroy(struct bitstream *stream) {
  fclose(stream->fichier);
  free(stream);
};

/*
    Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet). Attention nbits doit être compris entre 1 et 32.
*/
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker) {
        if (nbits + stream->indice > 64) {
          /* on écrit les 32 premiers bits du buffer */
          uint32_t buffer1 = stream->buffer >> 32;
          fwrite(&buffer1, sizeof(uint32_t), 1, stream->fichier);
          /* on met à jour le buffer */
          stream->buffer = stream->buffer << 32;
          stream->indice -= 32;
        }
        uint64_t value_64 = value;
        value_64 = (value_64 << (64 - nbits)) >> stream->indice;
        stream->buffer += value_64;
        printf("%lx\n", stream->buffer);
        stream->indice += nbits;
};

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
void bitstream_flush(struct bitstream *stream) {
        /* on recherche le plus petit multiple de 8 supérieur ou égal à stream->indice */
        uint8_t multiple = 8;
        while (stream->indice > multiple) {
          multiple += 8;
        }
        /* on écrit les multiple premiers bits du buffer */
        uint64_t buffer1 = stream->buffer >> (64 - multiple);
        printf("%lx\n", buffer1);
        fwrite(&buffer1, multiple / 8, 1, stream->fichier);
        /* on met à jour le buffer */
        stream->buffer = 0;
        stream->indice = 0;
};

/*
int main() {
  struct bitstream *stream = bitstream_create("test_de_mon_bitstream.ppm");
  // bitstream_write_nbits(stream, 7, 3, false);
  // bitstream_write_nbits(stream, 0, 1, false);
  // bitstream_write_nbits(stream, 1, 1, false);
  // bitstream_write_nbits(stream, 7, 3, false);
  // bitstream_write_nbits(stream, 0, 1, false);
  // bitstream_write_nbits(stream, 1, 1, false);
  // bitstream_flush(stream);
  // bitstream_write_nbits(stream, 7, 3, false);
  // bitstream_write_nbits(stream, 0, 1, false);
  // bitstream_write_nbits(stream, 1, 1, false);
  // bitstream_write_nbits(stream, 7, 3, false);
  // bitstream_write_nbits(stream, 0, 1, false);
  // bitstream_write_nbits(stream, 1, 1, false);
  // bitstream_flush(stream);
  uint32_t buffer = 0x7fff8000;
  bitstream_write_nbits(stream, buffer, 32, false);
  // bitstream_write_nbits(stream, 0x00c0, 16, false);
  // bitstream_write_nbits(stream, 0x00c0, 32, false);
  bitstream_flush(stream);
  bitstream_destroy(stream);
  return EXIT_SUCCESS;
}
*/
