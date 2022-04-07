//
// Created by Andrew Briasco-Stewart on 3/9/22.
//
#include "structs.h"
/**
 * shift the rows of the 4x4 column major order 128-bit key
 * @param state
 */
void shiftrows1(BYTE *state){
    BYTE tmp;
    //row 1
    state[0] = state[0];
    state[4] = state[4];
    state[8] = state[8];
    state[12] = state[12];

    //row 2, shift left by 1
    tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;

    //row 3, shift left by 2
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    //row 4 shift left by 3
    tmp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = tmp;

}
/**
 * Shift the rows of the 4x4 column major order state, and put into out
 * @param state the BYTE[16] 128 bit key source
 * @param out the destination where the shifted rows are copied to
 */
void shiftrows2(BYTE state[16], BYTE *out) {
    BYTE tmp;
    //row 1
    out[0] = state[0];
    out[4] = state[4];
    out[8] = state[8];
    out[12] = state[12];

    //row 2, shift left by 1
    tmp = state[1];
    out[1] = state[5];
    out[5] = state[9];
    out[9] = state[13];
    out[13] = tmp;

    //row 3, shift left by 2
    tmp = state[2];
    out[2] = state[10];
    out[10] = tmp;
    tmp = state[6];
    out[6] = state[14];
    out[14] = tmp;

    //row 4 shift left by 3
    tmp = state[15];
    out[15] = state[11];
    out[11] = state[7];
    out[7] = state[3];
    out[3] = tmp;
}