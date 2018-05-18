#include "../include/codage_huffman_RLE.h"
#include "../include/bitstream.h"

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
    *indice = (nombre > 0) ? abs_nombre : 2 * N - abs_nombre -
                                          1; // indice dans la classe, le nombre de bits que l'on écrit est exactement la classe
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
                printf("writing %i over %i bits\n", current, i + 1);
                return;
            }
            current++;
        }
        current = current << 1;
    }
}

void ecrire_codage_AC_avec_RLE(struct bitstream *stream, int16_t *tab, enum color_component cc, int num) {

    uint8_t nbr_coeff0_prec = 0;
    uint8_t magnitude;
    uint16_t indice;
    uint32_t nombre;
    printf("#%i\n", num);
    for (int i = 1; i < 64; i++) { // on commence à 1 car le premier a déjà été codé par codage différenciel DC
        if (tab[i] == 0 && i == 63) {
            ecrire_codage_huffman(stream, 0x00, AC, cc);
            //bitstream_write_nbits(stream, 0x00, 8, false);
        } else if (tab[i] == 0) {
            nbr_coeff0_prec += 1;
        } else {
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
    printf("\n");
}


struct bitstream * ecrire_entete_gris(struct jpeg_desc *jdesc, const char *jpeg_filename, uint32_t image_height, uint32_t image_width) {
    jpeg_desc_set_ppm_filename(jdesc, "chabanas.ppm");
    jpeg_desc_set_jpeg_filename(jdesc, "a.jpg");
    jpeg_desc_set_image_width(jdesc, image_width);
    jpeg_desc_set_image_height(jdesc, image_height);
    jpeg_desc_set_nb_components(jdesc, 1);
    jpeg_desc_set_sampling_factor(jdesc, Y, H, 1);
    jpeg_desc_set_sampling_factor(jdesc, Y, V, 1);
    struct huff_table *htable1 = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y],
                                                     htables_nb_symbols[DC][Y]);
    jpeg_desc_set_huffman_table(jdesc, DC, Y, htable1);
    struct huff_table *htable2 = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y],
                                                     htables_nb_symbols[AC][Y]);
    jpeg_desc_set_huffman_table(jdesc, AC, Y, htable2);
    jpeg_desc_set_quantization_table(jdesc, Y, compressed_Y_table);
    jpeg_write_header(jdesc);
    return jpeg_desc_get_bitstream(jdesc);
}


void ecrire_jpeg(ImagePPM *image, MCUsTransformMat *mcusTransform) {
    struct jpeg_desc *jdesc = jpeg_desc_create();
    struct bitstream *stream = ecrire_entete_gris(jdesc, image->chemin, image->hauteur, image->largeur);
    if (image->type == RGB) {
        for (int i = 0; i < mcusTransform->nbcol * mcusTransform->nblignes; ++i) {
            ecrire_codage_differenciel_DC(stream, mcusTransform->mcus[i].Y[0][0], Y);
            ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Y[0], Y,i);

            ecrire_codage_differenciel_DC(stream, mcusTransform->mcus[i].Cr[0], Cr);
            ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Cr, Cr, i);

            ecrire_codage_differenciel_DC(stream, mcusTransform->mcus[i].Cb[0], Cb);
            ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Cb, Cb, i);
        }
    }
    else {
        for (int i = 0; i < mcusTransform->nbcol * mcusTransform->nblignes; ++i) {
            int16_t dc;
            if (i==0){
                dc = mcusTransform->mcus[i].Y[0][0];
            }
            else {
                dc = mcusTransform->mcus[i].Y[0][0] - mcusTransform->mcus[i-1].Y[0][0];
            }
            ecrire_codage_differenciel_DC(stream, dc, Y);
            ecrire_codage_AC_avec_RLE(stream, mcusTransform->mcus[i].Y[0], Y, i);
        }
    }
    libererImage(image);
    libererMCUsTransform(mcusTransform);
    jpeg_write_footer(jdesc);
    jpeg_desc_destroy(jdesc);
}

/**
 * Libère l'espace mémoire d'une ImagePPM
 * @param image
 */
void libererImage(ImagePPM *image) {
    if (image->type == RGB) {
        for (int i = 0; i < image->hauteur; i++) {
            free(image->pixelsRGB[i]);
        }
        free(image->pixelsRGB);
    } else {
        for (int i = 0; i < image->hauteur; i++) {
            free(image->pixelsNB[i]);
        }
        free(image->pixelsNB);
    }
    free(image);
}

void libererMCUsTransform(MCUsTransformMat *mcUsTransform){
    for (int i = 0; i < mcUsTransform->nblignes * mcUsTransform->nbcol; ++i) {
        libererMCUTransform(mcUsTransform->mcus[i]);
    }
    free(mcUsTransform->mcus);
    free(mcUsTransform);
}

void libererMCUTransform(MCUTransform mcuTransform){
    for (int i = 0; i < mcuTransform.tailleY; ++i) {
        free(mcuTransform.Y);
    }
}