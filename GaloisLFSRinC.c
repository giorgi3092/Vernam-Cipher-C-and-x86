# include <stdint.h>
# include <stdio.h>

extern int GaloisLFSRinC(unsigned int seedIN, unsigned int charCount, char key[])
{
    unsigned int startingState = seedIN;  /* this is a starting state (seed) of the LFSR */
    unsigned int LFSR_STATE = seedIN;  /* LFSR_STATE will be updated per Galois LFSR technique */
    unsigned period = 0;  /* measure period */
    char* keyPtr = key;

    do {
        unsigned lsb = LFSR_STATE & 1;   /* generating least significant bit (LSB) (i.e. output). */
        LFSR_STATE >>= 1;                /* apply shift. */
        if (lsb)                         /* taps are applied iff LSB is 1. */
            LFSR_STATE ^= 0XA3000000u;   /* taps are: 32, 30, 26, 25. binary equivalent: 1010 0011 0000 0000 0000 0000 0000 0000*/
        ++period;
        keyPtr += sprintfKeys(keyPtr, LFSR_STATE);
        if(4*period >= charCount)
            break;
    } while (LFSR_STATE != startingState);
    return 1;
}
