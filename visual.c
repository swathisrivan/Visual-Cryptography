#include <sys/types.h>
#include <openssl/md5.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "visual_util.h"

int main(int argc, char *argv[])
{
	if(argc < MIN_ARGS || argc > MAX_ARGS)
        {
                fprintf(stderr, "\r\nerror: malformed command :invalid no.of arguments\r\nusage: hw2 stream -p=pphrase -l=len\r\n       hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n       hw2 merge pbmfile1 pbmfile2\r\n       hw2 decrypt [pbmfile]\r\n");
                exit(-1);

        }
        if(strcmp(argv[1], "stream") && strcmp(argv[1], "encrypt") && strcmp(argv[1], "merge") && strcmp(argv[1], "decrypt"))
        {
		fprintf(stderr, "\r\nerror: malformed command :invalid no.of arguments\r\nusage: hw2 stream -p=pphrase -l=len\r\n       hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n       hw2 merge pbmfile1 pbmfile2\r\n       hw2 decrypt [pbmfile]\r\n");
                exit(-1);
        }
	if(!strcmp(argv[1], "stream"))
	{
		validateStreamInput(argc, argv);
	}
	else if(!strcmp(argv[1], "encrypt"))
	{
		validateEncryptInput(argc, argv);
	}
	else if(!strcmp(argv[1], "merge"))
	{
		validateMergeInput(argc, argv);
	}
	else if(!strcmp(argv[1], "decrypt"))
	{
		validateDecryptInput(argc, argv);
	}
	else
	{
		fprintf(stderr, "\r\nerror: malformed command :invalid no.of arguments\r\nusage: hw2 stream -p=pphrase -l=len\r\n       hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n       hw2 merge pbmfile1 pbmfile2\r\n       hw2 decrypt [pbmfile]\r\n");
                exit(-1);
	}
	return 0;
}
