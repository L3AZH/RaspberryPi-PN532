/**
  * This example shows connecting to the PN532 and writing an Mifare Classic
  * type RFID tag
  */
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "pn532.h"
#include "pn532_rpi.h"

void resetBuff(uint8_t* buff){
    for (uint8_t i=0; i < 255; i++) {
        buff[i] = 0x00;
    }
}

void readFullDataMifareClassic1K(
    PN532 pn532,
    uint8_t* uid,
    uint8_t uid_length,
    uint8_t* key_a)
{
    uint8_t buff[255];
    //init buff with 0x00 character
    resetBuff(buff);

    uint32_t pn532_error = PN532_ERROR_NONE;
    for (uint8_t i=0; i < sizeof(buff); i++){
        buff[i]=0x00;
    }
    // Read data of 4N block
    for (uint8_t i=1; i<16 ; i++){
        printf("block %d \n", i);
        pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_length,
                    i*4, MIFARE_CMD_AUTH_A, key_a);
        if (pn532_error) {
            printf("Error when authenticating: 0x%02x\r\n", pn532_error);
            return;
        }
        pn532_error = PN532_MifareClassicReadBlock(&pn532, buff, i*4);
        if (pn532_error) {
            printf("Error when reading: 0x%02x\r\n", pn532_error);
        }
        for (uint8_t j=0; j < MIFARE_BLOCK_LENGTH; j++) {
            printf("%02x ", buff[j]);
        }
        printf("\n");
        resetBuff(buff);
    }
}


int main(int argc, char** argv) {
    uint8_t buff[255];
    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    uint8_t key_a[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint32_t pn532_error = PN532_ERROR_NONE;
    int32_t uid_len = 0;
    printf("Hello!\r\n");
    PN532 pn532;
    //PN532_SPI_Init(&pn532);
    PN532_I2C_Init(&pn532);
    //PN532_UART_Init(&pn532);
    if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
        printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);
    } else {
        return -1;
    }
    PN532_SamConfiguration(&pn532);
    printf("Waiting for RFID/NFC card...\r\n");
    while (1)
    {
        // Check if a card is available to read
        uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
        if (uid_len == PN532_STATUS_ERROR) {
            printf(".");
            fflush(stdout);
        } else {
            printf("Found card with UID: ");
            for (uint8_t i = 0; i < uid_len; i++) {
                printf("%02x ", uid[i]);
            }
            printf("\r\n");
            break;
        }
    }

    /**
      * Warning: DO NOT write the blocks of 4N+3 (3, 7, 11, ..., 63)
      * or else you will change the password for blocks 4N ~ 4N+2.
      * Note:
      * 1.  The first 6 bytes (KEY A) of the 4N+3 blocks are always shown as 0x00,
      * since 'KEY A' is unreadable. In contrast, the last 6 bytes (KEY B) of the
      * 4N+3 blocks are readable.
      * 2.  Block 0 is unwritable.
      */
    // Write block #6
    uint8_t block_number = 6;
    //uint8_t DATA[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len,
            block_number, MIFARE_CMD_AUTH_A, key_a);
    if (pn532_error) {
        printf("Error: 0x%02x\r\n", pn532_error);
        return -1;
    }

    readFullDataMifareClassic1K(pn532, uid, uid_len, key_a);
}

