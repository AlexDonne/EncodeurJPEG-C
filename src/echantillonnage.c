#include "../include/structures.h"
#include "../include/echantillonnage.h"
#include <math.h>
#include <stdbool.h>

MCUsMatrice *fusion_RGB(MCUsMatrice *mcus_rgb, int h1, int l1) {
    if (h1 > 2 || l1 > 2){
        printf("Echantillonnage non géré\n");
        exit(EXIT_FAILURE);
    }
    if ((h1 == 1 && l1 == 1) || mcus_rgb->type == NB) {

        return mcus_rgb;
    } else {
        MCUsMatrice *fusionmatrgb = malloc(sizeof(MCUsMatrice));
        test_malloc(fusionmatrgb);
        fusionmatrgb->nbcol = mcus_rgb->nbcol / h1;
        fusionmatrgb->nblignes = mcus_rgb->nblignes / l1;
        fusionmatrgb->mcus = malloc(fusionmatrgb->nbcol * fusionmatrgb->nblignes * sizeof(MCUPixels));
        test_malloc(fusionmatrgb->mcus);
        for (int i = 0; i < fusionmatrgb->nblignes; ++i) {
            for (int j = 0; j < fusionmatrgb->nbcol; ++j) {
                fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB = malloc(h1 * l1 * sizeof(PixelRGB*));
                test_malloc(fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB);
                fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsNB = NULL;

                if (h1 == 2 && l1 == 1) {
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[0] = mcus_rgb->mcus[
                            i * mcus_rgb->nbcol + 2 * j].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[1] = mcus_rgb->mcus[
                            i * mcus_rgb->nbcol + 2 * j +
                            1].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].tailleBlocs = 2;
                }
                if (h1 == 2 && l1 == 2) {
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[0] = mcus_rgb->mcus[
                            (2 * i) * mcus_rgb->nbcol +
                            2 * j].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[1] = mcus_rgb->mcus[
                            (2 * i) * mcus_rgb->nbcol +
                            2 * j + 1].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[2] = mcus_rgb->mcus[
                            (2 * i + 1) * mcus_rgb->nbcol +
                            2 * j].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[3] = mcus_rgb->mcus[
                            (2 * i + 1) * mcus_rgb->nbcol +
                            2 * j + 1].blocsRGB[0];

                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].tailleBlocs = 4;

                }
                if (h1 == 1 && l1 == 2) {
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[0] = mcus_rgb->mcus[
                            2 * i * mcus_rgb->nbcol +
                            j].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].blocsRGB[1] = mcus_rgb->mcus[
                            (2 * i + 1) * mcus_rgb->nbcol
                            + j].blocsRGB[0];
                    fusionmatrgb->mcus[i * fusionmatrgb->nbcol + j].tailleBlocs = 2;

                }
            }
        }

        fusionmatrgb->type = mcus_rgb->type;
        for (int k = 0; k < mcus_rgb->nblignes * mcus_rgb->nbcol; ++k) {
            free(mcus_rgb->mcus[k].blocsRGB);
        }
        free(mcus_rgb->mcus);
        free(mcus_rgb);
        return fusionmatrgb;
    }
}

void echan_h(int16_t *mat1, int16_t *mat2, int16_t *echan_mat) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (j < 4) {
                echan_mat[i * 8 + j] = round((mat1[i * 8 + 2 * j] + mat1[i * 8 + 2 * j + 1])) / 2;
            } else {
                echan_mat[i * 8 + j] = round((mat2[i * 8 + 2 * (j - 4)] + mat2[i * 8 + 2 * (j - 4) + 1])) / 2;
            }
        }
    }
}

void echan_v(int16_t *mat1, int16_t *mat2, int16_t *echan_mat) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i < 4) {
                echan_mat[i * 8 + j] = round((mat1[2 * i * 8 + j] + mat1[(2 * i + 1) * 8 + j])) / 2;
            } else {
                echan_mat[i * 8 + j] = round((mat2[2 * (i - 4) * 8 + j] + mat2[(2 * (i - 4) + 1) * 8 + j])) / 2;
            }
        }
    }
}

void echan_h_v(int16_t *mat1, int16_t *mat2, int16_t *mat3, int16_t *mat4, int16_t *echan_mat) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (j < 4) {
                if (i < 4) {
                    echan_mat[i * 8 + j] =
                            round((mat1[(2 * i) * 8 + 2 * j] + mat1[(2 * i) * 8 + 2 * j + 1] +
                                   mat1[(2 * i + 1) * 8 + 2 * j] + mat1[(2 * i + 1) * 8 + 2 * j + 1])) / 4;

                } else {
                    echan_mat[i * 8 + j] =
                            round((mat3[(2 * (i - 4)) * 8 + 2 * j] + mat3[(2 * (i - 4)) * 8 + 2 * j + 1] +
                                   mat3[(2 * (i - 4) + 1) * 8 + 2 * j] + mat3[(2 * (i - 4) + 1) * 8 + 2 * j + 1])) / 4;

                }
            } else {
                if (i < 4) {
                    echan_mat[i * 8 + j] =
                            round((mat2[(2 * i) * 8 + 2 * (j - 4)] + mat2[(2 * i) * 8 + 2 * (j - 4) + 1] +
                                   mat2[(2 * i + 1) * 8 + 2 * (j - 4)] +
                                   mat2[(2 * i + 1) * 8 + 2 * (j - 4) + 1])) / 4;

                } else {
                    echan_mat[i * 8 + j] =
                            round((mat4[(2 * (i - 4)) * 8 + 2 * (j - 4)] + mat4[(2 * (i - 4)) * 8 + 2 * (j - 4) + 1] +
                                   mat4[(2 * (i - 4) + 1) * 8 + 2 * (j - 4)] +
                                   mat4[(2 * (i - 4) + 1) * 8 + 2 * (j - 4) + 1])) / 4;

                }

            }
        }
    }
}


void mcu_echantillonnage(MCUTransform *mcu, MCUTransform *echan_mcu, int h1, int l1, int h2, int l2, int h3, int l3) {
    echan_mcu->tailleCb = h2 * l2;
    echan_mcu->tailleCr = h3 * l3;
    echan_mcu->tailleY = mcu->tailleY;
    echan_mcu->Y = mcu->Y;
    bool echantillonnageok = false;
    //Allocation des mcus

    if (h1 == h2 && l1 == l2) {
        echan_mcu->Cb = mcu->Cb;
        echantillonnageok = true;
    }
    else {
        echan_mcu->Cb = malloc(echan_mcu->tailleCb * sizeof(void*));
        for (int i = 0; i < echan_mcu->tailleCb; ++i) {
            echan_mcu->Cb[i] = malloc(64 * sizeof(int16_t));
        }
    }
    if (h1 == h3 && l1 == l3) {
        echan_mcu->Cr = mcu->Cr;
        echantillonnageok = true;
    }
    else {
        echan_mcu->Cr = malloc(echan_mcu->tailleCr * sizeof(void*));
        for (int j = 0; j < echan_mcu->tailleCr; ++j) {
            echan_mcu->Cr[j] = malloc(64 * sizeof(int16_t));

        }
    }

    if (h1 == 2 && l1 == 1) {
        if (h2 == 1 && l2 == 1) {
            echan_h(mcu->Cb[0],
                    mcu->Cb[1],
                    echan_mcu->Cb[0]);
            echantillonnageok = true;
        }
        if (h3 == 1 && l3 == 1) {
            echan_h(mcu->Cr[0],
                    mcu->Cr[1],
                    echan_mcu->Cr[0]);
            echantillonnageok = true;
        }
    }
    if (h1 == 1 && l1 == 2) {
        if (h2 == 1 && l2 == 1) {
            echan_v(mcu->Cb[0],
                    mcu->Cb[1],
                    echan_mcu->Cb[0]);
            echantillonnageok = true;
        }
        if (h3 == 1 && l3 == 1) {
            echan_v(mcu->Cr[0],
                    mcu->Cr[1],
                    echan_mcu->Cr[0]);
            echantillonnageok = true;
        }

    }
    if (h1 == 2 && l1 == 2) {
        if (h2 == 1 && l2 == 2) {
            echan_h(mcu->Cb[0], mcu->Cb[1], echan_mcu->Cb[0]);
            echan_h(mcu->Cb[2], mcu->Cb[3], echan_mcu->Cb[1]);
            echantillonnageok = true;
        }
        if (h3 == 1 && l3 == 2) {
            echan_h(mcu->Cr[0], mcu->Cr[1], echan_mcu->Cr[0]);
            echan_h(mcu->Cr[2], mcu->Cr[3], echan_mcu->Cr[1]);
            echantillonnageok = true;
        }
        if (h3 == 1 && l3 == 1) {
            echan_h_v(mcu->Cr[0],
                      mcu->Cr[1], mcu->Cr[2], mcu->Cr[3],
                      echan_mcu->Cr[0]);
            echantillonnageok = true;
        }
        if (h2 == 1 && l2 == 1) {
            echan_h_v(mcu->Cb[0],
                      mcu->Cb[1], mcu->Cb[2], mcu->Cb[3],
                      echan_mcu->Cb[0]);
            echantillonnageok = true;
        }
        if (h2 == 2 && l2 == 1) {
            echan_v(mcu->Cb[0], mcu->Cb[2], echan_mcu->Cb[0]);
            echan_v(mcu->Cb[1], mcu->Cb[3], echan_mcu->Cb[1]);
            echantillonnageok = true;
        }
        if (h3 == 2 && l3 == 1) {
            echan_v(mcu->Cr[0], mcu->Cr[2], echan_mcu->Cr[0]);
            echan_v(mcu->Cr[1], mcu->Cr[3], echan_mcu->Cr[1]);
            echantillonnageok = true;
        }
    }
    if (!echantillonnageok){
        printf("Format d'échantillonnage pas géré\n");
        exit(EXIT_FAILURE);
    }
}


void mat_mcu_echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int h1, int l1, int h2, int l2,
                             int h3, int l3) {
    for (int i = 0; i < matmcu->nblignes * matmcu->nbcol; ++i) {
        mcu_echantillonnage(&(matmcu->mcus[i]), &(echan_matmcu->mcus[i]), h1, l1, h2, l2, h3, l3);
    }


}


MCUsTransformMat *echantillonnage(MCUsTransformMat *matmcu, int h1, int l1, int h2, int l2, int h3, int l3) {
    if (matmcu->mcus->tailleCb == 0) {
        return matmcu;
    }
    if (l1 == l2 && h1 == h2 && l1 == l3 && h1 == h3) {
        //pas d'echantillonnage
        return matmcu;

    } else {
        bool libererCb = h1 == h2 && l1 == l2;
        bool libererCr = h1 == h3 && l1 == l3;
        MCUsTransformMat *echan_matmcu = malloc(sizeof(MCUsTransformMat));
        echan_matmcu->nblignes = matmcu->nblignes;
        echan_matmcu->nbcol = matmcu->nbcol;
        echan_matmcu->mcus = malloc(echan_matmcu->nbcol * echan_matmcu->nblignes * sizeof(MCUTransform));
        mat_mcu_echantillonnage(matmcu, echan_matmcu, h1, l1, h2, l2, h3, l3);
        if (!libererCb){
            for (int i = 0; i < matmcu->nbcol * matmcu->nblignes; ++i) {
                libererCbs(&(matmcu->mcus[i]));
            }
        }
        if (!libererCr){
            for (int i = 0; i < matmcu->nbcol * matmcu->nblignes; ++i) {
                libererCrs(&(matmcu->mcus[i]));
            }
        }
        free(matmcu->mcus);
        free(matmcu);
        return echan_matmcu;
    }
}

