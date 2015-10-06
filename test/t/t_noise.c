#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
} UserData;

int t_noise(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;

    UserData ud;
    sp_noise_create(&ud.ns);
    sp_noise_init(sp, ud.ns);

    SPFLOAT in;

    for(n = 0; n < tst->size; n++) {
        sp_noise_compute(sp, ud.ns, &in, &sp->out[0]); 
        sp_test_add_sample(tst, sp->out[0]);
    }

    if(sp_test_compare(tst, hash) == SP_NOT_OK) {
        printf("Generated hash %s does not match reference hash %s\n", 
                tst->md5string, hash);
        fail = 1;
    }

    sp_noise_destroy(&ud.ns);
    
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
