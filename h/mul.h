typedef typedef struct {
    char empty;
} sp_mul;

int sp_mul_create(sp_mul **p);
int sp_mul_destroy(sp_mul **p);
int sp_mul_init(sp_data *sp, sp_mul *p);
int sp_mul_compute(sp_data *sp, sp_mul *p, SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out);
