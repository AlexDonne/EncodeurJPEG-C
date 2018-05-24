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
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCb = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCr = 2;
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
                    //fusion des blocs 8x8 pour avoir un mcu de taille 1x2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleY = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCb = 2;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCr = 2;
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
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Y[1] = matmcu->mcus[(2 * i + 1) * fusionmatmcu->nbcol
                                                                                        + j].Y[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cb[0] = matmcu->mcus[2 * i * fusionmatmcu->nbcol +
                                                                                         j].Cb[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[0] = matmcu->mcus[2 * i * fusionmatmcu->nbcol +
                                                                                         j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[1] = matmcu->mcus[(2 * i + 1) * fusionmatmcu->nbcol
                                                                                         + j].Cr[0];
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].Cr[1] = matmcu->mcus[(2 * i + 1) * fusionmatmcu->nbcol
                                                                                         + j].Cb[0];

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
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCb = 4;
                    fusionmatmcu->mcus[i * fusionmatmcu->nbcol + j].tailleCr = 4;
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
                    if((2 * i) * matmcu->nbcol + 2 * j){

                    }
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

void echan_v(int16_t *mat1, int16_t *mat2, int16_t *echan_mat){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i<4) {
                echan_mat[i * 8 + j] = round((mat1[2*i * 8 + j] + mat1[2*(i+1) * 8 + j])) / 2;
            } else {
                echan_mat[i * 8 + j] = round((mat2[2*(i-4) * 8 + j] + mat2[(2*(i-4)+1) * 8 + j])) / 2;
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
    echan_mcu->tailleCb = h2*l2;
    echan_mcu->tailleCr = h3*l3;
    echan_mcu->tailleY = mcu->tailleY;
    echan_mcu->Y = mcu->Y;
    //Allocation des mcus
    echan_mcu->Cb = malloc(echan_mcu->tailleCb * sizeof(int16_t));
    echan_mcu->Cr = malloc(echan_mcu->tailleCr * sizeof(int16_t));
    for (int i = 0; i < echan_mcu->tailleCb; ++i) {
        echan_mcu->Cb[i] = malloc(64* sizeof(int16_t));

    }
    for (int j = 0; j < echan_mcu->tailleCr; ++j) {
        echan_mcu->Cr[j] = malloc(64* sizeof(int16_t));

    }
    if(h1 == h2 && l1==l2){
        echan_mcu->Cb = mcu->Cb;
    }
    if(h1==h3 && l1==l3){
        echan_mcu->Cr= mcu->Cr;
    }

    if(h1 == 2 && l1==1) {
        if (h2 == 1 && l2 == 1) {
            echan_h(mcu->Cb[0],
                    mcu->Cb[1],
                    echan_mcu->Cb[0]);
        }
        if(h3 == 1 && l3 == 1){
            echan_h(mcu->Cr[0],
                    mcu->Cr[1],
                    echan_mcu->Cr[0]);
        }
        else{
            printf("sample incompatible");
        }
    }
    if(h1== 1 && l1 == 2){
        printf("cc\n");
        if (h2 == 1 && l2 == 1) {
            echan_v(mcu->Cb[0],
                    mcu->Cb[1],
                    echan_mcu->Cb[0]);
        }
        if(h3 == 1 && l3 == 1) {
            echan_v(mcu->Cr[0],
                    mcu->Cr[1],
                    echan_mcu->Cr[0]);
        }else{
            printf("sample incompatible");
        }

    }
    if (h1==2 && l1==2){
        if (h2 == 1 && l2 == 2) {
            echan_h(mcu->Cb[0], mcu->Cb[1], echan_mcu->Cb[0]);
            echan_h(mcu->Cb[2], mcu->Cb[3], echan_mcu->Cb[1]);
        }
        if (h3 == 1 && l3 == 2) {
            echan_h(mcu->Cr[0], mcu->Cr[1], echan_mcu->Cr[0]);
            echan_h(mcu->Cr[2], mcu->Cr[3], echan_mcu->Cr[1]);
        }
        if (h3 == 1 && l3 == 1) {
            echan_h_v(mcu->Cr[0],
                      mcu->Cr[1], mcu->Cr[2], mcu->Cr[3],
                      echan_mcu->Cr[0]);
        }
        if (h2 == 1 && l2==1){
            echan_h_v(mcu->Cb[0],
                      mcu->Cb[1], mcu->Cb[2], mcu->Cb[3],
                      echan_mcu->Cb[0]);
            }
        else{
            printf("echantillonnage non gere par l'encodeur");
        }
    } else {
        printf("Taille du mcu incompatible");
    }
}



void mat_mcu_echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int h1, int l1, int h2, int l2, int h3, int l3) {
    afficherAllMCUs2(matmcu);
    for (int i = 0; i < matmcu->nblignes*matmcu->nbcol; ++i) {
        mcu_echantillonnage(&(matmcu->mcus[i]), &(echan_matmcu->mcus[i]), h1, l1, h2, l2, h3, l3);
    }


}


void echantillonnage(MCUsTransformMat *matmcu, MCUsTransformMat *echan_matmcu, int h1, int l1, int h2, int l2, int h3, int l3) {
    printf("%d %d\n", matmcu->nbcol, matmcu->nblignes);
    MCUsTransformMat *fusionmatmcu = fusion_mcu(matmcu, h1, l1);
    printf("%d %d\n", fusionmatmcu->nbcol, fusionmatmcu->nblignes);
    if (l1 == l2 && h1 == h2 && l1==l3 && h1==h3) {
        //pas d'echantillonnage
        *echan_matmcu = *fusionmatmcu;
        printf("%d %d\n", echan_matmcu->nblignes, echan_matmcu->nbcol);

    } else {
        echan_matmcu->nblignes = fusionmatmcu->nblignes;
        echan_matmcu->nbcol = fusionmatmcu->nbcol;
        echan_matmcu->mcus = malloc(echan_matmcu->nbcol * echan_matmcu->nblignes * sizeof(MCUTransform));
        mat_mcu_echantillonnage(fusionmatmcu, echan_matmcu, h1, l1, h2, l2, h3, l3);
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
        for (int i = 0; i <mcu.tailleCb; ++i) {
            printf("[Cb%i]\n", i);

            for (int j = 0; j < 64; ++j) {
                if (j % 8 == 0) {
                    printf("\n");
                }
                printf("%hx  ", mcu.Cb[i][j]);
            }
            printf("\n\n");
        }
        for (int i = 0; i <mcu.tailleCr; ++i) {
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