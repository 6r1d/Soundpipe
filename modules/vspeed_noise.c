#include <math.h>
#include <stdlib.h>
#include "soundpipe.h"
#include "noise_util.c"
#include <stdio.h>

// Serp, a mix between smootherstep and lerp functions
// Scale, and clamp pos to 0..1 range
// Evaluate polynomial
// Return a linear interpolation of a polynomial evaluation
float serp(float a, float b, float pos) {
  float x = sp_clamp(pos, 0.0, 1.0);
  float poly = x * x * x * (x * (x * 6 - 15) + 10);
  return a + poly * (b - a);
}

int update_peak_b(sp_data *sp, sp_vspeed_noise *ns) {
    switch (ns->mode) {
      case 1:
        ns->peak_b = sp_pink_noise_sample(sp, ns);
        break;
      case 2:
        ns->peak_b = sp_brown_noise_sample(sp, ns);
        break;
      default:
        ns->peak_b = sp_white_noise_sample(sp, ns);
    }
}

int sp_vspeed_noise_create(sp_vspeed_noise **ns)
{
    *ns = malloc(sizeof(sp_vspeed_noise));
    return SP_OK;
}

int sp_vspeed_noise_init(sp_data *sp, sp_vspeed_noise *ns)
{
    // Set initial amplitude, mode, etc.
    ns->amp = 1.0;
    ns->mode = 0;
    ns->peak_distance = 1;
    // Set smoothing position and peaks
    ns->smps_passed = 0;
    ns->peak_a = 0.0;
    update_peak_b(sp, ns);
    // Pink noise preparation
    int i;
    ns->amp = 1.0;
    ns->seed = sp_rand(sp);
    ns->total = 0;
    ns->counter = 0;
    for(i = 0; i < 7; i++) {
        ns->dice[i] = 0;
    }
    return SP_OK;
}

int sp_vspeed_noise_compute(sp_data *sp, sp_vspeed_noise *ns, SPFLOAT *in, SPFLOAT *out)
{
    if (ns->smps_passed < ns->peak_distance) {
        ns->smps_passed++;
    } else {
        ns->smps_passed = 0;
        ns->peak_a = ns->peak_b;
        update_peak_b(sp, ns);
    }
    float x = (float)ns->smps_passed / (float)ns->peak_distance;
    *out = serp(ns->peak_a, ns->peak_b, x);
    // Divide Brownian noise by 16
    if (ns->mode == 2) *out *= 0.0625f;
    *out *= ns->amp;
    return SP_OK;
}

int sp_vspeed_noise_destroy(sp_vspeed_noise **ns)
{
    free(*ns);
    return SP_OK;
}
