typedef struct{
    // Input values
    SPFLOAT amp;
    int mode;
    int peak_distance;
    // Recalculated values
    int smps_passed;
    SPFLOAT peak_a;
    SPFLOAT peak_b;
    // Pink noise data
    unsigned int newrand;
    unsigned int prevrand;
    unsigned int k;
    unsigned int seed;
    unsigned int total;
    uint32_t counter;
    unsigned int dice[7];
}sp_vspeed_noise;

int sp_vspeed_noise_create(sp_vspeed_noise **ns);
int sp_vspeed_noise_init(sp_data *sp, sp_vspeed_noise *ns);
int sp_vspeed_noise_compute(sp_data *sp, sp_vspeed_noise *ns, SPFLOAT *in, SPFLOAT *out);
int sp_vspeed_noise_destroy(sp_vspeed_noise **ns);
