#include <math.h>
#include "soundpipe.h"

/*
 * Constant
 */

// Used by a pink noise generator
static uint32_t ctz[64] =
{
    6, 0, 1, 0, 2, 0, 1, 0,
    3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0,
    3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0,
    3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0,
    3, 0, 1, 0, 2, 0, 1, 0,
};

/*
 * Noise generators
 */

// Calculates a single float value for a next white noise state
SPFLOAT sp_white_noise_sample(sp_data *sp, sp_vspeed_noise *ns) {
    SPFLOAT out = ((sp_rand(sp) % SP_RANDMAX) / (SP_RANDMAX * 1.0));
    return (out * 2) - 1;
}

// Calculates a single float value for a next pink noise state
SPFLOAT sp_pink_noise_sample(sp_data *sp, sp_vspeed_noise *ns) {
    uint32_t k = ctz[ns->counter & 63];
    ns->prevrand = ns->dice[k];
    ns->seed = 1664525 * ns->seed + 1013904223;
    ns->newrand = ns->seed >> 3;
    ns->dice[k] = ns->newrand;
    ns->total += (ns->newrand - ns->prevrand);
    ns->seed = 1103515245 * ns->seed + 12345;
    ns->newrand = ns->seed >> 3;
    short tmp = (short) ((((ns->total + ns->newrand) * (1.0f / (3 << 29)) - 1) - .25f) * 16384.0f);
    ns->counter = (ns->counter + 1) % 0xFFFFFFFF;
    return ((SPFLOAT) tmp / 32767) * ns->amp;
}

// Calculates a single float value for a next Brownian noise state
// Has a fixed amplitude, 0.0625f
SPFLOAT sp_brown_noise_sample(sp_data *sp, sp_vspeed_noise *ns) {
    SPFLOAT r, m_brown = ns->peak_a;
    while(1) {
        r = sp_white_noise_sample(sp, ns) * 0.5;
        m_brown += r;
        if(m_brown < -8.0f || m_brown > 8.0f) m_brown -= r;
        else break;
    }
    return m_brown;
}
