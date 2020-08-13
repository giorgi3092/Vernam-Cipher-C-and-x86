/*
    San Diego State University
    Giorgi Aptsiauri / 820968337
    Final Project for COMPE 271, Spring, 2018.

    Unbreakable Vernam Cipher using Galois LFSR.
*/

# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <stdlib.h>

int sprintfkeys(char* keyPtr, unsigned int LFSR_STATE);
extern int GaloisLFSRinC(unsigned int seedIN, unsigned int charCount, char key[]);
extern int galoislfsrina(unsigned int seedIN, unsigned int charCount, char key[]);
int Vernam(char key[], char message[], char cipher[]);
int revereTimeValue (unsigned int value);

int main(void) {
    unsigned int currentTime = 0;
    unsigned int stringLen = 0;
    char key[101];
    char cipher[101];
    char message[101];

    while(1)
    {
        currentTime = (unsigned int)time(NULL);
        /* currentTime value has to be reversed to accommodate "a more pseudorandom" sequence from LFSR.
           The reason is that time(NULL) returns time in seconds, where the most frequently updated bit is
           the least significant bit(LSB). Since the taps into the LFSR are most significant bits (MSB), LFSR would not
           consider currentTime's LSB into key generation and the same sequence of pseudorandom values would pop up
           for a long time which is not the objective of my program. By reversing, this problem is fairly solved for my
           application.*/
        currentTime = revereTimeValue(currentTime);

        do{
            printf("Enter a message to encrypt: ");
            fgets(message, 100, stdin);
            stringLen = strlen(message);
            message[stringLen-1] = '\0';
        } while (stringLen > 100);

        printf("Generating the key and cipher...\n");
        if(GaloisLFSRinC(currentTime, stringLen, &key[0]) != 1)
            exit(0);
        printf("%s\n", key);

        if(Vernam(&key[0], &message[0], &cipher[0]) != 1)
            exit(0);
        printf("\n");
    }

    return 0;
}

int Vernam(char key[], char message[], char cipher[])
{
    char bufferMessageByte[2]; // one byte for symbol, one for \0
    char bufferKeyByte[3]; // two bytes for symbol, one for \0
    char keyGenerated[strlen(message)+1];
    char cipherGenerated[strlen(message)+1];
    char reconstructedMessage[strlen(message)+1];
    char* messagePtr = &message[0];
    char* keyPtr = &key[0];
    int stingLen = strlen(messagePtr);
    unsigned char keySymbolAscii;
    unsigned char messageSymbolAscii;
    unsigned char cipherByte;
    int i;
    for(i = 0; i < stingLen; i++)
    {
        bufferMessageByte[0] = *messagePtr++;
        bufferMessageByte[1] = '\0';

        bufferKeyByte[0] = *keyPtr++;
        bufferKeyByte[1] = *keyPtr++;
        bufferKeyByte[2] = '\0';

        keySymbolAscii = (unsigned char)strtol(bufferKeyByte, NULL, 16);
        /* normalize key symbol to ASCII displayable symbols, i.e. decimal 33 - 126.
           the next line can be commented to allow all values, decimal 0 - 255, to be used during encryption as a key.
           Even though each byte of key is normalized, final cipher text will not be normalized.*/

        keySymbolAscii = 33 + (keySymbolAscii%93);
        keyGenerated[i] = (unsigned char)keySymbolAscii; // save to final key array
        messageSymbolAscii = (unsigned char)bufferMessageByte[0];

        // XORING
        cipherByte = keySymbolAscii ^ messageSymbolAscii;
        cipherGenerated[i] = (unsigned char)cipherByte; // save cipher text
    }

    keyGenerated[i] = '\0';
    cipherGenerated[i] = '\0';

    printf("key: %s\n", keyGenerated);
    printf("cipher text: %s\n", cipherGenerated);

    // Decryption
    printf("Now let's decrypt the cipher text...\n");
    messagePtr = &message[0];
    keyPtr = &keyGenerated[0];
    for(i = 0; i < stingLen; i++)
    {
        reconstructedMessage[i] = ((unsigned char)keyPtr[i] ^ (unsigned char)cipherGenerated[i]);
    }
    reconstructedMessage[i] = '\0';
    printf("Decrypted message: %s\n", reconstructedMessage);

    return 1;
}

int revereTimeValue (unsigned int value)
{
    int reversedValue = 0, remainder;
    while(value != 0)
    {
        remainder = value%10;
        reversedValue = reversedValue*10 + remainder;
        value /= 10;
    }
    return reversedValue;
}

int sprintfkeys(char* keyPtr, unsigned int LFSR_STATE)
{
    return sprintf(keyPtr , "%x", LFSR_STATE);
}
