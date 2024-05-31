#ifndef GAMESPYUTILS_H
#define GAMESPYUTILS_H

void gspy_encrypt(unsigned char *val, int len, unsigned char *secret);
void gspy_encode(unsigned char *val, int len, unsigned char *result);

#endif
