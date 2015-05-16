#include <stdlib.h>
#include "event.h"

int sp_create_evt_stack(sp_data *sp, int nvoices){
    return 1;
}
int sp_destroy_evt_stack(sp_data *sp, int nvoices){
    return 1;
}
int sp_event_update(sp_event *evt, sp_frame pos){
    if(evt->mode == SPEVT_FREE){
        return SP_OK;
    }else if(evt->mode == SPEVT_ERROR){
        return SP_NOT_OK;
    }else if(pos < evt->start){
        evt->mode = SPEVT_QUEUED;
    }else if(pos >= evt->start && pos < evt->end - 1){
        evt->mode = SPEVT_ON;
    }else if(pos == evt->end - 1 && evt->mode == SPEVT_ON){
        evt->mode = SPEVT_OFF;
    }else if(pos == evt->end && evt->mode == SPEVT_OFF){
        evt->mode = SPEVT_FREE;
    }else {
        return SP_NOT_OK;
    }
    return SP_OK;
}
int sp_event_insert(sp_event *evt, 
    sp_frame cpos, sp_frame start, sp_frame dur,
    void(*init_cb)(void *),
    void(*evton_cb)(void *),
    void(*evtoff_cb)(void *),
    void *ud){ 

    if(evt->mode != SPEVT_FREE) {
            fprintf(stderr, "Error: event mode is not set to SPEVT_FREE. Properly "
                "clear current event before proceeding.\n");
        return SP_NOT_OK;
    }

    if(dur < 1){
        fprintf(stderr, "Error: invalid duration %d.", dur);
        return SP_NOT_OK;
    }
 
   if(cpos > start){
        fprintf(stderr, "Error: Start time %d is past the current time %d.\n", 
                evt->start, cpos);
        evt->mode = SPEVT_ERROR;
        return SP_NOT_OK;
   } 

    evt->start = start;
    evt->end = start + dur;
    evt->init_cb = init_cb; 
    evt->evton_cb = evton_cb; 
    evt->evtoff_cb = evtoff_cb; 
    evt->ud = ud;

    evt->mode = SPEVT_QUEUED;
    if(init_cb != NULL)
    evt->init_cb(evt->ud);
    return SP_OK;
}
int sp_event_exec(sp_event *evt) {
    switch(evt->mode){
    case SPEVT_FREE:
        return SP_NOT_OK;
        break;
    case SPEVT_ON:
        evt->evton_cb(evt->ud);
        return SP_OK;
        break;
    case SPEVT_OFF:
        evt->evtoff_cb(evt->ud);
        return SP_OK;
    case SPEVT_QUEUED:
        return SP_OK;
    default:
        return SP_NOT_OK;
    }
}
int sp_event_clear(sp_event *evt) {
    /* when a note turns itself off, all the freeing should be done in the 
     * evtoff_cb function*/
    evt->evtoff_cb(evt->ud);
    evt->mode = SPEVT_FREE;
    return SP_OK;
}
int sp_event_create(sp_event **evt, int nevents){ 
    *evt = malloc(sizeof(sp_event));
    return SP_OK;
}

int sp_event_init(sp_event *evt){
    evt->mode = SPEVT_FREE;
    return SP_OK;
}
int sp_event_destroy(sp_event **evt){
    free(*evt);
    return SP_NOT_OK;
}

int sp_evtstack_create(sp_evtstack **es, int nevents) {
    int i;
    *es = malloc(sizeof(sp_evtstack));
    sp_evtstack *eptr = *es;

    eptr->nevents = nevents;
    /* figure out how to put this malloc into sp_event_create */
    eptr->evt = malloc(sizeof(sp_event) * nevents); 
    for(i = 0; i < nevents; i++){
        sp_event_init(&eptr->evt[i]);
    }
    return SP_OK;
}
int sp_evtstack_init(sp_evtstack *es){
    es->nxtfree = 0;
    es->lstfree = 0;
    return SP_OK;
}
int sp_evtstack_destroy(sp_evtstack **es){
    int i;
    sp_evtstack *eptr = *es;
    sp_event *evt = eptr->evt;
    free(evt);
    free(*es);
    return SP_OK;
}

int sp_evtstack_add(sp_evtstack *es, 
    sp_frame cpos, sp_frame start, sp_frame dur,
    void(*init_cb)(void *),
    void(*evton_cb)(void *),
    void(*evtoff_cb)(void *),
    void *ud){

    int id = SPEVSTK_NOFREE;
    sp_evtstack_nextfree(es, &id);

    if(id == SPEVSTK_NOFREE){
        printf("Error: no more free voices\n");
        return SP_NOT_OK;
    }

    sp_event *evt = &es->evt[id];

    if(!sp_event_insert(evt, cpos, start, dur, init_cb, evton_cb, evtoff_cb, ud)){
        return SP_NOT_OK;
    }

    return SP_OK;
}

int sp_evtstack_nextfree(sp_evtstack *es, int *id){
    int i, test_free;
    es->lstfree = es->nxtfree;
    if(es->nxtfree == SPEVSTK_NOFREE){
        *id = SPEVSTK_NOFREE;
        return SP_NOT_OK;
    }else if(es->nxtfree == SPEVSTK_SEARCH){
        es->nxtfree = SPEVSTK_NOFREE;
        *id = SPEVSTK_NOFREE;
        for(i = 0; i < es->nevents; i++){
            if(es->evt[i].mode == SPEVT_FREE){
                *id = i;
                break;
            }
        }
        if(es->nxtfree == SPEVSTK_NOFREE){
            return SP_NOT_OK;
        }
    }else{
        *id = es->nxtfree;
    }

    /* See if the next voice in line is free */
    test_free = (*id + 1) % es->nevents;
    if(es->evt[test_free].mode == SPEVT_FREE){
        es->nxtfree = test_free;
    }else{
        es->nxtfree = SPEVSTK_SEARCH;
    }

    return SP_OK;
}

int sp_evtstack_update(sp_evtstack *es, sp_frame pos){
    int i;
    for(i = 0; i < es->nevents; i++){
       sp_event *tmp = &es->evt[i];
       sp_event_update(tmp, pos); 
       if(tmp->mode == SPEVT_OFF){
            es->nxtfree = i;
       }
    }
    return SP_NOT_OK;
}

int sp_evtstack_exec(sp_evtstack *es){
    return SP_NOT_OK;
}
