#include <string.h>

static void key_swap(unsigned char *a, unsigned char *b) {
    unsigned char x = *a;
    *a = *b;
    *b = x;
}

static void genkey(unsigned char *secret, int secretlen, unsigned char *key) {
    for (int i = 0; i < 256; i++)
        key[i] = i;
    key[256] = key[257] = 0;
    int si = 0;
    int newi = 0;
    for (int i = 0; i < 256; i++) {
        newi = (secret[si] + key[i] + newi) % 256;
        key_swap(key + i, key + newi);
        si = (si + 1) % secretlen;
    }
}

static void encrypt_buf(unsigned char *buf, int len, unsigned char *key) {
    int pos1 = key[256], pos2 = key[257];
    for (int i = 0; i < len; i++) {
        pos1 = (pos1 + 1) % 256;
        pos2 = (pos2 + key[pos1]) % 256;
        key_swap(key + pos1, key + pos2);
        buf[i] ^= key[(key[pos1] + key[pos2]) % 256];
    }
    key[256] = pos1;
    key[257] = pos2;
}

void gspy_encrypt(unsigned char *val, int len, unsigned char *secret) {
    unsigned char key[260];
    int secretlen = strlen((char *)secret);
    genkey(secret, secretlen, key);
    encrypt_buf(val, len, key);
}

static char b64_char(int n) {
    return n < 26 ? n + 'A' :
        n < 52 ? n - 26 + 'a' :
        n < 62 ? n - 52 + '0' :
        n == 62 ? '+' : '/';
}

static void b64_part(unsigned char *val, char *out) {
    out[0] = b64_char(val[0] >> 2);
    out[1] = b64_char(((val[0] & 3) << 4) | (val[1] >> 4));
    out[2] = b64_char(((val[1] & 15) << 2) | (val[2] >> 6));
    out[3] = b64_char(val[2] & 63);
}

void gspy_encode(unsigned char *val, int len, unsigned char *result) {
    unsigned char part[3];
    char partout[4];
    int val_idx = 0;
    while (val_idx < len) {
        for (int j = 0; j < 3; j++) {
            part[j] = val_idx < len ? val[val_idx] : 0;
            val_idx++;
        }
        b64_part(part, partout);
        memcpy(result, partout, 4);
        result += 4;
    }
    *result = 0;
}
