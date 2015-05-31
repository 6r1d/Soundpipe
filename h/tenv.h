typedef struct sp_tenv{
    sp_tevent *te;
    uint32_t pos, atk_end, rel_start, sr, totaldur;
    SPFLOAT atk, rel, hold;
    SPFLOAT atk_slp, rel_slp;
    SPFLOAT last;
} sp_tenv;

int sp_tenv_create(sp_tenv **p);
int sp_tenv_destroy(sp_tenv **p);
int sp_tenv_init(sp_data *sp, sp_tenv *p, SPFLOAT atk, SPFLOAT hold, SPFLOAT rel);
void sp_tenv_reinit(void *ud);
void sp_tenv_comp(void *ud, SPFLOAT *out);
int sp_tenv_compute(sp_data *sp, sp_tenv *p, SPFLOAT *in, SPFLOAT *out);
