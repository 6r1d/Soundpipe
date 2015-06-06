typedef struct  {
    SPFLOAT delay;
    uint32_t bufpos;
    uint32_t bufsize;
    sp_auxdata *buf;
} sp_reverse;

int sp_reverse_alloc(sp_data *sp, sp_auxdata *aux, SPFLOAT delay, uint32_t *bufsize);
int sp_reverse_create(sp_data *sp, sp_reverse **p);
int sp_reverse_destroy(sp_reverse **p, sp_auxdata *aux); 
int sp_reverse_init(sp_data *sp, sp_reverse *p, sp_auxdata *aux, uint32_t bufsize);
int sp_reverse_compute(sp_data *sp, sp_reverse *p, SPFLOAT *in, SPFLOAT *out);
