#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bitstream.h"
#include "../include/qtables.h"
#include "../include/htables.h"
#include <string.h>


/*
    Type opaque contenant l'intégralité des informations nécessaires à
    l'écriture de l'entête JPEG.
*/
struct my_jpeg_desc {
  char *ppm_filename;
  char *jpeg_filename;
  char *commentaire;
  uint16_t height;
  uint16_t width;
  uint8_t nb_components;
  uint8_t sampling_factor[3][2];
  uint8_t *quantization_tables[2];
  uint8_t *huffman_tables[3][2];
  uint8_t taille_huffman_table[3][2];
  struct bitstream *stream;
};


struct my_jpeg_desc *my_jpeg_desc_create(void) {
    struct my_jpeg_desc *jdesc = malloc(sizeof(struct my_jpeg_desc));
    jdesc->ppm_filename = NULL;
    jdesc->jpeg_filename = NULL;
    jdesc->commentaire = NULL;
    jdesc->height = 0;
    jdesc->width = 0;
    jdesc->nb_components = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
        jdesc->sampling_factor[i][j] = 0;
      }
    }
    for (int i = 0; i < 2; i++) {
      jdesc->quantization_tables[i] = NULL;
    }
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
        jdesc->huffman_tables[i][j] = NULL;
      }
    }
    jdesc->stream = NULL;
    return jdesc;
}



void my_jpeg_desc_destroy(struct my_jpeg_desc *jdesc) {
  bitstream_destroy(jdesc->stream);
  free(jdesc);
}





void my_jpeg_desc_set_ppm_filename(struct my_jpeg_desc *jdesc, char *ppm_filename) {
  jdesc->ppm_filename = ppm_filename;
}

void my_jpeg_desc_set_comment(struct my_jpeg_desc *jdesc, char *commentaire) {
  jdesc->commentaire = commentaire;
}


void my_jpeg_desc_set_jpeg_filename(struct my_jpeg_desc *jdesc, char *jpeg_filename) {
  jdesc->jpeg_filename = jpeg_filename;
  jdesc->stream = bitstream_create(jpeg_filename);
}


void my_jpeg_desc_set_quantization_table(struct my_jpeg_desc *jdesc, enum color_component cc, uint8_t *qtable) {
  jdesc->quantization_tables[cc] = qtable;
}

void my_jpeg_desc_set_image_height(struct my_jpeg_desc *jdesc, uint16_t image_height) {
  jdesc->height = image_height;
}

void my_jpeg_desc_set_image_width(struct my_jpeg_desc *jdesc, uint16_t image_width) {
  jdesc->width = image_width;
}

void my_jpeg_desc_set_nb_components(struct my_jpeg_desc *jdesc, uint8_t nb_components) {
  jdesc->nb_components = nb_components;
}

void my_jpeg_desc_set_sampling_factor(struct my_jpeg_desc *jdesc, enum color_component cc, enum direction dir, uint8_t sampling_factor) {
  jdesc->sampling_factor[cc][dir] = sampling_factor;
}

void my_jpeg_desc_set_huffman_table(struct my_jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc, uint8_t *htable, uint8_t nb_symbols) {
  /* attention : htable est la concaténation de htables_nb_symb_per_lengths et htables_symbols */
  jdesc->taille_huffman_table[cc][acdc] = nb_symbols + 16;
  jdesc->huffman_tables[cc][acdc] = htable;
}




void ecrire_SOI(struct my_jpeg_desc *jdesc) {
  bitstream_write_nbits(jdesc->stream, 0xffd8, 16, true);
}

void ecrire_APPx(struct my_jpeg_desc *jdesc) {
  bitstream_write_nbits(jdesc->stream, 0xffe0, 16, true);
  bitstream_write_nbits(jdesc->stream, 16, 16, false); // longueur section
  bitstream_write_nbits(jdesc->stream, 0x4a464946, 32, false); // JFIF
  bitstream_write_nbits(jdesc->stream, 0, 8, false); // '\0'
  bitstream_write_nbits(jdesc->stream, 0x0101, 16, false); // doit valoir 1
  bitstream_write_nbits(jdesc->stream, 0, 32, false); // tout mettre à zéro
  bitstream_write_nbits(jdesc->stream, 0, 24, false); // tout mettre à zéro
}

void ecrire_COM(struct my_jpeg_desc *jdesc) {
  if (jdesc->commentaire != NULL) {
    bitstream_write_nbits(jdesc->stream, 0xfffe, 16, true);
    int lg = strlen(jdesc->commentaire);
    bitstream_write_nbits(jdesc->stream, lg + 2, 16, false);
    for (int i = 0; i < lg; i++) {
      bitstream_write_nbits(jdesc->stream, jdesc->commentaire[i], 8, false);
    }
  }
}

void ecrire_DQT(struct my_jpeg_desc *jdesc) {
  for (int i = 0; i < 2; i++) {
    if (jdesc->quantization_tables[i] != NULL) {
      bitstream_write_nbits(jdesc->stream, 0xffdb, 16, true);
      bitstream_write_nbits(jdesc->stream, 67, 16, false); // longueur section
      bitstream_write_nbits(jdesc->stream, 0, 4, false); // precision sur 8 bits
      bitstream_write_nbits(jdesc->stream, i, 4, false); //indice iq de la table de quantification
      for (int k = 0; k < 64; k++) {
        bitstream_write_nbits(jdesc->stream, jdesc->quantization_tables[i][k], 8, false);
      }
    }
  }
}

void ecrire_SOF(struct my_jpeg_desc *jdesc) {
  bitstream_write_nbits(jdesc->stream, 0xffc0, 16, true);
  bitstream_write_nbits(jdesc->stream, 8 + 3*jdesc->nb_components, 16, false); //longueur section
  bitstream_write_nbits(jdesc->stream, 8, 8, false); // toujours 8
  bitstream_write_nbits(jdesc->stream, jdesc->height, 16, false);
  bitstream_write_nbits(jdesc->stream, jdesc->width, 16, false);
  bitstream_write_nbits(jdesc->stream, jdesc->nb_components, 8, false);
  for (int i = 0; i < jdesc->nb_components; i++) {
    bitstream_write_nbits(jdesc->stream, i+1, 8, false);
    bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[i][H], 4, false);
    bitstream_write_nbits(jdesc->stream, jdesc->sampling_factor[i][V], 4, false);
    if (i == 0) {
      bitstream_write_nbits(jdesc->stream, 0, 8, false);
    }
    else {
      bitstream_write_nbits(jdesc->stream, 1, 8, false);
    }
  }
}


void ecrire_DHT(struct my_jpeg_desc *jdesc) {
  int n = (jdesc->nb_components == 1) ? 1 : 2;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 2; j++) {
      bitstream_write_nbits(jdesc->stream, 0xffc4, 16, true);
      bitstream_write_nbits(jdesc->stream, jdesc->taille_huffman_table[i][j] + 3, 16, false); // longueur section
      bitstream_write_nbits(jdesc->stream, 0, 3, false); // non utilisé, doit valoir (0 sinon erreur)
      bitstream_write_nbits(jdesc->stream, j, 1, false); // acdc
      bitstream_write_nbits(jdesc->stream, i, 4, false); // indice
      for (int k = 0; k < jdesc->taille_huffman_table[i][j]; k++) {
        bitstream_write_nbits(jdesc->stream, jdesc->huffman_tables[i][j][k], 8, false);
      }
    }
  }
}

void ecrire_SOS(struct my_jpeg_desc *jdesc) {
  bitstream_write_nbits(jdesc->stream, 0xffda, 16, true);
  bitstream_write_nbits(jdesc->stream, 6 + 2*jdesc->nb_components, 16, false); // longueur section
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
    tab[k] = htables_symbols[k-16];
  }
  return tab;
}

int main() {
  struct my_jpeg_desc *jdesc = my_jpeg_desc_create();
  my_jpeg_desc_set_jpeg_filename(jdesc, "image.pgm");
  my_jpeg_desc_set_ppm_filename(jdesc, "bachanas2.jpg");
  my_jpeg_desc_set_comment(jdesc, "<3 le projet C");
  my_jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
  my_jpeg_desc_set_quantization_table(jdesc, Cb, compressed_CbCr_table);
  my_jpeg_desc_set_image_height(jdesc, 8);
  my_jpeg_desc_set_image_width(jdesc, 8);
  my_jpeg_desc_set_nb_components(jdesc, 3);
  my_jpeg_desc_set_sampling_factor(jdesc, Y, H, 1);
  my_jpeg_desc_set_sampling_factor(jdesc, Y, V, 1);
  my_jpeg_desc_set_sampling_factor(jdesc, Cb, H, 1);
  my_jpeg_desc_set_sampling_factor(jdesc, Cb, V, 1);
  my_jpeg_desc_set_sampling_factor(jdesc, Cr, H, 1);
  my_jpeg_desc_set_sampling_factor(jdesc, Cr, V, 1);
  my_jpeg_desc_set_huffman_table(jdesc, DC, Y, concatener(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]), htables_nb_symbols[DC][Y]);
  my_jpeg_desc_set_huffman_table(jdesc, AC, Y, concatener(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]), htables_nb_symbols[AC][Y]);
  my_jpeg_desc_set_huffman_table(jdesc, DC, Cb, concatener(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]), htables_nb_symbols[DC][Cb]);
  my_jpeg_desc_set_huffman_table(jdesc, AC, Cb, concatener(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]), htables_nb_symbols[AC][Cb]);
  my_jpeg_desc_set_huffman_table(jdesc, DC, Cr, concatener(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]), htables_nb_symbols[DC][Cr]);
  my_jpeg_desc_set_huffman_table(jdesc, AC, Cr, concatener(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]), htables_nb_symbols[AC][Cr]);
  ecrire_SOI(jdesc);
  ecrire_APPx(jdesc);
  ecrire_COM(jdesc);
  ecrire_DQT(jdesc);
  ecrire_SOF(jdesc);
  ecrire_DHT(jdesc);
  ecrire_SOS(jdesc);
  return EXIT_SUCCESS;
}
