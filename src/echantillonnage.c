#include "../include/structures.h"
#include "../include/echantillonnage.h"
#include <math.h>
MCUsTransformMat *fusion_mcu(MCUsTransformMat *matmcu, int h1, int l1) {
    if (h1 == 1 && l1 == 1) {
        return matmcu;
    } else {
        MCUsTransformMat *fusionmatmcu = malloc(sizeof(MCUsTransformMat));
        if (h1 == 2 && l1 == 1) {
            fusionmatmcu->nbcol = matmcu->nbcol / 2;
            fusionmatmcu->nblignes = matmcu->nblignes;
            printf("%d %d\n", fusionmatmcu->nblignes, fusionmatmcu->nbcol);
            fusionmatmcu->mcus = malloc(fusionmatmcu->nbcol * fusionmatmcu->nblignes * sizeof(MCUTransform));
            for (int i = 0; i < fusionmatmcu->nblignes; ++i) {
                for (int j = 0; j < fusionmatmcu->nbcol; ++j) {
                    //fusion des blocs 8x8 pour avoir un mcu de taille 2x1
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y = malloc(2 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb = malloc(2 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr = malloc(2 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleC = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleY = 2;
                    for (int k = 0; k < 2; ++k) {
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[k] = malloc(64 * sizeof(int16_t));
                    }
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[0] = matmcu->mcus[i * matmcu->nbcol + 2 * j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[1] = matmcu->mcus[i * matmcu->nbcol + 2 * j +
                                                                                        1].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[0] = matmcu->mcus[i * matmcu->nbcol +
                                                                                         2 * j].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[0] = matmcu->mcus[i * matmcu->nbcol +
                                                                                         2 * j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[1] = matmcu->mcus[i * matmcu->nbcol + 2 * j +
                                                                                         1].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[1] = matmcu->mcus[i * matmcu->nbcol + 2 * j +
                                                                                         1].Cb[0];
                }
            }
        }
        if (h1 == 1 && l1 == 2) {
            fusionmatmcu->nbcol = matmcu->nbcol;
            fusionmatmcu->nblignes = matmcu->nblignes / 2;
            fusionmatmcu->mcus = malloc(fusionmatmcu->nbcol * fusionmatmcu->nblignes * sizeof(MCUTransform));
            for (int i = 0; i < fusionmatmcu->nbcol; ++i) {
                for (int j = 0; j < fusionmatmcu->nbcol; ++j) {
                    //fusion des blocs 8x8 pour avoir un mcu de taille 2x1;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleY = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleC = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y = malloc(2 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb = malloc(2 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr = malloc(2 * sizeof(int16_t));
                    for (int k = 0; k < 2; ++k) {
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[k] = malloc(64 * sizeof(int16_t));
                    }
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[0] = matmcu->mcus[2 * i * fusionmatmcu->nbcol +
                                                                                        j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[1] = matmcu->mcus[
                            (2 * i + 1) * fusionmatmcu->nbcol + j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[0] = matmcu->mcus[i * fusionmatmcu->nbcol +
                                                                                         j].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[0] = matmcu->mcus[i * fusionmatmcu->nbcol +
                                                                                         j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[1] = matmcu->mcus[
                            (2 * i + 1) * fusionmatmcu->nbcol + j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[1] = matmcu->mcus[
                            (2 * i + 1) * fusionmatmcu->nbcol + j].Cb[0];

                }
            }
        }

        if (h1 == 2 & l1 == 2) {

            fusionmatmcu->nbcol = (matmcu->nbcol) / 2 ;

            fusionmatmcu->nblignes = matmcu->nblignes / 2 ;
            printf("%d; %d", fusionmatmcu->nbcol, fusionmatmcu->nblignes);
            fusionmatmcu->mcus = malloc(fusionmatmcu->nbcol * fusionmatmcu->nblignes * sizeof(MCUTransform));
            for (int i = 0; i < fusionmatmcu->nblignes; ++i) {
                for (int j = 0; j < fusionmatmcu->nbcol; ++j) {
                    //fusion des blocs 8x8 pour avoir un mcu de taille 2x2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleY = 4;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleC = 4;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y = malloc(4 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb = malloc(4 * sizeof(int16_t));
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr = malloc(4 * sizeof(int16_t));
                    for (int k = 0; k < 4; ++k) {
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[k] = malloc(64 * sizeof(int16_t));
                        fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[k] = malloc(64 * sizeof(int16_t));
                    }
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[0] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                        2 * j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[1] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                        2 * j + 1].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[2] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                        2 * j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[3] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                        2 * j + 1].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[0] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                         2 * j].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[1] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                         2 * j + 1].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[2] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                         2 * j].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[3] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                         2 * j + 1].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[0] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                         2 * j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[1] = matmcu->mcus[(2 * i) * matmcu->nbcol +
                                                                                         2 * j + 1].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[2] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                         2 * j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[3] = matmcu->mcus[(2 * i + 1) * matmcu->nbcol +
                                                                                         2 * j + 1].Cr[0];

                }
            }
        }
        return fusionmatmcu;
    }
}
void echan_h(int16_t *mat1, int16_t *mat2, int16_t *echan_mat) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (j<4) {
                echan_mat[i * 8 + j] = round((mat1[i * 8 + 2 * j] + mat1[i * 8 + 2 * j + 1])) / 2;
            } else {
                echan_mat[i * 8 + j] = round((mat2[i * 8 + 2 * (j-4)] + mat2[i * 8 + 2 *(j-4) + 1])) / 2;
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


void mcu_echan_h(MCUTransform *mcu, MCUTransform *echan_mcu) {
    echan_mcu->tailleC = 1;
//    printf("%d\n", mcu->tailleC);
    echan_mcu->tailleY = mcu->tailleY;
    echan_mcu->Y = mcu->Y;
//    afficher_mcu2(*echan_mcu);
    echan_mcu->Cb = malloc(sizeof(int16_t));
    echan_mcu->Cr = malloc(sizeof(int16_t));
    echan_mcu->Cb[0] = malloc(64* sizeof(int16_t));
    echan_mcu->Cr[0] = malloc(64* sizeof(int16_t));
//    afficher_mcu2(*mcu);
    if (mcu->tailleC > 1) {
        echan_h(mcu->Cr[0],
                mcu->Cr[1],
                echan_mcu->Cr[0]);
        echan_h(mcu->Cb[0],
                mcu->Cb[1],
                echan_mcu->Cb[0]);
        afficher_mcu2(*echan_mcu);
        if (mcu->tailleC == 4) {
            echan_mcu->tailleC ++;
            echan_mcu->Cb[1] = malloc(64* sizeof(int16_t));
            echan_mcu->Cr[1] = malloc(64* sizeof(int16_t));

            echan_h(mcu->Cr[2],
                    mcu->Cr[3],
                    echan_mcu->Cr[1]);
            echan_h(mcu->Cb[2],
                    mcu->Cb[3],
                    echan_mcu->Cb[1]);

        }
    } else {
        printf("Taille du mcu incompatible");
    }
}

void mcu_echan_h_v(MCUTransform *mcu, MCUTransform *echan_mcu) {
    echan_mcu->tailleC = 1;
    echan_mcu->tailleY = mcu->tailleY;
    if (mcu->tailleC < 4) {
        printf("taille du mcu incompatible");
    } else {
        echan_mcu->Y = mcu->Y;
        echan_mcu->Cb = malloc(sizeof(int16_t));
        echan_mcu->Cr = malloc(sizeof(int16_t));
        echan_mcu->Cb[0] = malloc(64* sizeof(int16_t));
        echan_mcu->Cr[0] = malloc(64* sizeof(int16_t));
        echan_h_v(mcu->Cb[0], mcu->Cb[1], mcu->Cb[2], mcu->Cb[3], echan_mcu->Cb[0]);
        echan_h_v(mcu->Cr[0], mcu->Cr[1], mcu->Cr[2], mcu->Cr[3], echan_mcu->Cr[0]);

    }
}

void MATmcu_echan_h_v(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu) {
    for (int i = 0; i < matmcu->nblignes*matmcu->nbcol; ++i) {
        mcu_echan_h_v(&(matmcu->mcus[i]), &(echan_matmcu->mcus[i]));
    }


}

void MATmcu_echan_h(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu) {

    for (int i = 0; i < echan_matmcu->nbcol*echan_matmcu->nblignes; ++i) {
        mcu_echan_h(&(matmcu->mcus[i]), &(echan_matmcu->mcus[i]));

    }
}


void echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int h1, int l1, int h2, int l2) {
    printf("%d %d\n", matmcu->nbcol, matmcu->nblignes);
    MCUsTransformMat *fusionmatmcu = fusion_mcu(matmcu, h1, l1);
    printf("%d %d\n", fusionmatmcu->nbcol, fusionmatmcu->nblignes);
    if (l1 == l2 && h1 == h2) {
        //pas d'echantillonnage
        *echan_matmcu = *fusionmatmcu;
        printf("%d %d\n", echan_matmcu->nblignes, echan_matmcu->nbcol);

    } else {
        echan_matmcu->nblignes = fusionmatmcu->nblignes;
        echan_matmcu->nbcol = fusionmatmcu->nbcol;
        echan_matmcu->mcus = malloc(echan_matmcu->nbcol * echan_matmcu->nblignes * sizeof(MCUTransform));
        if (l1 == 2 && h1 == 2) {
            if (l2 == 2 && h2 == 1) {
                MATmcu_echan_h(fusionmatmcu, echan_matmcu);
            }
            if (l2 == 1 && h2 == 1) {
                MATmcu_echan_h_v(fusionmatmcu, echan_matmcu);
            }
        }
        if (h1 == 2 && l1 == 1 && l2 == 1 && h2 == 1) {
            MATmcu_echan_h(fusionmatmcu, echan_matmcu);
        }
    }
}

/**
 * Affiche tous les MCUs
 * @param mcusTransformMat
 */
void afficherAllMCUs2(MCUsTransformMat *mcusTransformMat){
    printf("%i, %i", mcusTransformMat->nblignes, mcusTransformMat->nbcol);
    for (int i = 0; i < mcusTransformMat->nblignes * mcusTransformMat->nbcol; ++i) {
        printf("MCU #%d\n",i);

        afficher_mcu2(mcusTransformMat->mcus[i]);
    }
}

/**
 * Affiche un MCU
 * @param mcu
 */
void afficher_mcu2(MCUTransform mcu) {
    for (int i = 0; i < mcu.tailleY; ++i) {
        printf("Y%i\n", i);
        for (int j = 0; j < 64; ++j) {
            if (j%8 == 0){
                printf("\n");
            }
            printf("%hx ", mcu.Y[i][j]);
        }
        printf("\n\n");
    }
    if (mcu.Cb != NULL) {
        for (int i = 0; i <mcu.tailleC; ++i) {
            printf("[Cb%i]\n", i);

            for (int j = 0; j < 64; ++j) {
                if (j % 8 == 0) {
                    printf("\n");
                }
                printf("%hx  ", mcu.Cb[i][j]);
            }
            printf("\n\n");
            printf("[Cr%i]\n", i);
            for (int j = 0; j < 64; ++j) {
                if (j % 8 == 0) {
                    printf("\n");
                }
                printf("%hx  ", mcu.Cr[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}