#include "../include/algo_matrices.h"

const uint8_t ordre_zigzag[64] = {
        0x00,
        0x01, 0x10,
        0x20, 0x11, 0x02,
        0x03, 0x12, 0x21, 0x30,
        0x40, 0x31, 0x22, 0x13, 0x04,
        0x05, 0x14, 0x23, 0x32, 0x41, 0x50,
        0x60, 0x51, 0x42, 0x33, 0x24, 0x15, 0x06,
        0x07, 0x16, 0x25, 0x34, 0x43, 0x52, 0x61, 0x70,
        0x71, 0x62, 0x53, 0x44, 0x35, 0x26, 0x17,
        0x27, 0x36, 0x45, 0x54, 0x63, 0x72,
        0x73, 0x64, 0x55, 0x46, 0x37,
        0x47, 0x56, 0x65, 0x74,
        0x75, 0x66, 0x57,
        0x67, 0x76,
        0x77
};


float tab_cosinus[6] = {
        1.387039845322147524342426549992524087429046630859375,  //  sqrt(2) * cos(M_PI/16)
        1.306562964876376575773520016809925436973571777343750,  //   sqrt(2) * cos(M_PI/8)
        1.175875602419358845196484253392554819583892822265625,  //   sqrt(2) * cos(3*M_PI/16)
        0.785694958387102349028907610772876068949699401855469,  //   sqrt(2) * cos(5*M_PI/16)
        0.541196100146197123237357118341606110334396362304688,  //   sqrt(2) * cos(3*M_PI/8)
        0.275899379282943113533832502071163617074489593505859   //   sqrt(2) * cos(7*M_PI/16)
};

#ifndef M_PI
#define M_PI 3.14159265
#endif

/**
 * Applique le zig zag sur une matrice
 * @param matrice
 * @param zig_matrice
 */
void zigzag(int16_t *matrice, int16_t *zig_matrice) {
    for (int i = 0; i < 64; i++) {
        zig_matrice[i] = matrice[(ordre_zigzag[i] >> 4) * 8 + (ordre_zigzag[i] & 7)];
    }
}


/**
 * Applique le DCT sur une matrice, en évitant les calculs de cosinus
 * @param matrice
 * @param dct_matrice
 */
void fast_discrete_cosinus_transform(int16_t *matrice) {
    float dct_matrice[64];
    for (int i = 0; i < 64; i++) {
        matrice[i] -= 128;
    }
    for (int j = 0; j < 8; j++) {
        dct_matrice[0 * 8 + j] =
                matrice[0 * 8 + j] + matrice[1 * 8 + j] + matrice[2 * 8 + j] + matrice[3 * 8 + j] + matrice[4 * 8 + j] +
                matrice[5 * 8 + j] + matrice[6 * 8 + j] + matrice[7 * 8 + j];
        dct_matrice[1 * 8 + j] = tab_cosinus[0] * (matrice[0 * 8 + j] - matrice[7 * 8 + j]) -
                                 tab_cosinus[2] * (matrice[6 * 8 + j] - matrice[1 * 8 + j]) +
                                 tab_cosinus[3] * (matrice[2 * 8 + j] - matrice[5 * 8 + j]) -
                                 tab_cosinus[5] * (matrice[4 * 8 + j] - matrice[3 * 8 + j]);
        dct_matrice[2 * 8 + j] =
                tab_cosinus[1] * (matrice[0 * 8 + j] + matrice[7 * 8 + j] - matrice[3 * 8 + j] - matrice[4 * 8 + j]) +
                tab_cosinus[4] * (matrice[1 * 8 + j] + matrice[6 * 8 + j] - matrice[2 * 8 + j] - matrice[5 * 8 + j]);
        dct_matrice[3 * 8 + j] = tab_cosinus[2] * (matrice[0 * 8 + j] - matrice[7 * 8 + j]) +
                                 tab_cosinus[5] * (matrice[6 * 8 + j] - matrice[1 * 8 + j]) -
                                 tab_cosinus[0] * (matrice[2 * 8 + j] - matrice[5 * 8 + j]) +
                                 tab_cosinus[3] * (matrice[4 * 8 + j] - matrice[3 * 8 + j]);
        dct_matrice[4 * 8 + j] =
                matrice[0 * 8 + j] - matrice[1 * 8 + j] - matrice[2 * 8 + j] + matrice[3 * 8 + j] + matrice[4 * 8 + j] -
                matrice[5 * 8 + j] - matrice[6 * 8 + j] + matrice[7 * 8 + j];
        dct_matrice[5 * 8 + j] = tab_cosinus[3] * (matrice[0 * 8 + j] - matrice[7 * 8 + j]) +
                                 tab_cosinus[0] * (matrice[6 * 8 + j] - matrice[1 * 8 + j]) +
                                 tab_cosinus[5] * (matrice[2 * 8 + j] - matrice[5 * 8 + j]) -
                                 tab_cosinus[2] * (matrice[4 * 8 + j] - matrice[3 * 8 + j]);
        dct_matrice[6 * 8 + j] =
                tab_cosinus[4] * (matrice[0 * 8 + j] + matrice[7 * 8 + j] - matrice[3 * 8 + j] - matrice[4 * 8 + j]) -
                tab_cosinus[1] * (matrice[1 * 8 + j] + matrice[6 * 8 + j] - matrice[2 * 8 + j] - matrice[5 * 8 + j]);
        dct_matrice[7 * 8 + j] = tab_cosinus[5] * (matrice[0 * 8 + j] - matrice[7 * 8 + j]) +
                                 tab_cosinus[3] * (matrice[6 * 8 + j] - matrice[1 * 8 + j]) +
                                 tab_cosinus[2] * (matrice[2 * 8 + j] - matrice[5 * 8 + j]) +
                                 tab_cosinus[0] * (matrice[4 * 8 + j] - matrice[3 * 8 + j]);
    }
    for (int i = 0; i < 8; i++) {
        matrice[i * 8 + 0] = 0.125 * (dct_matrice[i * 8 + 0] + dct_matrice[i * 8 + 1] + dct_matrice[i * 8 + 2] +
                                      dct_matrice[i * 8 + 3] + dct_matrice[i * 8 + 4] + dct_matrice[i * 8 + 5] +
                                      dct_matrice[i * 8 + 6] + dct_matrice[i * 8 + 7]);
        matrice[i * 8 + 1] = 0.125 * (tab_cosinus[0] * (dct_matrice[i * 8 + 0] - dct_matrice[i * 8 + 7]) -
                                      tab_cosinus[2] * (dct_matrice[i * 8 + 6] - dct_matrice[i * 8 + 1]) +
                                      tab_cosinus[3] * (dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]) -
                                      tab_cosinus[5] * (dct_matrice[i * 8 + 4] - dct_matrice[i * 8 + 3]));
        matrice[i * 8 + 2] = 0.125 * (tab_cosinus[1] *
                                      (dct_matrice[i * 8 + 0] + dct_matrice[i * 8 + 7] - dct_matrice[i * 8 + 3] -
                                       dct_matrice[i * 8 + 4]) + tab_cosinus[4] *
                                                                 (dct_matrice[i * 8 + 1] + dct_matrice[i * 8 + 6] -
                                                                  dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]));
        matrice[i * 8 + 3] = 0.125 * (tab_cosinus[2] * (dct_matrice[i * 8 + 0] - dct_matrice[i * 8 + 7]) +
                                      tab_cosinus[5] * (dct_matrice[i * 8 + 6] - dct_matrice[i * 8 + 1]) -
                                      tab_cosinus[0] * (dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]) +
                                      tab_cosinus[3] * (dct_matrice[i * 8 + 4] - dct_matrice[i * 8 + 3]));
        matrice[i * 8 + 4] = 0.125 * (dct_matrice[i * 8 + 0] - dct_matrice[i * 8 + 1] - dct_matrice[i * 8 + 2] +
                                      dct_matrice[i * 8 + 3] + dct_matrice[i * 8 + 4] - dct_matrice[i * 8 + 5] -
                                      dct_matrice[i * 8 + 6] + dct_matrice[i * 8 + 7]);
        matrice[i * 8 + 5] = 0.125 * (tab_cosinus[3] * (dct_matrice[i * 8 + 0] - dct_matrice[i * 8 + 7]) +
                                      tab_cosinus[0] * (dct_matrice[i * 8 + 6] - dct_matrice[i * 8 + 1]) +
                                      tab_cosinus[5] * (dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]) -
                                      tab_cosinus[2] * (dct_matrice[i * 8 + 4] - dct_matrice[i * 8 + 3]));
        matrice[i * 8 + 6] = 0.125 * (tab_cosinus[4] *
                                      (dct_matrice[i * 8 + 0] + dct_matrice[i * 8 + 7] - dct_matrice[i * 8 + 3] -
                                       dct_matrice[i * 8 + 4]) - tab_cosinus[1] *
                                                                 (dct_matrice[i * 8 + 1] + dct_matrice[i * 8 + 6] -
                                                                  dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]));
        matrice[i * 8 + 7] = 0.125 * (tab_cosinus[5] * (dct_matrice[i * 8 + 0] - dct_matrice[i * 8 + 7]) +
                                      tab_cosinus[3] * (dct_matrice[i * 8 + 6] - dct_matrice[i * 8 + 1]) +
                                      tab_cosinus[2] * (dct_matrice[i * 8 + 2] - dct_matrice[i * 8 + 5]) +
                                      tab_cosinus[0] * (dct_matrice[i * 8 + 4] - dct_matrice[i * 8 + 3]));
    }
}


/**
 * Applique la quantification pour Y sur une matrice
 * @param matrice
 */
void quantificationY(int16_t *matrice) {
    for (int i = 0; i < 64; i++) {
        matrice[i] = matrice[i] / compressed_Y_table[i];
    }
}

/**
 * Applique la quantification pour Cb ou Cr sur une matrice
 * @param matrice
 */
void quantificationCbCr(int16_t *matrice) {
    for (int i = 0; i < 64; i++) {
        matrice[i] = matrice[i] / compressed_CbCr_table[i];
    }
}