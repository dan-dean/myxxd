#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
    if (argc > 2) {
        printf("Usage: %s [-b|-bits]\n", argv[0]);
        exit(BAD_NUMBER_ARGS);
    }

    if (argc == 2 &&
        (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
        *bits = TRUE;
    } else {
        *bits = FALSE;
    }

    return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
    int x = 0;
    for (size_t i = 0; i < size; i++) {
        if (x % 2 == 0 )
            printf(" ");
        printf("%02x", data[i]);
        //printf("TODO 2: printDataAsHex (2)");
        x++;
    }
    while ( x < 16 ) {
        if (x % 2 == 0)
            printf(" ");
        printf("  ");
        x++;
    }
}

void printDataAsBits(unsigned char *data, size_t size) {
    int x = 0;
    int myArray[8];
    for (size_t i = 0; i < size; i++) {
        int conversion = data[i];
        for (int j = 7; j >= 0; j--) {
            int bit;
            if (conversion % 2 == 1)
                bit = 1;
            else
                bit = 0;
            conversion = conversion / 2;
            myArray[j] = bit;
        }
        for (int j = 0; j < 8; j++) {
            printf("%d", myArray[j]);
        }
        x++;
        printf(" ");
    }
    while ( x < 6 ) {
        printf("         ");
        x++;
    }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if(data[i] >= 32 && data[i] <= 126)
            printf("%c", data[i]);
        else
            printf(".");
        //printf("TODO 2: printDataAsChars (3)");
    }
    printf("\n");
}

void readAndPrintInputAsHex(FILE *input) {
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsHex(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        numBytesRead = fread(data, 1, 16, input);
        /*if (numBytesRead !=0)
            printf("\n");
        else
            printf(".\n");
            */
    }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
    //printf("TODO 3: readAndPrintInputAsBits\n");
    unsigned char data[6];
    int numBytesRead = fread(data, 1, 6, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x: ", offset);
        offset += numBytesRead;
        printDataAsBits(data, numBytesRead);
        printf(" ");
        printDataAsChars(data, numBytesRead);
        numBytesRead = fread(data, 1, 6, input);
    }
}

int main(int argc, char **argv) {
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);

    if (bits == FALSE) {
        readAndPrintInputAsHex(input);
    } else {
        readAndPrintInputAsBits(input);
    }
    return 0;
}
