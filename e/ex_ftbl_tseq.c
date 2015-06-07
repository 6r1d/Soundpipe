#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft, *delta, *nn; 
    sp_tenv *tenv;
    sp_dtrig *dt;
    sp_ftbl_seq *seq;
} udata;

void write_osc(sp_data *data, void *ud) {
    udata *udp = ud;
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT dry = 0;
    SPFLOAT trig;
    SPFLOAT dtrig = 0;
    SPFLOAT note = 0;
    if(data->pos == 0){
        trig = 1.0;
    }else{
        trig = 0.0;
    }
    sp_dtrig_compute(data, udp->dt, &trig, &dtrig);
    sp_ftbl_tseq_compute(udp->seq, &dtrig, &note);
    udp->osc->freq = sp_midi2cps(note + 12);
    sp_tenv_compute(data, udp->tenv, &dtrig, &env);
    sp_osc_compute(data, udp->osc, NULL, &osc);
    data->out = osc * env;
}

int main() {
    srand(time(NULL));
    udata ud;
    uint32_t bufsize;
    sp_data *sp;
    sp_create(&sp);
    sp_auxdata rvaux;

    sp_dtrig_create(&ud.dt);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_ftbl_create(sp, &ud.delta, 4);
    sp_ftbl_create(sp, &ud.nn, 4);
    sp_osc_create(&ud.osc);

    ud.delta->tbl[0] = 1.0; 
    ud.delta->tbl[1] = 1.0; 
    ud.delta->tbl[2] = 0.5; 
    ud.delta->tbl[3] = 0.5; 
    
    ud.nn->tbl[0] = 60; 
    ud.nn->tbl[1] = 62; 
    ud.nn->tbl[2] = 64; 
    ud.nn->tbl[3] = 67; 
    
    sp_ftbl_tseq_create(&ud.seq, ud.nn); 
    
    sp_dtrig_init(sp, ud.dt, ud.delta);
    sp_tenv_init(sp, ud.tenv, 0.03, 0.01, 0.3);
    sp_gen_sine(ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 1000;
    ud.osc->amp = 0.5;
    sp_gen_sine(ud.ft);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_dtrig_destroy(&ud.dt);
    sp_ftbl_tseq_destroy(&ud.seq); 
    sp_tenv_destroy(&ud.tenv);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.nn);
    sp_ftbl_destroy(&ud.delta);
    sp_osc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}
