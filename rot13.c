/*
   ROT13 encoding and decoding program
   respects capital and small letters while leaving other symbols as they are
*/

#include <stdio.h>
#include <string.h>

#include "rot13.h"

#define SIZE 500    // max string size

int main(void)
{
    char msg[SIZE];

    printf("Please enter a text to encrypt:\n");
    fgets(msg, SIZE, stdin);

    char *digest = encrypt(msg);
    char *plain_text = decrypt(digest);

    printf("\nThe encrypted message is:\n%s\n", digest);
    printf("The decrypted message is:\n%s\n", plain_text);

    return 0;
}

char *encrypt(char *msg)
{
    static char digest[SIZE];
    int ascii_char = 0;

    for (int i = 0; i < strlen(msg) ; i++) {

        // 65 = A, 90 = Z
        if ((msg[i] >= 65) && (msg[i] <= 90)) {
            ascii_char = msg[i] + 13;

            if (ascii_char > 90) {
                ascii_char = ascii_char - 90 + 64;
            }
            digest[i] = ascii_char;
        }

        // 97 = a, 122 = z
        else if ((msg[i] >= 97) && (msg[i] <= 122)) {
            ascii_char = msg[i] + 13;

            if (ascii_char > 122) {
                ascii_char = ascii_char - 122 + 96;
            }
            digest[i] = ascii_char;
        }
        else {
            digest[i] = msg[i];
        }
    }
    return digest;
}

char *decrypt(char *cipher)
{
    static char plain_text[SIZE];
    int ascii_char = 0;

    for (int i = 0; i < strlen(cipher); i++) {

        // 65 = A, 90 = Z
        if ((cipher[i] >= 65) && (cipher[i] <= 90)) {
            ascii_char = cipher[i] - 13;

            if (ascii_char < 65) {
                ascii_char = ascii_char + 90 - 64;
            }
            plain_text[i] = ascii_char;
        }

        // 97 = a, 122 = z
        else if ((cipher[i] >= 97) && (cipher[i] <= 122)) {
            ascii_char = cipher[i] - 13;
            if (ascii_char < 97) {
                ascii_char = ascii_char + 122 - 96;
            }
            plain_text[i] = ascii_char;
        }
        else {
            plain_text[i] = cipher[i];
        }
    }
    return plain_text;
}
