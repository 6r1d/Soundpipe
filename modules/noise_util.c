#include <math.h>
#include "soundpipe.h"

/*
 * Noise generators
 */

// Calculates a single float value for a next white noise state
SPFLOAT sp_white_noise_sample(sp_data *sp, sp_vspeed_noise *ns) {
    SPFLOAT out = ((sp_rand(sp) % SP_RANDMAX) / (SP_RANDMAX * 1.0));
    return (out * 2) - 1;
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
