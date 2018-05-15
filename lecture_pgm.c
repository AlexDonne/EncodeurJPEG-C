#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void getbloc88(int i0, int j0, int nb_col) {
  for (int i = i0; i < i0 + 8; i++) {
    for (int j = j0; j < j0 + 8; j ++) {
      printf("%d\n", i*nb_col + j);
    }
  }
}

int main() {
  FILE* fichier1 = fopen("gris.pgm", "rb");
  char lettre = fgetc(fichier1);
  char chiffre = fgetc(fichier1); // vaut 5 normalement
  printf("%c%c\n", lettre, chiffre);
  fgetc(fichier1); //0x0a

  uint32_t nb_col = 0;
  char a;
  while ((a = fgetc(fichier1)) != ' ') {
    nb_col = 10*nb_col + (a - '0');
  }

  uint32_t nb_ligne = 0;
  while ((a = fgetc(fichier1)) != '\n') {
    nb_ligne = 10*nb_ligne + (a - '0');
  }

  printf("(%d, %d)\n", nb_col, nb_ligne);

  fgetc(fichier1); //0x32
  fgetc(fichier1); //0x35
  fgetc(fichier1); //0x35
  fgetc(fichier1); //0x0a

  uint8_t tab[nb_col*nb_ligne];
  fread(tab, sizeof(uint8_t), nb_col*nb_ligne, fichier1);

  uint8_t tab2[nb_col*nb_ligne];

  nb_col = 16;
  nb_ligne = 16;
  for (int i = 0; i < nb_ligne; i += 8) {
    for (int j = 0; j < nb_col; j += 8) {
      getbloc88(i, j, nb_col);
    }
  }



  fclose(fichier1);
}
