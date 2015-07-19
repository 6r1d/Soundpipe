#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_randi *rnd;
} udata;

void write_osc(sp_data *data, void *ud) {
    udata *udp = ud;
    SPFLOAT freq;
    sp_randi_compute(data, udp->rnd, NULL, &freq);
    udp->osc->freq = freq;
    sp_osc_compute(data, udp->osc, NULL, &data->out[0]);
}

int main() {
    srand(time(NULL));
    udata ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 1024);
    sp_osc_create(&ud.osc);
    sp_randi_create(&ud.rnd);

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft);
    sp_randi_init(sp, ud.rnd, 12345);
    
    ud.rnd->min = 300;  
    ud.rnd->max = 3000;  

    sp->len = 44100 * 5;

    sp_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_randi_destroy(&ud.rnd);
    sp_destroy(&sp);
    return 0;
}
