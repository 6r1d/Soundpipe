#include <stdlib.h>
#include "soundpipe.h"

int sp_metro_create(sp_metro **p){
    *p = malloc(sizeof(sp_metro));
    return SP_OK;
}
int sp_metro_destroy(sp_metro **p){
    free(*p);
    return SP_OK;
}
int sp_metro_init(sp_data *sp, sp_metro *p){
    p->iphs = 0;
    p->cps = 2;
    SPFLOAT phs = p->iphs;
    int32_t  longphs = phs;
    if (phs >= 0.0){
      p->curphs = (SPFLOAT)phs - (SPFLOAT)longphs;
    }
    p->flag=1;
    p->onedsr = 1.0 / sp->sr;
    return SP_OK;
}
int sp_metro_compute(sp_data *sp, sp_metro *p, SPFLOAT *in, SPFLOAT *out){
    SPFLOAT phs= p->curphs;
    if (phs == 0.0 && p->flag){
        *out = 1.0;
        p->flag = 0;
    }else if ((phs += p->cps * p->onedsr) >= 1.0){
        *out = 1.0;
        phs -= 1.0;
        p->flag = 0;
    }else{
        *out = 0.0;
    }
    p->curphs = phs;
    return SP_OK;
}
