//
// Created by Andrew Briasco-Stewart on 3/9/22.
//

//also known as addRoundKey
#include "structs.h"

void addRoundKey(BYTE *state, BYTE *roundKey) {
  //bitwise xor each state with roundkey
round_key_loop:
    for (int i = 0; i < 16; ++i) {
#pragma HLS UNROLL
        state[i] = state[i] xor roundKey[i];
    }
}
