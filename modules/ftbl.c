#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soundpipe.h" 

#ifndef M_PI
#define M_PI		3.14159265358979323846	/* pi */
#endif 

int sp_ftbl_create(sp_data *sp, sp_ftbl **ft, size_t size) 
{
    *ft = malloc(sizeof(sp_ftbl));
    sp_ftbl *ftp = *ft;
    ftp->size = size;
    ftp->tbl = malloc(sizeof(SPFLOAT) * (size + 1));
    memset(ftp->tbl, 0, sizeof(SPFLOAT) * (size + 1));
    ftp->sicvt = 1.0 * SP_FT_MAXLEN / sp->sr;
    ftp->lobits = log2(SP_FT_MAXLEN / size);
    ftp->lomask = pow(2, ftp->lobits) - 1;
    ftp->lodiv = 1.0 / pow(2, ftp->lobits);
    return SP_OK;
}

int sp_ftbl_destroy(sp_ftbl **ft)
{
    sp_ftbl *ftp = *ft;
    free(ftp->tbl);
    free(*ft);
    return SP_OK;
}

int sp_gen_vals(sp_ftbl *ft, char *string)
{
    char *str1, *token, *t;
    char *saveptr1;
    int j;
    char *d;
    d = malloc(sizeof(char) + 1);
    d[0] = ' ';
    d[1] = 0;
    t = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(t, string);
    for (j = 0, str1 = t; ; j++, str1 = NULL) {
        token = strtok_r(str1, d, &saveptr1);
        if (token == NULL)
            break;
        if(ft->size < j + 1){
            ft->tbl = realloc(ft->tbl, sizeof(SPFLOAT) * (ft->size + 2));
            ft->size++;
        }
        ft->tbl[j] = atof(token);
    }

    free(t);
    free(d);
    return SP_OK;
}

int sp_gen_sine(sp_ftbl *ft)
{
    unsigned long i;
    SPFLOAT step = 2 * M_PI / ft->size;
    for(i = 0; i < ft->size; i++){
        ft->tbl[i] = sin(i * step);
    }
    return SP_OK;
}

/*TODO: add error checking, make tests */
int sp_gen_file(sp_ftbl *ft, const char *filename)
{
    SF_INFO info;
    SNDFILE *snd = sf_open(filename, SFM_READ, &info);
    sf_readf_float(snd, ft->tbl, ft->size);
    sf_close(snd);
    return SP_OK;
}

/* port of GEN10 from Csound */
int sp_gen_sinesum(sp_data *sp, sp_ftbl *ft, char *argstring)
{
    sp_ftbl *args;
    sp_ftbl_create(sp, &args, 1);
    sp_gen_vals(args, argstring);

    int32_t phs;
    SPFLOAT amp;
    int32_t flen = ft->size;
    SPFLOAT tpdlen = 2.0 * M_PI / (SPFLOAT) flen;

    int i, n;

    for(i = args->size; i > 0; i--){
        amp = args->tbl[args->size - i];
        if(amp != 0) {
            for(phs = 0, n = 0; n < ft->size; n++){
                ft->tbl[n] += sin(phs * tpdlen) * amp;
                phs += i;
                phs %= flen;
            }
        }
    }
    sp_ftbl_destroy(&args);
    return SP_OK;
}

/* port of GEN07 from Csound */
int sp_gen_line(sp_data *sp, sp_ftbl *ft, char *argstring) 
{
    int i, n = 0, seglen, nsegs, argpos = 1;
    SPFLOAT valp;
    SPFLOAT amp1, incr;
    sp_ftbl *args;   
    sp_ftbl_create(sp, &args, 1);
    sp_gen_vals(args, argstring);
    valp = args->tbl[0];
    nsegs = args->size >> 1; 
    
    if((args->size % 2) == 0 || args->size == 1) {
        fprintf(stderr, "Error: not enough arguments for gen_line.\n");
        sp_ftbl_destroy(&args);
        return SP_NOT_OK;
    } 
       
    for(i = 0; i < nsegs; i ++) {
        seglen = (int)args->tbl[argpos++];
        amp1 = args->tbl[argpos++];
        incr = (valp - amp1) / seglen;
        while(seglen--) {
            if(n < ft->size) {
                ft->tbl[n] = amp1;
                amp1 += incr;
                n++;
            } else {
                break;
            }
        }
        valp = amp1;
    }
    
    sp_ftbl_destroy(&args);
    return SP_OK;
}
 