#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    sp_jitter *jit;
} udata;

int t_jitter(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    udata ud;
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);
    sp_jitter_create(&ud.jit);   

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    sp_jitter_init(sp, ud.jit);
    ud.jit->cpsMin = 0.5;
    ud.jit->cpsMax = 4;
    ud.jit->amp = 3;  

    for(n = 0; n < tst->size; n++) {
        SPFLOAT jit = 0;
        sp_jitter_compute(sp, ud.jit, NULL, &jit);
        ud.osc->freq = sp_midi2cps(60 + jit);
        sp_fosc_compute(sp, ud.osc, NULL, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    if(sp_test_compare(tst, hash) == SP_NOT_OK) {
        printf("Generated hash %s does not match reference hash %s\n", 
                tst->md5string, hash);
        fail = 1;
    }
     
    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);
    sp_jitter_destroy(&ud.jit);
 
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
