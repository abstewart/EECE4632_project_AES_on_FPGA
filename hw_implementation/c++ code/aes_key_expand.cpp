//
// Created by Andrew Briasco-Stewart on 3/9/22.
//
//#include <cstdint>
#include "structs.h"
#include "methods.h"
//uint_32
#define WORD unsigned long
#define RKEY WORD[4]

static WORD ROUND_CONSTANT[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                          0x10000000, 0x20000000, 0x40000000, 0x80000000,
                                          0x1B000000, 0x36000000};

void deconWord(WORD w, BYTE *a, BYTE *b, BYTE *c, BYTE *d) {
    *a = (BYTE)(w >> 24);
    *b = (BYTE)(w >> 16);
    *c = (BYTE)(w >> 8);
    *d = (BYTE)(w);
}

void conWord(WORD *w, BYTE a, BYTE b, BYTE c, BYTE d) {
    *w = (((WORD)a) << 24) | (((WORD)b) << 16) | (((WORD)c) << 8) | ((WORD)d);
}

WORD rotateWord(WORD w) {
    BYTE wor[4];
    WORD ans;
    deconWord(w, &wor[0], &wor[1], &wor[2], &wor[3]);
    //old deconstruction of word
    //wor[0] = (BYTE)(w >> 24);
    //wor[1] = (BYTE)(w >> 16);
    //wor[2] = (BYTE)(w >> 8);
    //wor[3] = (BYTE)(w);

    conWord(&ans, wor[1], wor[2], wor[3], wor[0]);
    return ans;
    //old return
    //return ((WORD)wor[1] << 24) | ((WORD)wor[2]<<16) | ((WORD)wor[3]<<8) | ((WORD)wor[0]);
}

WORD subWord(WORD w) {
    BYTE wor[4];
    wor[0] = (BYTE)(w >> 24);
    wor[1] = (BYTE)(w >> 16);
    wor[2] = (BYTE)(w >> 8);
    wor[3] = (BYTE)(w);

    return (((WORD)lookupForwardSbox(wor[0])) << 24) | (((WORD)lookupForwardSbox(wor[1])) << 16) |
           (((WORD)lookupForwardSbox(wor[2])) << 8) | (((WORD)lookupForwardSbox(wor[3])));
}

//majority of this function derived from wikipedia
/**
 * Construct the round keys from the cipher key.
 * @param key the cipher key (not mutated)
 * @param keys one long BYTE array of the keys one after another (each key is 16 BYTE) total: BYTE[176]
 */
void keyExpand(BYTE *key, BYTE *keys) {

    WORD k[44];

    k[3] = (((int)key[12]) << 24) | (((int)key[13]) << 16) | (((int)key[14]) << 8) | key[15];
    k[2] = (((int)key[8]) << 24) | (((int)key[9]) << 16) | (((int)key[10]) << 8) | key[11];
    k[1] = (((int)key[4]) << 24) | (((int)key[5]) << 16) | (((int)key[6]) << 8) | key[7];
    k[0] = (((int)key[0]) << 24) | (((int)key[1]) << 16) | (((int)key[2]) << 8) | key[3];

    for (int i = 4; i < 45; ++i) {
        if(i % 4 == 0) {
            k[i] = k[i-4] ^ subWord(rotateWord(k[i-1])) ^ ROUND_CONSTANT[i/4 - 1];
        } else {
            k[i] = k[i-4] ^ k[i-1];
        }
    }

    //copy the words into the BYTE array
    //k[0] -> keys[0-3]
    //k[1] -> keys[4-7]
    for (int i = 0; i < 44; i++) {
        deconWord(k[i], &keys[i*4], &keys[i*4 + 1], &keys[i*4 + 2], &keys[i*4 + 3]);

    }
}

/*
 * N as the length of the key in 32-bit words (4)
 * k0,k1,k2,k3 as the 32-bit words of the original key (4 since 128-bit key)
 * R as the number of round keys needed (11)
 * W0, W1, W2, ... W43, W44 as the 32-bit words of the expanded key
 * RotWord shifts bytes by 1 to the left (circular)
 */



