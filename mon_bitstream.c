#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct bitstream {
  FILE *fichier;
  uint8_t buffer;
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
    Ecrit nbits bits dans le bitstream. La valeur portée par cet ensemble de
    bits est value. Le paramètre is_marker permet d'indiquer qu'on est en train
    d'écrire un marqueur de section dans l'entête JPEG ou non (voir section
    2.10.4 du sujet). Attention nbits doit être compris entre 1 et 32.
*/
void bitstream_write_nbits(struct bitstream *stream, uint32_t value, uint8_t nbits, bool is_marker) {
        if (is_marker) {
          bitstream_flush(stream);
          stream->buffer = 0;
          stream->indice = 0;
          fwrite(&value, sizeof(uint8_t), 1, stream->fichier);
        }
        else {
          if (nbits + stream->indice <= 8) {
            stream->buffer += (value << (32 - nbits)) >> (24 + stream->indice);
            stream->indice += nbits;
          }
          else {
            /* on écrit la première partie de value */
            uint8_t value1 = (value << (32 - nbits)) >> (24 + stream->indice);
            value1 += stream->buffer;
            fwrite(&value1, sizeof(uint8_t), 1, stream->fichier);

            /* gestion du byte stuffing */
            if (value1 == 0xff) {
              value1 = 0;
              fwrite(&value1, sizeof(uint8_t), 1, stream->fichier);
            }

            /* on efface les bits au delà de nbits */
            nbits -= 8 - stream->indice;
            uint32_t value2 = (value << (32 - nbits)) >> (32 - nbits);

            /* on écrit la seconde partie de value */
            stream->buffer = 0;
            stream->indice = 0;
            bitstream_write_nbits(stream, value2, nbits, is_marker);
          }
        }


};

/*
    Force l'exécution des écritures en attente sur le bitstream, s'il en
    existe.
*/
void bitstream_flush(struct bitstream *stream) {
        fwrite(&(stream->buffer), sizeof(uint8_t), 1, stream->fichier);
        stream->buffer = 0;
        stream->indice = 0;
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

int main() {
  struct bitstream *stream = bitstream_create("test_de_mon_bitstream.ppm");
  // On veut écrire la suite de bits suivante : 1110-1011-0100-0111-1100-0110-00
  /*
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 2, 3, false); // 010
  bitstream_write_nbits(stream, 14, 3, false); // 110
  bitstream_write_nbits(stream, 1, 1, false); // 1
  bitstream_write_nbits(stream, 7, 6, false); // 00 0111
  bitstream_write_nbits(stream, 5, 24, false); // 11000
  bitstream_write_nbits(stream, 5, 24, false); // 11000
  */
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 15, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 0, 3, false); // 111
  bitstream_write_nbits(stream, 1, 31, false); // 111
  bitstream_destroy(stream);
  return EXIT_SUCCESS;
}
