#include <sys/types.h>
#include <openssl/md5.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define WH_PX_BL_PX 0x01
#define BL_PX_WH_PX 0x02
#define TEMP_SIZE   100
#define BYTE_SHIFT_MAX 7
#define MIN_ARGS 2
#define MAX_ARGS 5

extern void md5_stream(char *p, int byt_len, char *mkey_str);
extern void img_encrypt(char *p, char *file_src, char *file_new);
extern void validateEncryptInput(int argc, char *argv[]);
extern void validateStreamInput(int argc, char *argv[]);
extern void validateMergeInput(int argc, char *argv[]);
extern void img_decrypt(char *file_src);
extern void validateDecryptInput(int argc, char *argv[]);
