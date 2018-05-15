#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main() {
  FILE* fichier1 = fopen("image.ppm", "rb");
  FILE* fichier2 = fopen("image2.ppm", "wb");


  char lettre = fgetc(fichier1);
  char chiffre = fgetc(fichier1);
  printf("%c%c\n", lettre, chiffre);
  fgetc(fichier1);
  uint8_t nb_col = fgetc(fichier1) - '0';
  fgetc(fichier1);
  uint8_t nb_ligne = fgetc(fichier1) - '0';
  printf("(%d, %d)\n", nb_col, nb_ligne);

  fgetc(fichier1);
  fgetc(fichier1);
  fgetc(fichier1);
  fgetc(fichier1);
  fgetc(fichier1);

  uint8_t tab[3*nb_col*nb_ligne];

  fread(tab, sizeof(uint8_t), 3*nb_col*nb_ligne, fichier1);




  fwrite(tab, sizeof(uint8_t), 3*nb_col*nb_ligne, fichier2);
  fclose(fichier1);
  fclose(fichier2);
}
