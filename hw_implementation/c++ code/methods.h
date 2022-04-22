//
// Created by Andrew Briasco-Stewart on 3/12/22.
//

#ifndef AES_FPGA_METHODS_H
#define AES_FPGA_METHODS_H

#include "structs.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>

//methods in sbox class
BYTE lookupForwardSbox(BYTE);
BYTE lookupInverseSbox(BYTE);
void forwardSubst1(BYTE *state);
void forwardSubst2(BYTE[16], BYTE*);
void inverseSubst1(BYTE *state);
void inverseSubst2(BYTE[16], BYTE*);

//methods in shiftrows
//may want to make 2 input? for inp and out?
void shiftrows1(BYTE*);
void shiftrows2(BYTE[16], BYTE*);

//methods in the addRoundKey
void addRoundKey(BYTE*, BYTE*);

//methods in mixcolumn.cpp
void mixColViaMath(BYTE *r);
void mixColumnsInBlock(BYTE *state);

//aes_key_expand
void keyExpand(BYTE *key, BYTE *keys);

//aes_main
void aes_encrypt(BYTE *block, BYTE *key, BYTE *encrypted);

void bigIntToArr(ap_uint<128> bigInt, BYTE *arr_out);
ap_uint<128> arrToBigInt(BYTE *arr);

#endif //AES_FPGA_METHODS_H
