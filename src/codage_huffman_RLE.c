#include "../include/codage_huffman_RLE.h"
#include "../include/htables.h"

/* Ecrit le codage differentiel DC pour la valeur nombre selon la composante cc */
void ecrire_codage_differenciel_DC(struct bitstream *stream, int16_t nombre, enum color_component cc) {
    uint8_t magnitude;
    uint16_t indice;
    magnitude_indice(nombre, &magnitude, &indice);
    ecrire_codage_huffman(stream, magnitude, DC, cc);
    bitstream_write_nbits(stream, indice, magnitude, false);
    //printf("writing %i over %i bits\n", indice, magnitude);
}

/* Ecrit dans magnitude un nombre entre 0 et 11 correspondant à la magnitude de nombre et dans indice son indice correspondant */
void magnitude_indice(int16_t nombre, uint8_t *magnitude, uint16_t *indice) {
    uint16_t abs_nombre = (nombre < 0) ? -nombre : nombre;
    uint16_t N = 1024;
    *magnitude = 11;
    while (abs_nombre < N) {
        N /= 2;
        *magnitude -= 1;
    }
    *indice = (nombre > 0) ? abs_nombre : 2 * N - abs_nombre -
                                          1; // indice dans la classe, le nombre de bits que l'on écrit est exactement la classe
}

/* Ecrit le codage Huffman de nombre selon les tables de Huffman associées au sample_type acdc et de la color_component cc */
void ecrire_codage_huffman(struct bitstream *stream, uint32_t nombre, enum sample_type acdc, enum color_component cc) {
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

/* Ecrit le codage AC avec RLE pour le bloc 8x8 tab */
void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc) {

    uint8_t nbr_coeff0_prec = 0;
    uint8_t magnitude;
    uint16_t indice;
    uint32_t nombre;
    for (int i = 1; i < 64; i++) { // on commence à 1 car le premier a déjà été codé par codage différenciel DC
        if (tab[i] == 0 && i == 63) {
            ecrire_codage_huffman(stream, 0x00, AC, cc);
        } else if (tab[i] == 0) {
            nbr_coeff0_prec += 1;
        } else {
            for (int j = 0; j < (nbr_coeff0_prec / 16); j++) {
                ecrire_codage_huffman(stream, 0xf0, AC, cc);
            }
            nbr_coeff0_prec = nbr_coeff0_prec % 16;
            magnitude_indice(tab[i], &magnitude, &indice);

            nombre = (nbr_coeff0_prec << 4) + magnitude;
            ecrire_codage_huffman(stream, nombre, AC, cc);
            bitstream_write_nbits(stream, indice, magnitude, false);
            nbr_coeff0_prec = 0;
        }
    }
}

/* Ecrit toute l'entête jpeg */
struct bitstream *ecrire_entete(struct jpeg_desc *jdesc, const char *ppm_filename, const char *jpeg_filename,
                                uint32_t image_height, uint32_t image_width, bool couleur, int h1, int l1, int h2, int l2, int h3, int l3) {
    jpeg_desc_set_ppm_filename(jdesc, ppm_filename);
    jpeg_desc_set_jpeg_filename(jdesc, jpeg_filename);
    jpeg_desc_set_image_width(jdesc, image_width);
    jpeg_desc_set_image_height(jdesc, image_height);
    jpeg_desc_set_comment(jdesc, "<3 le projet C");
    if (couleur) {
        jpeg_desc_set_nb_components(jdesc, 3);
        jpeg_desc_set_sampling_factor(jdesc, Y, H, h1);
        jpeg_desc_set_sampling_factor(jdesc, Y, V, l1);
        jpeg_desc_set_sampling_factor(jdesc, Cr, H, h2);
        jpeg_desc_set_sampling_factor(jdesc, Cr, V, l2);
        jpeg_desc_set_sampling_factor(jdesc, Cb, H, h3);
        jpeg_desc_set_sampling_factor(jdesc, Cb, V, l3);


        jpeg_desc_set_huffman_table(jdesc, DC, Y,
                                    concatener(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y],
                                               htables_nb_symbols[DC][Y]), htables_nb_symbols[DC][Y]);
        jpeg_desc_set_huffman_table(jdesc, AC, Y,
                                    concatener(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y],
                                               htables_nb_symbols[AC][Y]), htables_nb_symbols[AC][Y]);
        jpeg_desc_set_huffman_table(jdesc, DC, Cb,
                                    concatener(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb],
                                               htables_nb_symbols[DC][Cb]), htables_nb_symbols[DC][Cb]);
        jpeg_desc_set_huffman_table(jdesc, AC, Cb,
                                    concatener(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb],
                                               htables_nb_symbols[AC][Cb]), htables_nb_symbols[AC][Cb]);
        jpeg_desc_set_huffman_table(jdesc, DC, Cr,
                                    concatener(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr],
                                               htables_nb_symbols[DC][Cr]), htables_nb_symbols[DC][Cr]);
        jpeg_desc_set_huffman_table(jdesc, AC, Cr,
                                    concatener(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr],
                                               htables_nb_symbols[AC][Cr]), htables_nb_symbols[AC][Cr]);

        jpeg_desc_set_quantization_table(jdesc, Cb, compressed_CbCr_table);
        jpeg_desc_set_quantization_table(jdesc, Cr, compressed_CbCr_table);
        jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
    } else {
        jpeg_desc_set_nb_components(jdesc, 1);
        jpeg_desc_set_sampling_factor(jdesc, Y, H, 1);
        jpeg_desc_set_sampling_factor(jdesc, Y, V, 1);
        jpeg_desc_set_huffman_table(jdesc, DC, Y,
                                    concatener(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y],
                                               htables_nb_symbols[DC][Y]), htables_nb_symbols[DC][Y]);
        jpeg_desc_set_huffman_table(jdesc, AC, Y,
                                    concatener(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y],
                                               htables_nb_symbols[AC][Y]), htables_nb_symbols[AC][Y]);
        jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
    }
    jpeg_write_header(jdesc);
    return jpeg_desc_get_bitstream(jdesc);
}

/* Réalise toutes les étapes pour écrire le jpeg */
void ecrire_jpeg(ImagePPM *image, MCUsTransformMat *mcusTransform, int h1, int l1, int h2, int l2, int h3, int l3) {
    struct jpeg_desc *jdesc = jpeg_desc_create();
    if (image->type == RGB) {
        struct bitstream *stream = ecrire_entete(jdesc, image->chemin, image->nom, image->hauteur, image->largeur,
                                                 true, h1, l1, h2, l2, h3, l3);
        int16_t dcy, dccb, dccr;
        for (int i = 0; i < mcusTransform->nbcol * mcusTransform->nblignes; ++i) {

            for (int j = 0; j < mcusTransform->mcus[i].tailleY; ++j) {
                if (i == 0 && j == 0) {
                    dcy = mcusTransform->mcus[i].Y[j][0];
                } else {
                    if (j == 0) {
                        dcy = mcusTransform->mcus[i].Y[j][0] -
                              mcusTransform->mcus[i - 1].Y[mcusTransform->mcus[i].tailleY - 1][0];
                    } else {
                        dcy = mcusTransform->mcus[i].Y[j][0] - mcusTransform->mcus[i].Y[j - 1][0];
                    }
                }
                ecrire_codage_differenciel_DC(stream, dcy, Y);
                ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Y[j], Y);
            }


            for (int k = 0; k < mcusTransform->mcus[i].tailleCb; ++k) {
                if (i == 0 && k == 0) {

                    dccb = mcusTransform->mcus[i].Cb[k][0];
                } else {
                    if (k == 0) {
                        dccb = mcusTransform->mcus[i].Cb[k][0] -
                               mcusTransform->mcus[i - 1].Y[mcusTransform->mcus[i].tailleCb - 1][0];
                    } else {
                        dccb = mcusTransform->mcus[i].Cb[k][0] - mcusTransform->mcus[i].Cb[k - 1][0];

                    }
                }
                ecrire_codage_differenciel_DC(stream, dccb, Cb);
                ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Cb[k], Cb);
            }

            for (int l = 0; l < mcusTransform->mcus[i].tailleCr; ++l) {
                if (i == 0 && l == 0) {
                    dccr = mcusTransform->mcus[i].Cr[l][0];

                } else {
                    if (l == 0) {
                        dccr = mcusTransform->mcus[i].Cr[l][0] -
                               mcusTransform->mcus[i - 1].Y[mcusTransform->mcus[i].tailleCr - 1][0];
                    } else {
                        dccr = mcusTransform->mcus[i].Cr[l][0] - mcusTransform->mcus[i].Cr[l - 1][0];

                    }
                }
                ecrire_codage_differenciel_DC(stream, dccr, Cr);
                ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Cr[l], Cr);
            }

        }
    } else {
        struct bitstream *stream = ecrire_entete(jdesc, image->nom, image->nom, image->hauteur, image->largeur, false, h1, l1, h2, l2, h3, l3);
        int16_t dcy;
        for (int i = 0; i < mcusTransform->nbcol * mcusTransform->nblignes; ++i) {
            if (i == 0) {
                dcy = mcusTransform->mcus[i].Y[0][0];
            } else {
                dcy = mcusTransform->mcus[i].Y[0][0] -
                      mcusTransform->mcus[i - 1].Y[0][0];
            }
            ecrire_codage_differenciel_DC(stream, dcy, Y);
            ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Y[0], Y);
        }

        libererImage(image);
        libererMCUsTransform(mcusTransform);
        jpeg_write_footer(jdesc);
        jpeg_desc_destroy(jdesc);
    }
}

/**
 * Libère l'espace mémoire d'une ImagePPM
 * @param image
 */
void libererImage(ImagePPM *image) {
    free(image->nom);
    free(image);
}

/* Libère l'espace mémoire d'une MCUsTransformMat */
void libererMCUsTransform(MCUsTransformMat *mcUsTransform) {
    for (int i = 0; i < mcUsTransform->nblignes * mcUsTransform->nbcol; ++i) {
        libererMCUTransform(&(mcUsTransform->mcus[i]));
    }
    free(mcUsTransform->mcus);
    free(mcUsTransform);
}

/* Libère l'espace mémoire d'une MCUTransform */
void libererMCUTransform(MCUTransform *mcuTransform) {
    for (int i = 0; i < mcuTransform->tailleY; ++i) {
        free(mcuTransform->Y[i]);
    }
    free(mcuTransform->Y);
    if (mcuTransform->Cb != NULL) {
        for (int i = 0; i < mcuTransform->tailleCb; ++i) {
            free(mcuTransform->Cb[i]);
        }
        free(mcuTransform->Cb);
        for (int i = 0; i < mcuTransform->tailleCr; ++i) {
            free(mcuTransform->Cr[i]);
        }
        free(mcuTransform->Cr);
    }
}
