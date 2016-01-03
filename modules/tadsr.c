/*
 * Foo
 * 
 * This is a dummy module. It doesn't do much.
 * Feel free to use this as a boilerplate template.
 * 
 */

#include <stdlib.h>
#include "soundpipe.h"

int sp_tadsr_create(sp_tadsr **p)
{
    *p = malloc(sizeof(sp_tadsr));
    return SP_OK;
}

int sp_tadsr_destroy(sp_tadsr **p)
{
    free(*p);
    return SP_OK;
}

int sp_tadsr_init(sp_data *sp, sp_tadsr *p)
{
    /* Initalize variables here. */
    p->bar = 123;
    return SP_OK;
}

int sp_tadsr_compute(sp_data *sp, sp_tadsr *p, SPFLOAT *in, SPFLOAT *out)
{
    /* Send the signal's input to the output */
    *out = *in;
    return SP_OK;
}
