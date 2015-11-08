#include <fftw3.h>

#define fftw_real double
#define rfftw_plan fftw_plan

typedef struct FFTFREQS {
    int size;
    SPFLOAT *s,*c;
} FFTFREQS;

typedef struct {
    int fftsize;
    fftw_real *tmpfftdata1, *tmpfftdata2;
    rfftw_plan planfftw,planfftw_inv;
} FFTwrapper;

void FFTwrapper_create(FFTwrapper **fw, int fftsize);
void FFTwrapper_destroy(FFTwrapper **fw);

void newFFTFREQS(FFTFREQS *f, int size);
void deleteFFTFREQS(FFTFREQS *f);

void smps2freqs(FFTwrapper *ft, SPFLOAT *smps, FFTFREQS freqs);
void freqs2smps(FFTwrapper *ft, FFTFREQS freqs, SPFLOAT *smps);
