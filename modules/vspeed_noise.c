#include <math.h>
#include <stdlib.h>
#include "soundpipe.h"
#include "noise_util.c"
#include <stdio.h>

SPFLOAT sp_clamp(SPFLOAT x, SPFLOAT lowerlimit, SPFLOAT upperlimit) {
  if (x < lowerlimit) x = lowerlimit;
  if (x > upperlimit) x = upperlimit;
  return x;
}

// Serp, a mix between smootherstep and lerp functions:
//   scale, and clamp pos to 0..1 range,
//   evaluate polynomial,
//   return a linear interpolation of a polynomial evaluation
SPFLOAT serp(SPFLOAT a, SPFLOAT b, SPFLOAT pos) {
  SPFLOAT x = sp_clamp(pos, 0.0, 1.0);
  SPFLOAT poly = x * x * x * (x * (x * 6 - 15) + 10);
  return a + poly * (b - a);
}

// Sets a next peak value to a sample of a selected noise
int update_next_peak(sp_data *sp, sp_vspeed_noise *ns) {
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
    update_next_peak(sp, ns);
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
        update_next_peak(sp, ns);
    }
    SPFLOAT x = (SPFLOAT)ns->smps_passed / (SPFLOAT)ns->peak_distance;
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
