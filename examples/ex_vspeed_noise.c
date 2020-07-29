#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_vspeed_noise *ns;
} UserData;

void write_noise(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0;
    sp_vspeed_noise_compute(sp, ud->ns, NULL, &sp->out[0]);
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_vspeed_noise_create(&ud.ns);
    sp_vspeed_noise_init(sp, ud.ns);
    ud.ns->amp = 0.8f;
    // mode for a value source can be 0 (white noise),
    // 1 (pink noise), 2 (Brown noise)
    ud.ns->mode = 2;
    // Distance between noise peaks influences
    // the sound in several ways
    ud.ns->peak_distance = 100;
    sp->len = 44100 * 9;
    sp_process(sp, &ud, write_noise);
    sp_vspeed_noise_destroy(&ud.ns);
    sp_destroy(&sp);
    return 0;
}
