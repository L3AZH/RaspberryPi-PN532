//
// Created by l3azh on 28/08/2025.
//

#ifndef PN532_NFC_HAT_CODE_BRUTE_FORCE_KEY_H
#define PN532_NFC_HAT_CODE_BRUTE_FORCE_KEY_H
#include <stdint.h>
#endif //PN532_NFC_HAT_CODE_BRUTE_FORCE_KEY_H


#define KEY_SIZE 6
#define KEY_LIST_SIZE 1000000

uint8_t** getKeys();
void freeKeys(uint8_t** keys);
void printKeys(uint8_t** keys);

