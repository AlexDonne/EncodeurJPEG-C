
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t htables_nb_symb_per_lengths[2][3][16] = {
  {{0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}},
  {{0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125},
  {0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119},
  {0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119}}
};

uint8_t tab1[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

uint8_t tab2[162] = {
  1, 2, 3, 0, 4, 17, 5, 18, 33, 49, 65, 6, 19,
  81, 97, 7, 34, 113, 20, 50, 129, 145, 161, 8,
  35, 66, 177, 193, 21, 82, 209, 240, 36, 51, 98,
  114, 130, 9, 10, 22, 23, 24, 25, 26, 37, 38,
  39, 40, 41, 42, 52, 53, 54, 55, 56, 57, 58, 67,
  68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 86, 87,
  88, 89, 90, 99, 100, 101, 102, 103, 104, 105, 106,
  115, 116, 117, 118, 119, 120, 121, 122, 131, 132, 133,
  134, 135, 136, 137, 138, 146, 147, 148, 149, 150, 151,
  152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169,
  170, 178, 179, 180, 181, 182, 183, 184, 185, 186, 194,
  195, 196, 197, 198, 199, 200, 201, 202, 210, 211, 212,
  213, 214, 215, 216, 217, 218, 225, 226, 227, 228, 229,
  230, 231, 232, 233, 234, 241, 242, 243, 244, 245, 246,
  247, 248, 249, 250
};

uint8_t tab3[162] = {
0, 1, 2, 3, 17, 4, 5, 33, 49, 6, 18, 65, 81, 7, 97,
113, 19, 34, 50, 129, 8, 20, 66, 145, 161, 177, 193,
9, 35, 51, 82, 240, 21, 98, 114, 209, 10, 22, 36, 52,
225, 37, 241, 23, 24, 25, 26, 38, 39, 40, 41, 42, 53,
54, 55, 56, 57, 58, 67, 68, 69, 70, 71, 72, 73, 74,
83, 84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104,
105, 106, 115, 116, 117, 118, 119, 120, 121, 122, 130,
131, 132, 133, 134, 135, 136, 137, 138, 146, 147, 148,
149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166,
167,168, 169, 170, 178, 179, 180, 181, 182, 183, 184,
185, 186, 194, 195, 196, 197, 198, 199, 200, 201, 202,
210, 211, 212, 213, 214, 215, 216, 217, 218, 226, 227,
228, 229, 230, 231, 232, 233, 234, 242, 243, 244, 245,
246, 247, 248, 249, 250
};

uint8_t *htables_symbols[2][3] = {
  {tab1, tab1, tab1}, {tab2, tab3, tab3}
};

uint8_t htables_nb_symbols[2][3] = {
  {12,12,12},
  {162,162,162}
};
