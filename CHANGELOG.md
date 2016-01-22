# Change log

1.0: AudioKit
-------------
    - New Modules:
        - oscmorph
        - adsr
        - tadsr
    - ftable bug fix
    - tseq fixes
    - line/expon are now triggerable

0.6 
--------------
    - New Modules:
        - conv: performs partitioned convolution
        - pitchamdf: pitch tracking using average magnitude difference function
        - bandlimited oscillators: saw, square, triangle
        - autowah
        - pink noise generator
        - phaser
        - thresh
        - trand: triggerable RNG
    - pluck rewritten and simplified
    - removed "event" module
    - make examples more consistent
    - prop: added [] to double duration
    - nsmp: reads from ftbl instead of disk
    - tabread rewritten to do linear interpolation
    - attribution for zitarev and tweaks
    - Makefile/config.mk reorganization
    - Improvements towards Faust automation

0.5 "Spectral"
--------------
    - Internal RNG created for cross-platform consistency
    - NO_LIBSOUNDFILE compilation flag
    - config.mk can be a variable for multiple config files
    - kissfft library codebase added
    - gen_padsynth uses kissfft instead of fftw (and is now a default module)
    - FFTs for RISC by John Green added to Soundpipe codebase
    - improved testing interface 
    - Many tests (50+) created
    - Dynamic library generation enabled by default (in addition to static lib)
    - New Modules:
        - mincer
        - line
        - expon
        - tenvx 
        - zitarev
     - New gen routine: load_soundfile
     - Fixes:
        - Removed AAIO dependency in sp_jack
        - fixed interpolation bug with fosc, posc3, and osc
        - better comments in boilerplate code
        - removed seed argument in sp_randi_init
        - osx makefile fixes
        - minor fixes to docgen output

0.4 
---
    - License change: GPL to MIT
    - sp_progress CLI progress bar
    - New modules: scale, butbr, pan2, pareq, tenv2, jcrev, barmodel
    - Modules can now be made with Faust!
    - Testing utility and framework (libsptest) created. See test folder
    - Improved docgen
    - Bug fixes

0.3 "Automation"
----------------
    - New modules:
        - eqfil
        - atone
        - butbr
        - foo (dummy module)
        - pluck
        - delay (static delay with feedback)
        - random
        - diskin
        - in
        - nsmp
        - switch
        - dmetro
        - samphold
    - Module boostrapper with documentation
    - renamed delay to vdelay
    - Documented gen routines
    - Style guide written

0.2.2: "Tables"
---------------
    - New gen routines: gen_line, gen_xline, gen_gauss
    - New Modules: vco, reson, allpass, phasor, comb, tabread
    - Docgen support for gen routines
    - A standardized layout for gen routines
    - Bug fixes relating to docgen, maxdel, and libsndfile valgrind errors
    - Musical example 5 (byte beats!)

0.2.1: "Mo Modules"
-------------------
    - New modules: dist, gbuzz fosc, butbp, drip, clip,
    balance, rms, jitter, lpf18
    - Updated: mode, jack, rpi, tenv
    - 2 new musical examples
    - updated html docgen to work with Lynx better
    - amalgamation of soundpipe sources possible
    - sp_param/sp_set
0.2.0
-----
    - html documentation generator
    - metadata for current modules created in modules/data 
    - improved build instructions for OSX
    - example cleanup
    - module variable cleanup
    - Port of Csound GEN07 (gen_line)
    - Multi-channel and Multi-mono file support
    - Revsc module is now stereo
    - This changelog exists

0.1.0
-----

    initial release
