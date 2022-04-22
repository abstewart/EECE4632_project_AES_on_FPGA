#include <iostream>
using namespace std;
#include "structs.h"
#include "methods.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_axis<128,1,1,1> AXI_VAL;

void printState(BYTE* state){
    auto flags = cout.flags();
    cout << hex << +state[0] << +state[4] << +state[8] << +state[12] << '\n';
    cout << +state[1] << +state[5] << +state[9] << +state[13] << '\n';
    cout << +state[2] << +state[6] << +state[10] << +state[14] << '\n';
    cout << +state[3] << +state[7] << +state[11] << +state[15] << '\n';
    cout.flags(flags);
}

int main() {
    BYTE t1 = 0x00;

    unsigned long int all1 = 0xFFFFFFFFFFFFFFFF;
    BYTE all1byte[16];
    for (BYTE &i : all1byte) {
        i = 0xFF;
    }
    BYTE everyOtherByte[16];
    for (BYTE &i : everyOtherByte) {
        i = 0x33;
    }

    BYTE testBlankState[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
    BYTE forwardSboxState[16] = {};
    BYTE inverseSboxState[16] = {};

    BYTE incrementalState[16] = {0x00,0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08,0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    BYTE postShiftRows[16] = {};

    BYTE mixColTest2[4] = {0xdb, 0x13, 0x53, 0x45};
    BYTE mixColTest3[4] = {0xf2,  0x0a, 0x22, 0x5c};
    BYTE mixColTest[4] = {0x2d,  0x26, 0x31, 0x4c};

    //call the functions
    forwardSubst2(testBlankState, forwardSboxState);
    inverseSubst2(forwardSboxState, inverseSboxState);

    auto flags = cout.flags();
    cout << "testing single forward subst" << '\n';
    if(lookupForwardSbox(t1) == 0x63)
        cout << "single forward subst passes" << '\n';
    if(lookupInverseSbox(t1) == 0x52)
        cout << "single inverse subst passes" << '\n';
    cout << "testing complete sbox subst" << '\n';

    //test full substitution
    cout << "testing full subst" << '\n';
    printState(testBlankState);
    printState(forwardSboxState);
    printState(inverseSboxState);

    //test shiftrows
    cout << "testing shift rows" << '\n' << "before shift \n";
    printState(incrementalState);
    shiftrows2(incrementalState, postShiftRows);
    cout << "after shift: \n";
    printState(postShiftRows);
    cout << "after a second shift \n";
    shiftrows1(postShiftRows);
    printState(postShiftRows);

    cout << "testing var sizes (should be in bytes)" << '\n';
    cout << "unsigned char: " << sizeof(unsigned char) << '\n';
    cout << "unsigned int: " << sizeof(unsigned int) << '\n';
    cout << "unsigned long: " << sizeof(unsigned long) << '\n';
    cout << "unsigned long long: " << sizeof(unsigned long long) << '\n';

    cout << "testing addRoundKey\n";
    cout << "state before addRoundKey\n";
    printState(all1byte);
    cout << "state after adding 0xF0 byte key\n";
    addRoundKey(all1byte, everyOtherByte);
    printState(all1byte);

    cout << "testing the mixColumns function\n";
    cout << hex << +mixColTest[0] << +mixColTest[1] << +mixColTest[2] << +mixColTest[3] << '\n';
    mixColViaMath(mixColTest);
    cout << hex << +mixColTest[0] << +mixColTest[1] << +mixColTest[2] << +mixColTest[3] << '\n';

    cout << "testing conversion of 32-bit word to char\n";
    uint32_t w = 0x87654321;
    BYTE ww = (BYTE)(w);
    BYTE www = (BYTE)(w >> 24);
    cout << hex << +w << '\n';
    cout << hex << +ww << '\n';
    cout << hex << +www << '\n';

    cout.flags(flags);

    //key expansion testing
    BYTE testKey[16] = {0x54, 0x68, 0x61, 0x74,
                        0x73, 0x20, 0x6D, 0x79,
                        0x20, 0x4B, 0x75, 0x6E,
                        0x67, 0x20, 0x46, 0x75};
    BYTE roundKeys[176];
    keyExpand(testKey, roundKeys);
    printState(&roundKeys[0]);
    cout << "final round key\n";
    printState(&roundKeys[160]);
    /*
    for (int i = 1; i < 11; ++i) {
        //160, 161, 62, 63, 64, 65, 66, 67
        //68, 69, 70, 71, 72, 73, 74, 75
        printState(&roundKeys[i*16]);

    }
    */

    //example testing with https://www.kavaliro.com/wp-content/uploads/2014/03/AES.pdf
    BYTE ptext[16] = {0x54, 0x77, 0x6F, 0x20, 0x4F, 0x6E, 0x65,
                      0x20, 0x4E, 0x69, 0x6E, 0x65, 0x20, 0x54, 0x77, 0x6F};
    BYTE ctext[16];
    aes_encrypt(ptext, testKey, ctext);


    //final testing
    BYTE key1[16] = {0x10, 0xa5, 0x88, 0x69, 0xd7,
                     0x4b, 0xe5, 0xa3, 0x74, 0xcf,
                     0x86, 0x7c, 0xfb, 0x47, 0x38, 0x59};
    BYTE cipher[16];
    fill( cipher, cipher + sizeof( cipher ), 0 );
    BYTE encrypted[16];
    cout << "ciper before encrypt\n";
    printState(cipher);
    aes_encrypt(cipher, key1, encrypted);
    cout << "testing key\n";
    printState(key1);
    cout << "cipher\n";
    printState(cipher);
    cout << "ciphertext\n";
    printState(encrypted);

    cout<< "ABOVE TESTING FOR FUNCTIONALITY, BELOW FOR INTERFACE TESTING\n";
    ap_uint<128> inpu = (ap_uint<128>)0x54776F204F6E6520 << 64;
    inpu ^= (ap_uint<128>)0x4E696E652054776F;
    long long int a = (long long int) inpu;
    long long int b = (long long int) (inpu >> 64);
    BYTE ans[16];
    bigIntToArr(inpu, ans);
    cout << hex << +b << '\n';
    cout << hex << +a << '\n';
    cout << "into arr:\n";
    printState(ans);

    cout << "going the other way\n";
    ap_uint<128> rev = 0;
    rev = arrToBigInt(ans);
    long long int aa = (long long int) rev;
    long long int bb = (long long int) (rev >> 64);
    cout << hex << +bb << '\n';
    cout << hex << +aa << '\n';



}
