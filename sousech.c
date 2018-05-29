#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "algo_matrices.c"
#include "my_jpeg_writer.c"
#include "codage_huffman_RLE.c"

struct mcuTransform {
    int16_t **Y;
    int16_t **Cb;
    int16_t **Cr;
    int tailleY;
    int tailleCb;
    int tailleCr;
  };


struct mcusTransformMat {
      int nbcol;
      int nblignes;
      struct mcuTransform *mcus;
};



int16_t *creer_matrice(int16_t tab_image[], int i, int j, int H, int V, int h, int v, bool couleur) {
  /* h et v sont les "vraies" valeurs ; on donne qu'une seule matrice ; i et j sont les indices de début !! */
  int somme;
  int16_t *matrice = malloc(64 * sizeof(int16_t));
  int pas = (couleur) ? 3 : 1;
  int pas_h = h / (8*pas); // nb bloc horizontaux
  int pas_v = v / 8; // nb bloc verticaux
  int indice = 0;

  for (int k = i; k < i + v; k += pas_v) {
    for (int l = j; l < j + h; l+= pas_h*pas) {
      /* on fait la moyenne */
      somme = 0;
      for (int x = k; x < k + pas_v; x++) {
        for (int y = l; y < l + pas_h*pas; y+=pas) {
          somme += tab_image[x*H + y];
          //printf(" k = %i; l = %i; x = %i; y = %i, valeur = %x\n", k, l , x, y, tab_image[x*H + y]);
        }
      }
      //printf("\n");
      matrice[indice] = somme / ((float) (pas_h * pas_v));
      indice++;
    }
  }
  //printf("%i\n", indice);
  return matrice;
}


struct mcuTransform *creer_mcu(int16_t tab_image[], int i, int j, int H, int V, int h, int v, int h1, int v1, int h2, int v2, int h3, int v3, bool couleur) {
  /* h1 et v1 sont les fausses valeurs : par exemple 2x2 ... ; h et v sont les vrais du grand bloc considéré */
  int pas = (couleur) ? 3 : 1;
  int indice;

  int nb_matrices_Y = h1 * v1;
  indice = 0;
  int16_t **matrices_Y = malloc(sizeof(int16_t *) * nb_matrices_Y);
  for (int k = i; k < i + v; k += 8*(v1 / v1)) {
    for (int l = j; l < j + h; l += 8*pas*(h1 / h1)) {
      matrices_Y[indice] = creer_matrice(tab_image, k, l, H, V, h / h1, v / v1, couleur);
      indice++;
    }
  }


  if (couleur) {


    int nb_matrices_Cb = h2 * v2;
    indice = 0;
    int16_t **matrices_Cb = malloc(sizeof(int16_t *) * nb_matrices_Cb);
    //int16_t *matrices_Cb;
    j++; // on se cale sur Cb
    for (int k = i; k < i + v; k += 8*(v1 / v2)) {
      for (int l = j; l < j + h; l += 8*pas*(h1 / h2)) {
        //printf(" k = %i; l = %i", k, l);
        matrices_Cb[indice] = creer_matrice(tab_image, k, l, H, V, h / h2, v / v2, couleur);
        indice++;
      }
    }

    int nb_matrices_Cr = h3 * v3;
    indice = 0;
    int16_t **matrices_Cr = malloc(sizeof(int16_t *) * nb_matrices_Cr);
    j++; // on se cale sur Cr
    for (int k = i; k < i + v; k += 8*(v1 / v3)) {
      for (int l = j; l < j + h; l += 8*pas*(h1 / h3)) {
        matrices_Cr[indice] = creer_matrice(tab_image, k, l, H, V, h / h3, v / v3, couleur);
        indice++;
      }
    }

    struct mcuTransform *mcu_associee = malloc(sizeof(struct mcuTransform));
    mcu_associee->Y = matrices_Y;
    mcu_associee->Cb = matrices_Cb;
    mcu_associee->Cr = matrices_Cr;
    mcu_associee->tailleY = nb_matrices_Y;
    mcu_associee->tailleCb = nb_matrices_Cb;
    mcu_associee->tailleCr = nb_matrices_Cr;
    return mcu_associee;
  }

  else {
    struct mcuTransform *mcu_associee = malloc(sizeof(struct mcuTransform));
    mcu_associee->Y = matrices_Y;
    mcu_associee->tailleY = nb_matrices_Y;
    mcu_associee->tailleCb = 0;
    mcu_associee->tailleCr = 0;
    return mcu_associee;
  }

}



struct mcuTransform *creer_tableau_mcu(int16_t tab_image[], int H, int V, int h1, int v1, int h2, int v2, int h3, int v3, bool couleur) {
  int pas = (couleur) ? 3 : 1;
  int h = h1 * 8 * pas;
  int v = v1 * 8;
  int nbcol =  H / h;
  int nblignes = V / v;
  struct mcuTransform *mcus = malloc(sizeof(struct mcuTransform) * nbcol * nblignes);

  int indice = 0;
  for (int i = 0; i < V; i+=v) {
    for (int j = 0; j < H; j+=h) {
        mcus[indice] = *creer_mcu(tab_image, i, j, H, V, h, v, h1, v1, h2, v2, h3, v3, couleur);
        indice++;
    }
  }
  return mcus;
}

struct mcusTransformMat *creer_mcusTransformMat(int16_t tab_image[], int H, int V, int h1, int v1, int h2, int v2, int h3, int v3, bool couleur) {
  int pas = (couleur) ? 3 : 1;
  int h = h1 * 8 * pas;
  int v = v1 * 8;
  int nbcol =  H / h;
  int nblignes = V / v;

  struct mcusTransformMat *mon_mcu_transform_mat = malloc(sizeof(struct mcusTransformMat));
  mon_mcu_transform_mat->nbcol = nbcol;
  mon_mcu_transform_mat->nblignes = nblignes;
  mon_mcu_transform_mat->mcus = creer_tableau_mcu(tab_image, H, V, h1, v1, h2, v2, h3, v3, couleur);
  return mon_mcu_transform_mat;
};




/*

// tableau de la page 35
uint32_t tab_test[128] = {
0xb8a092, 0xb19b8d, 0xa79787, 0xa29785, 0xa19a88, 0xa19a88, 0x9e9586, 0x9e8d83, 0xad9990, 0xb59995, 0xb89c99, 0xb99d9a, 0xb59d9b, 0xb29f9b, 0xafa29c, 0xb2a39e,
0xb29b8d, 0xb49e90, 0xaa9a8a, 0x988d7b, 0x8f8876, 0x968f7d, 0x9d9485, 0x9f9184, 0xad9990, 0xb39a95, 0xb89c98, 0xb99d9a, 0xb59d99, 0xb29f99, 0xafa29c, 0xb1a49e,
0xb6a092, 0xa69384, 0xa39384, 0xab9f8f, 0xa69f8d, 0x97907e, 0x979080, 0xa99b8e, 0xac9a90, 0xb39a93, 0xb89c98, 0xb99d99, 0xb59d99, 0xb29f99, 0xb0a39b, 0xb1a49c,
0xb7a194, 0xa99688, 0x9c8f7f, 0x99907f, 0x999282, 0x979080, 0x979080, 0x9e9083, 0xac9a90, 0xb39a93, 0xb69d98, 0xb79e99, 0xb59d99, 0xb29f99, 0xafa29a, 0xafa59c,
0xdac4b7, 0xe8d5c7, 0xd3c5b8, 0xa09788, 0x8b8575, 0x9e9888, 0xa9a194, 0x9f9387, 0xab998f, 0xb19a92, 0xb59c97, 0xb69d98, 0xb29e97, 0xb09f97, 0xaca298, 0xada59a,
0xffebe0, 0xfff3e7, 0xfff9ec, 0xf2e9da, 0xc5bfaf, 0x9e9888, 0x969082, 0xa79b8f, 0xa8978d, 0xaf9890, 0xb39a93, 0xb49b94, 0xb09c95, 0xae9d95, 0xaba197, 0xaba398,
0xf8e4d9, 0xefddd1, 0xfdf1e3, 0xffffef, 0xf8f2e4, 0xb5afa1, 0x908a7c, 0x9a8e82, 0xa6958b, 0xad968e, 0xb09991, 0xb19a92, 0xae9a91, 0xac9b91, 0xa99f93, 0xaaa094,
0xf5e1d8, 0xffeee4, 0xfbefe3, 0xf4ecdf, 0xfef8ea, 0xfffbed, 0xd6d0c2, 0x9f9387, 0xa5948a, 0xac958d, 0xaf9890, 0xb09991, 0xad9990, 0xab9a90, 0xa89e92, 0xa99f93
};

int16_t *transformation_tab_test(uint32_t tab_test[]) {
  int16_t *tab_test2 = malloc(sizeof(int16_t)*384); // 3*8*16
  for (int i = 0; i < 8*16; i++) {
    tab_test2[3*i + 2] = tab_test[i] & 0x0000ff;
    tab_test2[3*i + 1] = (tab_test[i] & 0x00ff00) >> 8;
    tab_test2[3*i] = (tab_test[i] & 0xff0000) >> 16;
  }
  return tab_test2;
}

*/

void RGB_to_YCbCr(int16_t tab_test[], int nb_elts) {
  int16_t y, cb, cr;
  for (int i = 0; i < nb_elts; i+=3) {
    y = 0.299 * tab_test[i] + 0.578 * tab_test[i + 1] + 0.114 * tab_test[i + 2];
    cb = -0.1687 * tab_test[i] - 0.3313 * tab_test[i + 1] + 0.5 * tab_test[i + 2] + 128;
    cr = 0.5 * tab_test[i] - 0.4187 * tab_test[i + 1] -0.0813 * tab_test[i + 2] + 128;
    tab_test[i] = y;
    tab_test[i + 1] = cb;
    tab_test[i + 2] = cr;
  }
}


int16_t *agrandir(int16_t tab[], int *nb_ligne, int *nb_col, int h1, int v1, bool couleur) {
  int pas = (couleur) ? 3 : 1;

  int new_nb_ligne = 0;
  while (new_nb_ligne < *nb_ligne) {
    new_nb_ligne += 8*v1;
  }
  int new_nb_col = 0;
  while (new_nb_col < *nb_col) {
    new_nb_col += 8*pas*h1;
  }
  int16_t *new_tab = calloc(new_nb_col * new_nb_ligne, sizeof(int16_t));

  for (int i = 0; i < *nb_ligne; i++) {
    for (int j = 0; j < *nb_col; j++) {
      new_tab[i*new_nb_col + j] = tab[i*(*nb_col) + j];
    }
  }

  *nb_ligne = new_nb_ligne;
  *nb_col = new_nb_col;
  return new_tab;
};

int main() {
  FILE* fichier1 = fopen("horizontal.ppm", "rb");
  char lettre = fgetc(fichier1);
  char chiffre = fgetc(fichier1); // 5 ou 6
  printf("%c%c\n", lettre, chiffre);
  fgetc(fichier1); //0x0a

  int nb_col = 0;
  char a;
  while ((a = fgetc(fichier1)) != ' ') {
    nb_col = 10*nb_col + (a - '0');
  }

  int nb_ligne = 0;
  while ((a = fgetc(fichier1)) != '\n') {
    nb_ligne = 10*nb_ligne + (a - '0');
  }

  printf("(%d, %d)\n", nb_col, nb_ligne);

  fgetc(fichier1); //0x32
  fgetc(fichier1); //0x35
  fgetc(fichier1); //0x35
  fgetc(fichier1); //0x0a

  int h1 = 1;
  int v1 = 1;
  int h2 = 1;
  int v2 = 1;
  int h3 = 1;
  int v3 = 1;
  bool couleur = true;
  int pas = (couleur) ? 3 : 1;

  nb_col *= pas;

  uint8_t tab2[nb_col*nb_ligne];
  fread(tab2, sizeof(uint8_t), nb_col*nb_ligne, fichier1);
  fclose(fichier1);

  // passage de uint8_t à int16_t (maladroit)
  int16_t tab3[nb_col*nb_ligne];
  for (int i = 0; i < nb_col*nb_ligne; i++) {
    tab3[i] = tab2[i];
  }





  int16_t *tab = agrandir(tab3, &nb_ligne, &nb_col, h1, v1, couleur);

  if (couleur) {
  RGB_to_YCbCr(tab, nb_col*nb_ligne);
  }





  int nbcol = nb_col / (8*pas*h1);
  int nblignes = nb_ligne /(8*v1);


  // struct mcuTransform *mcus = creer_tableau_mcu(tab, nb_col, nb_ligne, 2, 2, 1, 1, 1, 1, true);
  // for (int i = 0; i<nbcol; i++) {
  //   for (int j = 0; j<nblignes; j++) {
  //     afficher_matrice8x8_16(mcus[i*nblignes + j].Cb[0]);
  //   }
  // }


  struct mcusTransformMat *mon_mcu_transform_mat = creer_mcusTransformMat(tab, nb_col, nb_ligne, h1, v1, h2, v2, h3, v3, couleur);



  int tailleY = mon_mcu_transform_mat->mcus[0].tailleY;
  int tailleCb = mon_mcu_transform_mat->mcus[0].tailleCb;
  int tailleCr = mon_mcu_transform_mat->mcus[0].tailleCr;
  for (int i = 0; i<mon_mcu_transform_mat->nbcol; i++) {
    for (int j = 0; j<mon_mcu_transform_mat->nblignes; j++) {
      for (int k = 0; k < tailleY; k++) {
        fast_discrete_cosinus_transform(mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k], tab_cosinus);
        zigzag_en_place(mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k]);
        quantificationY(mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k]);
      }
      for (int k = 0; k < tailleCb; k++) {
        fast_discrete_cosinus_transform(mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k], tab_cosinus);
        zigzag_en_place(mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k]);
        quantificationCbCr(mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k]);
      }
      for (int k = 0; k < tailleCr; k++) {
        fast_discrete_cosinus_transform(mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k], tab_cosinus);
        zigzag_en_place(mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k]);
        quantificationCbCr(mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k]);
      }

    }
  }




    struct my_jpeg_desc *jdesc = my_jpeg_desc_create();
    my_jpeg_desc_set_jpeg_filename(jdesc, "bachanas3.jpg");
    my_jpeg_desc_set_ppm_filename(jdesc, "horizontal.ppm" );
    my_jpeg_desc_set_comment(jdesc, "<3 le projet C");
    my_jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
    if (couleur) {
      my_jpeg_desc_set_quantization_table(jdesc, Cb, compressed_CbCr_table);
    }

    my_jpeg_desc_set_image_height(jdesc, nb_ligne);
    my_jpeg_desc_set_image_width(jdesc, nb_col/pas);
    my_jpeg_desc_set_nb_components(jdesc, pas);
    my_jpeg_desc_set_sampling_factor(jdesc, Y, H, h1);
    my_jpeg_desc_set_sampling_factor(jdesc, Y, V, v1);
    if (couleur) {
    my_jpeg_desc_set_sampling_factor(jdesc, Cb, H, h2);
    my_jpeg_desc_set_sampling_factor(jdesc, Cb, V, v2);
    my_jpeg_desc_set_sampling_factor(jdesc, Cr, H, h3);
    my_jpeg_desc_set_sampling_factor(jdesc, Cr, V, v3);
    }
    my_jpeg_desc_set_huffman_table(jdesc, DC, Y, concatener(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]), htables_nb_symbols[DC][Y]);
    my_jpeg_desc_set_huffman_table(jdesc, AC, Y, concatener(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]), htables_nb_symbols[AC][Y]);
    if (couleur) {
    my_jpeg_desc_set_huffman_table(jdesc, DC, Cb, concatener(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]), htables_nb_symbols[DC][Cb]);
    my_jpeg_desc_set_huffman_table(jdesc, AC, Cb, concatener(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]), htables_nb_symbols[AC][Cb]);
    my_jpeg_desc_set_huffman_table(jdesc, DC, Cr, concatener(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]), htables_nb_symbols[DC][Cr]);
    my_jpeg_desc_set_huffman_table(jdesc, AC, Cr, concatener(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]), htables_nb_symbols[AC][Cr]);
    }
    my_jpeg_write_header(jdesc);

    struct bitstream *stream = my_jpeg_desc_get_bitstream(jdesc);


    int16_t valDCYprec = 0;
    int16_t valDCCbprec = 0;
    int16_t valDCCrprec = 0;
    for (int i = 0; i<mon_mcu_transform_mat->nbcol; i++) {
      for (int j = 0; j<mon_mcu_transform_mat->nblignes; j++) {
        for (int k = 0; k < tailleY; k++) {
            ecrire_codage_differenciel_DC(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k][0] - valDCYprec, Y);
            valDCYprec = mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k][0];
            ecrire_codage_AC_avec_RLE(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Y[k], Y);
        }
        for (int k = 0; k < tailleCb; k++) {
          ecrire_codage_differenciel_DC(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k][0] - valDCCbprec, Cb);
          valDCCbprec = mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k][0];
          ecrire_codage_AC_avec_RLE(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[k], Cb);

        }
        for (int k = 0; k < tailleCr; k++) {
          ecrire_codage_differenciel_DC(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k][0] - valDCCrprec, Cr);
          valDCCrprec = mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k][0];
          ecrire_codage_AC_avec_RLE(stream, mon_mcu_transform_mat->mcus[i*nblignes + j].Cr[k], Cr);

        }
      }
    }

    my_jpeg_write_footer(jdesc);
    my_jpeg_desc_destroy(jdesc);


  // for (int i = 0; i<mon_mcu_transform_mat->nbcol; i++) {
  //   for (int j = 0; j<mon_mcu_transform_mat->nblignes; j++) {
  //     afficher_matrice8x8_16(mon_mcu_transform_mat->mcus[i*nblignes + j].Cb[0]);
  //   }
  // }
  return EXIT_SUCCESS;
}
