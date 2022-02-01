/*
   ROT13 encoding/decoding program
   respects capital and small letters while leaving other symbols as they are
*/

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define STR_SIZE 500

char *generate_digest(char *input)
{
    static char digest[STR_SIZE];
    unsigned char ascii_char = 0;

    for (size_t i = 0; i < strlen(input) ; i++) {
        // 65 -> 90  = A -> Z
        // 97 -> 122 = a -> z
        if (input[i] >= 65 && input[i] <= 90) {
            ascii_char = input[i] + 13;
            // handle overflow when reaching the end of the alphabet
            if (ascii_char > 90) {
                ascii_char = ascii_char - 90 + 64;
            }
            digest[i] = ascii_char;
        }
        else if (input[i] >= 97 && input[i] <= 122) {
            ascii_char = input[i] + 13;
            // handle overflow when reaching the end of the alphabet
            if (ascii_char > 122) {
                ascii_char = ascii_char - 122 + 96;
            }
            digest[i] = ascii_char;
        }
        else {
            digest[i] = input[i];
        }
    }
    return digest;
}

int main(void)
{
    char input[STR_SIZE];

    printf("Please enter a text to encrypt/decrypt:\n");
    fgets(input, STR_SIZE, stdin);

    printf("\nThe result is:\n%s", generate_digest(input));
    return 0;
}

