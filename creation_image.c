#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
  FILE* fichier = NULL;
  fichier = fopen("image.ppm", "wb");
  uint8_t tab[38];
  tab[0] = 0x50;
  tab[1] = 0x36;
  tab[2] = 0x0a;
  tab[3] = 0x33; //3 colonnes
  tab[4] = 0x20;
  tab[5] = 0x33; // 3 lignes
  tab[6] = 0x0a;
  tab[7] = 0x32;
  tab[8] = 0x35;
  tab[9] = 0x35;
  tab[10] = 0x0a;
  tab[11] = 0x00;
  tab[12] = 0x00;
  tab[13] = 0xff;
  tab[14] = 0xff;
  tab[15] = 0xff;
  tab[16] = 0xff;
  tab[17] = 0xff;
  tab[18] = 0x00;
  tab[19] = 0x00;
  tab[20] = 0x00;
  tab[21] = 0xff;
  tab[22] = 0x00;
  tab[23] = 0xff;
  tab[24] = 0xff;
  tab[25] = 0xff;
  tab[26] = 0xff;
  tab[27] = 0x00;
  tab[28] = 0x00;
  tab[29] = 0x00;
  tab[30] = 0x00;
  tab[31] = 0x00;
  tab[32] = 0x00;
  tab[33] = 0x00;
  tab[34] = 0x00;
  tab[35] = 0xff;
  tab[36] = 0xff;
  tab[37] = 0x00;
  fwrite(tab, sizeof(uint8_t), 38, fichier);
  fclose(fichier);
}
