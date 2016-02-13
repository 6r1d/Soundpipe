# Modules that don't require external libraries go here
MODULES= \
base \
ftbl \
tevent \
adsr \
allpass \
atone \
autowah \
bal \
bar \
biquad \
biscale \
blsaw \
blsquare \
bltriangle \
fold \
bitcrush \
butbp \
butbr \
buthp \
butlp \
clip \
comb \
count \
conv \
dcblock \
delay \
dist \
dmetro \
drip \
dtrig \
dust \
eqfil \
expon \
fof \
fofilt \
foo \
fosc \
gbuzz \
in \
jcrev \
jitter \
line \
lpf18 \
maygate \
metro \
mincer \
mode \
moogladder \
noise \
osc \
oscmorph \
pan2 \
panst \
pareq \
phaser \
phasor \
pinknoise \
pitchamdf \
pluck \
port \
posc3 \
progress \
prop \
randh \
randi \
randmt \
random \
reverse \
revsc \
rms \
rpt \
samphold \
scale \
streson \
switch \
tabread \
tadsr \
tbvcf \
tenv \
tenv2 \
tenvx \
thresh \
tin \
tone \
trand \
tseq \
vdelay \
zitarev

ifndef NO_LIBSNDFILE
	MODULES += nsmp diskin
else
	CFLAGS += -DNO_LIBSNDFILE
endif

# ini parser needed for nsmp module
include lib/inih/Makefile

# Header files needed for modules generated with FAUST
CFLAGS += -I lib/faust

# fft library
include lib/fft/Makefile

# JACK module
#
#MODULES += jack
#CFLAGS += -ljack

# RPi Module
#
#MODULES += rpi
#CFLAGS += -lasound

include lib/kissfft/Makefile
MODULES += fftwrapper
MODULES += padsynth

# Uncomment to use FFTW3 instead of kissfft.
# CFLAGS += -lfftw3 -DUSE_FFTW3

# Build dynamic library. Needed for Sporth chugin
# CFLAGS += -shared -fPIC
# BUILD_DYNAMIC=1
