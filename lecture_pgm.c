#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main() {
  FILE* fichier1 = fopen("mon_image.pgm", "rb");
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
  // tab contient tous les pixels de l'image de départ

  uint8_t tab2[nb_col*nb_ligne];
  int indice = 0;
  for (int i = 0; i < nb_ligne; i += 8) {
    for (int j = 0; j < nb_col; j += 8) {
      for (int i_b = i; i_b < i + 8; i_b++) {
        for (int j_b = j; j_b < j + 8; j_b++) {
          tab2[indice] = tab[i_b*nb_col + j_b];
          indice += 1;
        }
      }
    }
  }
  FILE* fichier2 = fopen("image_bloc.pgm", "wb");
  fwrite(tab2, sizeof(uint8_t), nb_col*nb_ligne, fichier2);
  // tab2 contient tous les pixels mais réorganisés par bloc de 8*8

  fclose(fichier1);
  fclose(fichier3);
}
