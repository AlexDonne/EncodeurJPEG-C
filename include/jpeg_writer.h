//
// Created by donnea on 5/23/18.
//

#ifndef ETU_JPEG_WRITER_H
#define ETU_JPEG_WRITER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mon_bitstream.h"
#include "qtables.h"
#include "test_malloc.h"
#include <string.h>

/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component {
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

/*
    Type énuméré représentant les types de composantes fréquentielles (DC ou
    AC).
*/
enum sample_type {
    DC,
    AC,
    NB_SAMPLE_TYPES
};

/*
    Type énuméré représentant la direction des facteurs d'échantillonnage (H
    pour horizontal, V pour vertical).
*/
enum direction {
    H,
    V,
    NB_DIRECTIONS
};


struct jpeg_desc {
    const char *ppm_filename;
    const char *jpeg_filename;
    char *commentaire;
    uint16_t height;
    uint16_t width;
    uint8_t nb_components;
    uint8_t sampling_factor[3][2];
    uint8_t *quantization_tables[3];
    uint8_t *huffman_tables[3][2];
    uint8_t taille_huffman_table[3][2];
    struct bitstream *stream;
};

/**
 * Alloue et retourne une nouvelle structure jpeg_desc.
 * @return
 */
struct jpeg_desc *jpeg_desc_create(void);

/**
 * Détruit un jpeg_desc. Toute la mémoire qui lui est
    associée est libérée.
 * @param jdesc
 */
void jpeg_desc_destroy(struct jpeg_desc *jdesc);

/**
 * Ecrit le nom de fichier PPM ppm_filename dans le jpeg_desc jdesc.
 * @param jdesc
 * @param ppm_filename
 */
void jpeg_desc_set_ppm_filename(struct jpeg_desc *jdesc, const char *ppm_filename);

/**
 * Ecrit le nom du fichier de sortie jpeg_filename dans le jpeg_desc jdesc.
 * @param jdesc
 * @param jpeg_filename
 */
void jpeg_desc_set_jpeg_filename(struct jpeg_desc *jdesc, const char *jpeg_filename);

/**
* Ecrit dans le jpeg_desc jdesc le commentaire à écrire dans le fichier jpeg.
* @param jdesc
* @param commentaire
*/
void jpeg_desc_set_comment(struct jpeg_desc *jdesc, char *commentaire);

/**
* Ecrit dans le jpeg_desc jdesc la table de quantification qtable à utiliser
pour compresser les coefficients de la composante de couleur cc.
* @param jdesc
* @param cc
* @param qtable
*/
void jpeg_desc_set_quantization_table(struct jpeg_desc *jdesc, enum color_component cc, uint8_t *qtable);

/**
 * Ecrit la hauteur en nombre de pixels de l'image traitée image_height dans
    le jpeg_desc jdesc.
 * @param jdesc
 * @param image_height
 */
void jpeg_desc_set_image_height(struct jpeg_desc *jdesc, uint16_t image_height);

/**
* Ecrit la largeur en nombre de pixels de l'image traitée image_width dans le
jpeg_desc jdesc.
* @param jdesc
* @param image_width
*/
void jpeg_desc_set_image_width(struct jpeg_desc *jdesc, uint16_t image_width);

/**
* Ecrit le nombre de composantes de couleur de l'image traitée nb_components
dans le jpeg_desc jdesc.
* @param jdesc
* @param nb_components
*/
void jpeg_desc_set_nb_components(struct jpeg_desc *jdesc, uint8_t nb_components);

/**
 *  Ecrit dans le jpeg_desc jdesc le facteur d'échantillonnage sampling_factor
    à utiliser pour la composante de couleur cc et la direction dir.
 * @param jdesc
 * @param cc
 * @param dir
 * @param sampling_factor
 */
void jpeg_desc_set_sampling_factor(struct jpeg_desc *jdesc, enum color_component cc, enum direction dir,
                                   uint8_t sampling_factor);

/**
* Ecrit dans le jpeg_desc jdesc la table de Huffman htable à utiliser
pour encoder les données de la composante fréquentielle acdc pour la
        composante de couleur cc.
* @param jdesc
* @param acdc
* @param cc
* @param htable
* @param nb_symbols
*/
void jpeg_desc_set_huffman_table(struct jpeg_desc *jdesc, enum sample_type acdc, enum color_component cc,
                                 uint8_t *htable, uint8_t nb_symbols);


void ecrire_SOI(struct jpeg_desc *jdesc);


void ecrire_APPx(struct jpeg_desc *jdesc);


void ecrire_COM(struct jpeg_desc *jdesc);


void ecrire_DQT(struct jpeg_desc *jdesc);


void ecrire_SOF(struct jpeg_desc *jdesc);


void ecrire_DHT(struct jpeg_desc *jdesc);


void ecrire_EOI(struct jpeg_desc *jdesc);


void ecrire_SOS(struct jpeg_desc *jdesc);

/**
 *  Concatene les deux tableaux htables_nb_symb_per_lengths et htables_symbols
 * @param htables_nb_symb_per_lengths
 * @param htables_symbols
 * @param nb_symbols
 * @return
 */
uint8_t *concatener(uint8_t *htables_nb_symb_per_lengths, uint8_t *htables_symbols, uint8_t nb_symbols);


void jpeg_write_header(struct jpeg_desc *jdesc);


void jpeg_write_footer(struct jpeg_desc *jdesc);

/**
 * Retourne le bitstream associé au fichier de sortie enregistré dans le
    jpeg_desc.
 * @param jdesc
 * @return
 */
struct bitstream *jpeg_desc_get_bitstream(struct jpeg_desc *jdesc);


#endif //ETU_JPEG_WRITER_H
