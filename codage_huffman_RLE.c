#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/htables.h"
#include "../include/bitstream.h"
#include "../algo_matrices.c"
#include "../include/huffman.h"
#include "../include/jpeg_writer.h"
#include <stdbool.h>

void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc);
void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc);
void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice);


void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc) {
  uint8_t magnitude;
  uint16_t indice;
  magnitude_indice(nombre, &magnitude, &indice);
  ecrire_codage_huffman(stream, magnitude, DC, cc);
  bitstream_write_nbits(stream, indice, magnitude, false);
  printf("writing %i over %i bits\n", indice, magnitude);
  //printf("%x\n", (codage_huffman(m, DC, Y) << m) + indice);
}


void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice) {
  /* ecrit dans magnitude un nombre entre 0 et 11 correspondant à la magnitude et dans indice son indice correspondant */
  uint16_t abs_nombre = (nombre < 0) ? -nombre : nombre;
  uint16_t N = 1024;
  *magnitude = 11;
  while (abs_nombre < N) {
    N /= 2;
    *magnitude -= 1;
  }
  *indice = (nombre > 0) ? abs_nombre : 2*N - abs_nombre - 1; // indice dans la classe, le nombre de bits que l'on écrit est exactement la classe
}


void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc) {
      // uint8_t nb_symbols = htables_nb_symbols[acdc][cc];
      uint8_t *tab_nb_symb_per_lengths = htables_nb_symb_per_lengths[acdc][cc];
      uint8_t *tab_symbols = htables_symbols[acdc][cc];
      /* on recherche l'indice de nombre dans tab_symbols */
      int indice = 0;
      while (tab_symbols[indice] != nombre) {
        indice++;
      }
      indice++;
      int current = 0;
      for (int i = 0; i < 16; i++) {
        for (int j = 0; j < tab_nb_symb_per_lengths[i]; j++) {
          indice--;
          if (indice == 0) {
            bitstream_write_nbits(stream, current, i + 1, false);
            printf("writing %i over %i bits\n", current, i+1);
            return;
          }
        current++;
        }
      current = current << 1;
      }
}

void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc) {

      uint8_t nbr_coeff0_prec = 0;
      uint8_t magnitude;
      uint16_t indice;
      uint32_t nombre;

      for (int i = 1; i < 64; i++) { // on commence à 1 car le premier a déjà été codé par codage différenciel DC
        if (tab[i] == 0 && i == 63) {
          ecrire_codage_huffman(stream, 0x00, AC, cc);
          //bitstream_write_nbits(stream, 0x00, 8, false);
        }
        else if (tab[i] == 0) {
          nbr_coeff0_prec += 1;
        }
        else {
          for (int j = 0; j < (nbr_coeff0_prec / 16); j++) {
            ecrire_codage_huffman(stream, 0xf0, AC, cc);
            //bitstream_write_nbits(stream, 0xf0, 8, false);
          }
          nbr_coeff0_prec = nbr_coeff0_prec % 16;
          magnitude_indice(tab[i], &magnitude, &indice);

          nombre = (nbr_coeff0_prec << 4) + magnitude;
          ecrire_codage_huffman(stream, nombre, AC, cc);
          /*
          bitstream_write_nbits(stream, nbr_coeff0_prec, 4, false);
          bitstream_write_nbits(stream, magnitude, 4, false);
          */
          bitstream_write_nbits(stream, indice, magnitude, false);
          printf("writing %i over %i bits\n", indice, magnitude);
          nbr_coeff0_prec = 0;
        }
      }
}



struct bitstream *ecrire_entete(struct jpeg_desc *jdesc, const char *ppm_filename, const char *jpeg_filename, uint32_t image_height, uint32_t image_width, bool couleur) {
  jpeg_desc_set_ppm_filename(jdesc, ppm_filename);
  jpeg_desc_set_jpeg_filename(jdesc, jpeg_filename);
  jpeg_desc_set_image_width(jdesc, image_width);
  jpeg_desc_set_image_height(jdesc, image_height);
  if (couleur) {
    jpeg_desc_set_nb_components(jdesc, 3);
    jpeg_desc_set_sampling_factor(jdesc, Y, H, 1);
    jpeg_desc_set_sampling_factor(jdesc, Y, V, 1);
    jpeg_desc_set_sampling_factor(jdesc, Cr, H, 1);
    jpeg_desc_set_sampling_factor(jdesc, Cr, V, 1);
    jpeg_desc_set_sampling_factor(jdesc, Cb, H, 1);
    jpeg_desc_set_sampling_factor(jdesc, Cb, V, 1);

    struct huff_table *htable1 = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    jpeg_desc_set_huffman_table(jdesc, DC, Y, htable1);
    struct huff_table *htable2 = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    jpeg_desc_set_huffman_table(jdesc, AC, Y, htable2);


    struct huff_table *htable3 = huffman_table_build(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]);
    jpeg_desc_set_huffman_table(jdesc, DC, Cr, htable3);
    struct huff_table *htable4 = huffman_table_build(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]);
    jpeg_desc_set_huffman_table(jdesc, AC, Cr, htable4);


    struct huff_table *htable5 = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
    jpeg_desc_set_huffman_table(jdesc, DC, Cb, htable5);
    struct huff_table *htable6 = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
    jpeg_desc_set_huffman_table(jdesc, AC, Cb, htable6);


    jpeg_desc_set_quantization_table(jdesc, Cb, compressed_CbCr_table);
    jpeg_desc_set_quantization_table(jdesc, Cr, compressed_CbCr_table);
    jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
  }
  else {
    jpeg_desc_set_nb_components(jdesc, 1);
    jpeg_desc_set_sampling_factor(jdesc, Y, H, 1);
    jpeg_desc_set_sampling_factor(jdesc, Y, V, 1);
    struct huff_table *htable1 = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
    jpeg_desc_set_huffman_table(jdesc, DC, Y, htable1);
    struct huff_table *htable2 = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
    jpeg_desc_set_huffman_table(jdesc, AC, Y, htable2);
    jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
  }
  jpeg_write_header(jdesc);
  return jpeg_desc_get_bitstream(jdesc);
}





//page 37
int16_t matrice_test[64] = {
  0xa6, 0xa0, 0x9a, 0x98, 0x9a, 0x9a, 0x96, 0x91,
  0xa0, 0xa3, 0x9d, 0x8e, 0x88, 0x8f, 0x95, 0x94,
  0xa5, 0x97, 0x96, 0xa1, 0x9f, 0x90, 0x90, 0x9e,
  0xa6, 0x9a, 0x91, 0x91, 0x92, 0x90, 0x90, 0x93,
  0xc9, 0xd9, 0xc8, 0x98, 0x85, 0x98, 0xa2, 0x95,
  0xf0, 0xf5, 0xf9, 0xea, 0xbf, 0x98, 0x90, 0x9d,
  0xe9, 0xe1, 0xf3, 0xfd, 0xf2, 0xaf, 0x8a, 0x90,
  0xe6, 0xf2, 0xf1, 0xed, 0xf8, 0xfb, 0xd0, 0x95
};


// page 40 du poly
int16_t matrice_testCr[64] = {
0x000e, 0xffff, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};


int16_t matrice_invader[64] = {
  0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
  0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00,
  0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff
};



int main() {


  afficher_matrice8x8_16(matrice_invader);
  int16_t dct_matrice[64];
  discrete_cosinus_transform(matrice_invader, dct_matrice);
  afficher_matrice8x8_16(dct_matrice);
  int16_t zig_matrice[64];
  zigzag(dct_matrice, zig_matrice);
  afficher_matrice8x8_16(zig_matrice);
  quantificationY(zig_matrice);
  afficher_matrice8x8_16(zig_matrice);

  // Entete



  struct jpeg_desc *jdesc = jpeg_desc_create();
  struct bitstream *stream = ecrire_entete(jdesc, "chabanas.ppm", "bachanas.jpg", 8, 8, false);
  // struct bitstream *stream = bitstream_create("chabanas.jpg");
  ecrire_codage_differenciel_DC(stream, zig_matrice[0], Y);
  ecrire_codage_AC_avec_RLE(stream, zig_matrice, Y);
  //bitstream_destroy(stream);
  jpeg_write_footer(jdesc);
  jpeg_desc_destroy(jdesc);
  // magnitudeDC_Y(0x004d);
  // printf("%i\n", codage_huffman(7, DC, Y));
}
