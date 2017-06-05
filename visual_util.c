#include "visual_util.h"

void md5_stream(char *p, int byt_len, char *mkey_str)
{
    
    char md5_buf[MD5_DIGEST_LENGTH];
    int len          = strlen(p)+2+MD5_DIGEST_LENGTH;
    char *s          = malloc(len+1);
    int i            = 0;
    int j            = 0;
    int m            = 0;
    int byte_len     = byt_len;
    int total_len    = 0;
    int flag         = 0;

    MD5((const unsigned char *)p, strlen(p), (unsigned char *)md5_buf);
    memset(mkey_str, 0, byt_len);

    while(!flag) 
    {
	j = 0;
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((const unsigned char *)s, len, (unsigned char *)md5_buf);
        while(j < (sizeof(md5_buf)>>1))
	{
		
		mkey_str[m++] = md5_buf[j++] & 0xff;
		total_len++;
                if(total_len == byte_len)
		{
			flag = 1;
			break;
		}
	}
	i++;
	if(i == 100)
	{
		i = 0;
	}
    }
    free(s);
}

void img_encrypt(char *p, char *file_src, char *file_new)
{
	FILE *fp        = NULL;
	FILE *fp1       = NULL;
	FILE *fp2       = NULL;
	char *bef       = NULL;
	char *aft       = NULL;
	char *p1        = NULL;
        char *p2        = NULL;
	char *p3        = NULL;
	char *p4        = NULL;
	char *s         = NULL;
	int i           = 0;
        int j           = 0;
        int k           = 0;
        int l           = 0;
        int m           = 0;
	int n           = 0;
	float height    = 0;
	float width     = 0;
	int out_wb      = 0;
	int in_wb       = 0;
        int st          = 0;
	int st_i        = BYTE_SHIFT_MAX;
	float byte_len  = 0;

	char *mkey_str = NULL; 
	char buffer[TEMP_SIZE];
	
	memset(buffer, 0, TEMP_SIZE);
 
	if(file_src)
	{
		fp = fopen(file_src, "rb");
		if(!fp)
                {
                        fprintf(stderr, "\r\nerror: %s %s\n", file_src, strerror(errno));
                        exit(-1);
                }
                if(errno)
                {
                        fprintf(stderr, "\r\nerror: %s %s\n", file_src, strerror(errno));
                        exit(-1);
                }

	}
	else
	{
		fp = stdin;
	}

	sprintf(buffer, "%s.1.pbm", file_new);
	fp1      = fopen(buffer, "wb+");
	memset(buffer, 0, TEMP_SIZE);
	sprintf(buffer, "%s.2.pbm", file_new);        
	fp2      = fopen(buffer, "wb+");
	memset(buffer, 0, TEMP_SIZE);

	fgets(buffer, sizeof(buffer), fp);
	fgets(buffer,  sizeof(buffer), fp);
	bef      = strtok_r(buffer, " ", &aft);
	width    = atoi(bef);
	height   = atoi(aft);
	in_wb    = ceil(width/8); 
	out_wb   = ceil((width * 2)/8);
	byte_len = (width * height);
	byte_len = ceil(byte_len/8);
	

	p1 = (char *)malloc(out_wb);
        memset(p1, 0, out_wb);
        p2 = (char *)malloc(out_wb);
        memset(p2, 0, out_wb);
	p3 = (char *)malloc(out_wb);
        memset(p3, 0, out_wb);
	p4 = (char *)malloc(out_wb);
        memset(p4, 0, out_wb);
	s = (char *)malloc(in_wb);
	memset(s, 0, in_wb);

	mkey_str = (char *)malloc(byte_len);
        memset(mkey_str, 0, byte_len);
        md5_stream(p, byte_len, mkey_str);

	fwrite("P4\n", 1, 3,fp1);
	fwrite("P4\n", 1, 3,fp2);

	sprintf(buffer, "%d %d\n", (int)(width * 2), (int)(height * 2)); 
	fwrite(buffer, 1, strlen(buffer),fp1);
	fwrite(buffer, 1, strlen(buffer),fp2);



	while(1) /*output while to limit reading from src file*/
	{
		i    = 0;
		m    = 0;
		j    = 0;
		l    = 0;
		
		memset(p1, 0, out_wb);
		memset(p2, 0, out_wb);
		memset(p3, 0, out_wb);
		memset(p4, 0, out_wb);
		memset(s,  0, in_wb);

		n = fread(s, 1, in_wb, fp);	
		if(n < in_wb) /*break out of loop after reading all bits*/
		{
			break;
		}
		while(i < width) /* reading only width no. of bits */
        	{
                	for(k = 0; k < 8 && i < width; k++) /* loop to jump to next bytes in p1,p2 */
                	{
                        	if(((s[j] & (0x80 >> k)) >> (7 - k)) == 1)
                        	{
					if(((mkey_str[st] >> st_i) & 0x01) == 1)
					{
						//printf("pixel is 1 and key is 1");
						p1[l] = (p1[l] | (WH_PX_BL_PX << (6 - m)));
                                                p2[l] = (p2[l] | (BL_PX_WH_PX << (6 - m)));
                                                p3[l] = (p3[l] | (BL_PX_WH_PX << (6 - m)));
                                                p4[l] = (p4[l] | (WH_PX_BL_PX << (6 - m)));
					}
					else
					{
						//printf("pixel is 1 and key is 0");
						p1[l] = (p1[l] | (BL_PX_WH_PX << (6 - m)));
                                                p2[l] = (p2[l] | (WH_PX_BL_PX << (6 - m)));
                                                p3[l] = (p3[l] | (WH_PX_BL_PX << (6 - m)));
                                                p4[l] = (p4[l] | (BL_PX_WH_PX << (6 - m)));

					}

                        	}
                        	else
                        	{
					if(((mkey_str[st] >> st_i) & 0x01) == 1)
                                        {
                                                //printf("pixel is 0 and key is 1");
						p1[l] = (p1[l] | (WH_PX_BL_PX << (6 - m)));
                                                p2[l] = (p2[l] | (BL_PX_WH_PX << (6 - m)));
                                                p3[l] = (p3[l] | (WH_PX_BL_PX << (6 - m)));
                                                p4[l] = (p4[l] | (BL_PX_WH_PX << (6 - m)));
                                        }
                                        else
                                        {
                                                //printf("pixel is 0 and key is 0");

						p1[l] = (p1[l] | (BL_PX_WH_PX << (6 - m)));
                                                p2[l] = (p2[l] | (WH_PX_BL_PX << (6 - m)));
                                                p3[l] = (p3[l] | (BL_PX_WH_PX << (6 - m)));
                                                p4[l] = (p4[l] | (WH_PX_BL_PX << (6 - m)));
                                        }

                        	}
				st_i--;
				if(st_i == -1)
				{
					st_i = BYTE_SHIFT_MAX;
					st++;
				}	
                        	m += 2;
                        	if(m == 8)
                        	{
                                	m = 0;
                                	l++;
                        	}
                        	i++;
                	}/* end of for loop for every byte of one row*/
                	j++;
        	}/* end of one row of input file */
		fwrite(p1, 1, out_wb,fp1);
		fwrite(p2, 1, out_wb,fp1);
		fwrite(p3, 1, out_wb,fp2);
		fwrite(p4, 1, out_wb,fp2);
		
	}/* iterating to the next row of the input file*/ 

	fclose(fp1);
	fclose(fp2);
	fclose(fp);
}

void validateEncryptInput(int argc, char *argv[])
{
	char *token    = NULL;
	char *save     = NULL;
	char *p        = NULL;
	char *file_new = NULL;
	char *file_src = NULL;
	if(argc < 4)
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n");
                exit(-1);
        }
	token = strtok_r(argv[2], "=", &save);
        if(!strlen(save))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n");
                exit(-1);
        }
        if(strcmp(token, "-p"))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n");
                exit(-1);
        }
	p     = save;
        token = NULL;
        save  = NULL;
        token = strtok_r(argv[3], "=", &save);
        if(!strlen(save))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n");
                exit(-1);
        }
        if(strcmp(token, "-out"))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 encrypt -p=pphrase -out=name [pbmfile]\r\n");
                exit(-1);
        }
	file_new = save;
	if(argc > 4)
	{
		file_src = argv[4];
	}
	img_encrypt(p, file_src, file_new); 
}

void validateStreamInput(int argc, char *argv[])
{
	char *token    = NULL;
	char *save     = NULL;
	char *p        = NULL;
	char *mkey_str = NULL;
	int byt_len    = 0;
	if(argc != 4)
	{
		fprintf(stderr, "\r\nerror: malformed command :invalid no.of arguments\r\nusage: hw2 stream -p=pphrase -l=len\r\n");
                exit(-1);
	}
	token = strtok_r(argv[2], "=", &save);
	if(!strlen(save))
	{
		fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 stream -p=pphrase -l=len\r\n");
                exit(-1);
	}
	if(strcmp(token, "-p"))
	{
		fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 stream -p=pphrase -l=len\r\n");
                exit(-1);
	}
	p     = save;
	token = NULL;
	save  = NULL;
	token = strtok_r(argv[3], "=", &save);
        if(!strlen(save))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 stream -p=pphrase -l=len\r\n");
                exit(-1);
        }
	if(strcmp(token, "-l"))
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 stream -p=pphrase -l=len\r\n");
                exit(-1);
        }
	byt_len  = atoi(save);
	mkey_str = (char *)malloc(byt_len);
	memset(mkey_str, 0, byt_len);
 
	md5_stream(p, byt_len, mkey_str);
	fwrite(mkey_str, 1, byt_len, stdout);
	
}

void validateMergeInput(int argc, char *argv[])
{
	if(argc < 4 || argc > 4)
	{
		fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 merge pbmfile1 pbmfile2\r\n");
                exit(-1);
        }

	FILE *fp1    = NULL;
	FILE *fp2    = NULL;
	FILE *op     = NULL;
	char *bef    = NULL;
        char *aft    = NULL;
	float width  = 0;  
	float height = 0;
	int in_wb    = 0; 
	int n        = 0;
	int m        = 0;
	int i        = 0;
	char buffer[TEMP_SIZE];
	char *buf1   = NULL;
	char *buf2   = NULL;

        memset(buffer, 0, TEMP_SIZE);

	fp1    = fopen(argv[2], "rb");
	if(!fp1)
        {
        	fprintf(stderr, "\r\nerror: %s %s\n", argv[2], strerror(errno));
                exit(-1);
       	}
        if(errno)
        {
        	fprintf(stderr, "\r\nerror: %s %s\n", argv[2], strerror(errno));
                exit(-1);
        }
	fp2    = fopen(argv[3], "rb");
	if(!fp2)
        {
                fprintf(stderr, "\r\nerror: %s %s\n", argv[3], strerror(errno));
                exit(-1);
        }
        if(errno)
        {
                fprintf(stderr, "\r\nerror: %s %s\n", argv[3], strerror(errno));
                exit(-1);
        }

	op = stdout;

	fgets(buffer, sizeof(buffer), fp2);
        fgets(buffer, sizeof(buffer), fp2);
	memset(buffer, 0, TEMP_SIZE);

        fgets(buffer, sizeof(buffer), fp1);
        fgets(buffer, sizeof(buffer), fp1);

	bef    = strtok_r(buffer, " ", &aft);
        width  = atoi(bef);
        height = atoi(aft);
        in_wb  = ceil(width/8);
	buf1   = (char *)malloc(in_wb);
	buf2   = (char *)malloc(in_wb);
	
	memset(buf1, 0, in_wb);
	memset(buf2, 0, in_wb);
	memset(buffer, 0, TEMP_SIZE);

	fwrite("P4\n", 1, 3, op);
	sprintf(buffer, "%d %d\n", (int)width, (int)height);
	fwrite(buffer, 1, strlen(buffer), op);
	
	while(1)
	{
		n = 0;
		m = 0;
		memset(buf1, 0, in_wb);
		memset(buf2, 0, in_wb);

	 	n = fread(buf1, 1, in_wb, fp1); 
		m = fread(buf2, 1, in_wb, fp2);
	
		if(n < in_wb || m < in_wb)
		{
			break;
		}
		for(i = 0; i < in_wb; i++)
		{
			buf1[i] = ((buf1[i] & 0xff) | (buf2[i] & 0xff));
		}
		fwrite(buf1, 1, in_wb, op);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(op);	
}
void img_decrypt(char *file_src)
{
	FILE *fp        = NULL;
	FILE *op        = stdout;
	char *bef       = NULL;
        char *aft       = NULL;
	char *p         = NULL;
	char *s         = NULL;
	float height    = 0;
        float width     = 0;
        int out_wb      = 0;
        int in_wb       = 0;
	int n           = 0;
	int i           = 0;
	int j           = 0;
	int k           = 0;
	int l           = 0;
	int m           = 0;
	int z           = BYTE_SHIFT_MAX;
	
	char buffer[TEMP_SIZE];
	char buf[TEMP_SIZE];

        memset(buffer, 0, TEMP_SIZE);
	memset(buf, 0, TEMP_SIZE);

	if(file_src)
        {
                fp = fopen(file_src, "rb");
		if(!fp)
	        {
        	        fprintf(stderr, "\r\nerror: %s %s\n", file_src, strerror(errno));
                	exit(-1);
        	}
        	if(errno)
        	{
                	fprintf(stderr, "\r\nerror: %s %s\n", file_src, strerror(errno));
                	exit(-1);
        	}

        }
        else
        {
                fp = stdin;
        }
	fgets(buffer, sizeof(buffer), fp);
        fgets(buffer, sizeof(buffer), fp);
        bef      = strtok_r(buffer, " ", &aft);
        width    = atoi(bef);
        height   = atoi(aft);
	in_wb    = ceil(width/8); /* example 34 bits - 5 bytes */
        out_wb   = ceil((width/2)/8); /*example 17 bits - 3 bytes */
		
	p = (char *)malloc(out_wb);
        memset(p, 0, out_wb);
	s = (char *)malloc(in_wb);
        memset(s, 0, in_wb);

	fwrite("P4\n", 1, 3, op);
	sprintf(buf, "%d %d\n", ((int)width)/2, ((int)height)/2);
	fwrite(buf, 1, strlen(buf), op);

	while(1) /*output while to limit reading from src file*/
        {
		i = 0;
		j = 0;
		l = 0;
		z = BYTE_SHIFT_MAX;
		memset(p, 0, out_wb);
		memset(s, 0, in_wb);
		n = fread(s, 1, in_wb, fp);
                if(n < in_wb) /*break out of loop after reading all data from src file*/
                {
                        break;
                }
		while(i < width) /* reading only width no. of bits */
                {
			m = 6;
			for(k = 0; k < 4 && i < width; k++) /* loop each byte in s until i < width */
                        {
				if((((s[j] >> m) & 0x03) == WH_PX_BL_PX) || (((s[j] >> m) & 0x03) == BL_PX_WH_PX))
				{
			 		p[l] = (p[l] | (0x00 << z));
				}
				else
				{
					p[l] = (p[l] | (0x01 << z));
				}
				z--;
				if(z == -1)
				{
					z = BYTE_SHIFT_MAX;
					l++;
				}
				m -= 2;
				i += 2;
			}
			j++;
		} /*finish reading one bit */
	        fread(s, 1, in_wb, fp);
		fwrite(p, 1, out_wb, op);
	} /* end of reading one line from src file*/
	fclose(fp);
	fclose(op);
}

void validateDecryptInput(int argc, char *argv[])
{
        char *file_src = NULL;
        if(argc < 2)
        {
                fprintf(stderr, "\r\nerror: malformed command\r\nusage: hw2 decrypt [pbmfile]\r\n");
                exit(-1);
        }
        if(argc == 3)
        {
                file_src = argv[2];
        }
        img_decrypt(file_src);
}

