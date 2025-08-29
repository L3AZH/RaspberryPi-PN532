//
// Created by l3azh on 28/08/2025.
//

#include "brute_force_key.h"

#include <stdio.h>
#include <stdlib.h>

#include "pn532.h"


uint8_t** getKeys() {
    uint8_t** result =  malloc(KEY_LIST_SIZE * sizeof(uint8_t*));
    for (int i = 0 ; i <= KEY_LIST_SIZE; i++) {
        result[i] = malloc(KEY_SIZE * sizeof(uint8_t));
    }
    for (int i = 0 ; i <= KEY_LIST_SIZE; i++) {
        result[i][0] = (uint8_t) (i % 10);
        result[i][1] = (uint8_t) (i / 10 % 10);
        result[i][2] = (uint8_t) (i / 100 % 10);
        result[i][3] = (uint8_t) (i / 1000 % 10);
        result[i][4] = (uint8_t) (i / 10000 % 10);
        result[i][5] = (uint8_t) (i / 100000 % 10);
    }

    return result;
}

void freeKeys(uint8_t** keys) {
    for (int i = 0 ; i <= KEY_LIST_SIZE; i++) {
        free(keys[i]);
    }
    free(keys);
}

void printKeys(uint8_t** keys) {
    for (int i = 0 ; i <= KEY_LIST_SIZE; i++) {
        for (int j = 0 ; j < KEY_SIZE; j++) {
            printf("%02X ", keys[i][j]);
        }
        printf("\n");
    }
}