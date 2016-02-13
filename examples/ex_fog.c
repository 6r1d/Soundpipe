#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_fog *fog;
    sp_ftbl *wav;
    sp_ftbl *win;
    sp_phasor *phs;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT phs = 0, fog = 0;
    sp_phasor_compute(sp, ud->phs, NULL, &phs);
    ud->fog->spd = phs;
    sp_fog_compute(sp, ud->fog, NULL, &fog);
    sp->out[0] = fog;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    //sp_ftbl_loadfile(sp, &ud.wav, "oneart.wav");
    sp_ftbl_create(sp, &ud.wav, 524288);
    sp_ftbl_create(sp, &ud.win, 1024);
    sp_fog_create(&ud.fog);
    sp_phasor_create(&ud.phs);

    sp_gen_file(sp, ud.wav, "oneart.wav");
    sp_gen_sinecomp(sp, ud.win, "0.5 0.5 270 0.5");

    sp_fog_init(sp, ud.fog, ud.wav, ud.win, 100, 0);

    sp_phasor_init(sp, ud.phs, 0);
    ud.phs->freq = 0.1;
    //ud.phs->freq = 1.0;

    sp->len = 44100 * 10;
    sp_process(sp, &ud, process);

    sp_fog_destroy(&ud.fog);
    sp_ftbl_destroy(&ud.wav);
    sp_ftbl_destroy(&ud.win);
    sp_phasor_destroy(&ud.phs);

    sp_destroy(&sp);
    return 0;
}
