#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../include/htables.h"
#include "../include/bitstream.h"
#include "../include/algo_matrices.h"

void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc);
void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc);
void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice);


void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc) {
  uint8_t magnitude;
  uint16_t indice;
  magnitude_indice(nombre, &magnitude, &indice);
  ecrire_codage_huffman(stream, magnitude, DC, cc);
  bitstream_write_nbits(stream, indice, magnitude, false);
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
          bitstream_write_nbits(stream, 0x00, 8, false);
        }
        else if (tab[i] == 0) {
          nbr_coeff0_prec += 1;
        }
        else {
          for (int j = 0; j < (nbr_coeff0_prec / 16); j++) {
            bitstream_write_nbits(stream, 0xf0, 8, false);
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
          nbr_coeff0_prec = 0;
        }
      }
}
/*
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

int main() {
  struct bitstream *stream = bitstream_create("chabanas.jpg");
  ecrire_codage_AC_avec_RLE(stream, matrice_testCr, Cr);
  bitstream_destroy(stream);
  // magnitudeDC_Y(0x004d);
  // printf("%i\n", codage_huffman(7, DC, Y));
}
*/
