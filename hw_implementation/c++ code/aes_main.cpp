//
// Created by Andrew Briasco-Stewart on 3/9/22.
//
#include "structs.h"
#include "methods.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axis<128,1,1,1> AXI_VAL;

/**
 * Encrypt the given 128-bit block with the given 128-bit key into the encrypted pointer
 * @param block
 * @param key
 * @param encrypted
 */
void aes_encrypt(BYTE *block, BYTE *key, BYTE *encrypted){

	BYTE iencrypted[16];
	for (int i = 0; i < 16; i++) {
		iencrypted[i] = block[i];
	}
	/*
	BYTE iblock[128];
	BYTE ikey[128];
	BYTE iencrypted[128];

	//copy inputs to local vars to define size
	for(int i = 0; i < 128; i++) {
		iblock[i] = block[i];
		ikey[i] = key[i];
		iencrypted[i] = encrypted[i];
	}
	*/

    BYTE roundKeys[176];

    //copy block to encrypted


    //STEP 1, key expansion
    keyExpand(key, roundKeys);

    //initial round key addition
    addRoundKey(iencrypted, &roundKeys[0]);

    //9 rounds of the following
    for (int i = 0; i < 9; ++i) {
        forwardSubst1(iencrypted);
        shiftrows1(iencrypted);
        mixColumnsInBlock(iencrypted);
        addRoundKey(iencrypted, &roundKeys[i*16 + 16]);
    }

    //final (10th) round
    forwardSubst1(iencrypted);
    shiftrows1(iencrypted);
    addRoundKey(iencrypted, &roundKeys[160]);

    //at this point encrypted should be finished
    //copy output to encrypted
    for(int i = 0; i<16; i++) {
    	encrypted[i] = iencrypted[i];
    }

}

void bigIntToArr(ap_uint<128> bigInt, BYTE *arr_out) {
	for (int i = 0; i < 16; i++) {
		arr_out[15 - i] = (BYTE)(bigInt >> (8*i));
	}
}

ap_uint<128> arrToBigInt(BYTE *arr) {
	ap_uint<128> ans = 0;
	for (int i = 0; i < 16; i++) {
		ans ^= (ap_uint<128>)arr[i] << (120 - i*8);
	}
	return ans;
}

void aes_encrypt_impl(AXI_VAL *block, AXI_VAL *encrypted, ap_uint<128> key_const) {
#pragma HLS INTERFACE axis port=encrypted
#pragma HLS INTERFACE axis port=block
#pragma HLS INTERFACE s_axilite port=key_const
#pragma HLS INTERFACE ap_ctrl_none port=return

	BYTE keyarr[16];
	BYTE blockarr[16];
	BYTE ansarr[16];

	bigIntToArr(block->data, blockarr);
	bigIntToArr(key_const, keyarr);
	aes_encrypt(blockarr, keyarr, ansarr);

	ap_uint<128> ansEncrypt = 0;

	ansEncrypt = arrToBigInt(ansarr);


	encrypted->data = ansEncrypt;
	encrypted->dest = block->dest;
	encrypted->id = block->id;
	encrypted->keep = block->keep;
	encrypted->last = block->last;
	encrypted->strb = block->strb;
	encrypted->user = block->user;
}
