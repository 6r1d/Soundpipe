#include <stdlib.h>
#include "alsa/asoundlib.h"
#include "soundpipe.h"

void sp_rpi_shutdown (void *arg){
	exit (1);
}

int sp_rpi_process(sp_data *sp, void *ud, void (*callback)(sp_data *, void *)){

    int err;
    unsigned int i;
    SPFLOAT buf[SP_BUFSIZE];
    snd_pcm_t *handle;
	if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		printf("Playback open error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	if ((err = snd_pcm_set_params(handle,
	                              SND_PCM_FORMAT_FLOAT,
	                              SND_PCM_ACCESS_RW_INTERLEAVED,
	                              1,
	                              sp->sr,
	                              1,
	                              500000)) < 0) {	/* 0.5sec */
		printf("Playback open error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
    while(1) {
        for(i = 0; i < SP_BUFSIZE; i++){
            callback(sp, ud);
            buf[i] = sp->out;
        }
        snd_pcm_writei(handle, buf, SP_BUFSIZE);
    }
	snd_pcm_close(handle);
    return SP_OK;
}
