#include "../util/ftok.h"

#include <stdio.h>
#include <stdlib.h>


const int KEY_BUFFER_SIZE     = 2048;
const int CIPHER_BUFFER_SIZE  = 2048;
const int DECRYPT_BUFFER_SIZE = 115200;


int read_key(FILE *in, char *key, int size) {
  int i      = 0;
  char *word = NULL;
  for (i = 0; (i < size) && ((word = ftok(in, " \n")) != NULL); i++) {
    key[i] = *word;
  }
  return i;
}

int read_cipher(FILE *in, int *cipher, int size) {
  int i      = 0;
  char *word = NULL;
  for (i = 0; (i < size) && ((word = ftok(in, " ,\n")) != NULL); i++) {
    cipher[i] = atoi(word);
  }
  return i;
}

int decrypt(char *key, int key_size, int *cipher, int cipher_size,
    char *decrypt, int decrypt_size) {
  int i;
  for (i = 0; i < cipher_size && i < decrypt_size; i++) {
    if (cipher[i - 1] >= key_size) {
      fprintf(stderr,
          "Error: word '%i' does not exist, the key is only '%i' words long.\n",
          i - 1, key_size);
      return 0;
    }
    decrypt[i] = key[cipher[i] - 1];
  }
  decrypt[i] = '\0';
  return i;
}


int main(int argc, char **argv) {
  char key[KEY_BUFFER_SIZE];
  int key_size = 0;

  int cipher[CIPHER_BUFFER_SIZE];
  int cipher_size = 0;

  char decrypted[DECRYPT_BUFFER_SIZE];

  if (argc != 3) {
    fprintf(stderr, "Usage: %s KEY CIPHERTEXT\n", argv[0]);
    fprintf(stderr,
        "KEY:        file containing text to be used as key for the Beale "
        "cipher\n");
    fprintf(stderr, "CIPHERTEXT: file containing the encrypted message\n");
    return 0;
  }

  FILE *fkey = NULL;
  if ((fkey = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error: can't open '%s'.\n", argv[1]);
    return 1;
  }
  key_size = read_key(fkey, key, KEY_BUFFER_SIZE);
  if (fclose(fkey)) {
    fprintf(stderr, "Error: can't close '%s'.\n", argv[1]);
    return 2;
  }

  FILE *fcipher = NULL;
  if ((fcipher = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "Error: can't open '%s'.\n", argv[2]);
    return 1;
  }
  cipher_size = read_cipher(fcipher, cipher, CIPHER_BUFFER_SIZE);
  if (fclose(fcipher)) {
    fprintf(stderr, "Error: can't close '%s'.\n", argv[2]);
    return 2;
  }

  if (decrypt(key, key_size, cipher, cipher_size, decrypted,
          DECRYPT_BUFFER_SIZE) == 0) {
    fprintf(stderr, "Error: can't decrypt cipher.\n");
    return 3;
  }

  puts(decrypted);
  return 0;
}
