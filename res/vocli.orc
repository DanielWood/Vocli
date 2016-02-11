; Vocli synth engines

/* Audio settings */
sr	   	 =		44100
ksmps	   	 =		2
nchnls	   	 =		2


/* User-defined opcodes */

; mapk: Maps a normalized control signal between two points
		opcode		mapk, k, kii
kin, i1, i2	xin
		xout		i1 + (i2 - i1) * kin
		endop

; tienv: Generates an envelope for portamento instruments
		opcode		tienv, k, ii
iatk, irel	xin
itied		tival
idur		=		abs(p3)
if (p3 < 0 && itied == 0) then
	kenv	linseg		.001, iatk, 1, idur-iatk, 1	; Attack 
elseif (p3 < 0 && itied == 1) then
	kenv	=		1				; Sustain
elseif (p3 > 0 && itied == 1) then
	kenv	linseg		1, idur-irel, 1, irel, 0	; Release
elseif (p3 > 0 && itied == 0) then
	kenv	adsr		iatk, 0, 1, irel		; Standalone A/D
endif
		xout		kenv
		endop

; Cylob voice
;1 start dur   db	pitch	a/d	port	vibatk	vibhz	vibstr	mouth	fqc1	bw1	dBoffs1	fqc2	bw2	dBoffs2	fqc3	bw3	dBoffs3	special
;1 2     3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21
   	 	instr		1
idur		=		abs(p3)
idb		=		p4
ipitch		=		(p5 < 20) ? cpspch(p5) : p5
iatkrel		=		p6
iport		=		p7
ivibatk		limit		p8, .01, idur - .001
ivibhz		=		p9
ivibstr		=		p10
imouth		limit		p11, 0, 1

ifqc1		=		p12
ibw1		limit		p13 / 100, 0, 1.125	; Bandwidth 1 is used as lowpass resonance
idBoffs1	=		p14

ifqc2		=		p15
ibw2		=		p16
idBoffs2	=		p17

ifqc3		=		p18
ibw3		=		p19
idBoffs3	=		p20

; Special is used as table lookup
iwave		=		p21

; Tie value
iskip		tival

; Vibrato fade-out multiplier
ivibfademulti	=		(iskip == 1 && p3 > 0) ? 0 : 1

; Cache parameters for sweeping
		tigoto		skip_init
ilastdb		init		idb
ilastpitch	init		ipitch
ilastvibhz	init		ivibhz
ilastvibstr	init		ivibstr
ilastmouth	init		imouth
ilastfqc1	init		ifqc1
ilastfqc2	init		ifqc2
ilastfqc3	init		ifqc3
ilastbw1	init		ibw1
ilastbw2	init		ibw2
ilastbw3	init		ibw3
ilastdBoffs1	init		idBoffs1
ilastdBoffs2	init		idBoffs2
ilastdBoffs3	init		idBoffs3
ilastwave	init		iwave

; Sweeping envelopes
skip_init:
kvibstr		linseg		iskip * ilastvibstr, ivibatk, ivibstr, idur - ivibatk * 1.5, ivibstr, ivibatk * .5, ivibstr * ivibfademulti
kswp		linseg		0, iport, 1, idur - iport, 1
kdb		mapk		kswp, ilastdb, idb
kpitch		mapk		kswp, ilastpitch, ipitch
kvibhz		mapk		kswp, ilastvibhz, ivibhz
kmouth		mapk		kswp, ilastmouth, imouth
kfqc1		mapk		kswp, ilastfqc1, ifqc1
kfqc2		mapk		kswp, ilastfqc2, ifqc2
kfqc3		mapk		kswp, ilastfqc3, ifqc3
kbw1		mapk		kswp, ilastbw1, ibw1
kbw2		mapk		kswp, ilastbw2, ibw2
kbw3		mapk		kswp, ilastbw3, ibw3
kdBoffs1	mapk		kswp, ilastdBoffs1, idBoffs1
kdBoffs2	mapk		kswp, ilastdBoffs2, idBoffs2
kdBoffs3	mapk		kswp, ilastdBoffs3, idBoffs3
kwave		mapk		kswp, ilastwave, iwave

; Re-cache sweep params
ilastdb		=		idb
ilastpitch	=		ipitch
ilastvibhz	=		ivibhz
ilastvibstr	=		ivibstr
ilastmouth	=		imouth
ilastfqc1	=		ifqc1
ilastfqc2	=		ifqc2
ilastfqc3	=		ifqc3
ilastbw1	=		ibw1
ilastbw2	=		ibw2
ilastbw3	=		ibw3
ilastdBoffs1	=		idBoffs1
ilastdBoffs2	=		idBoffs2
ilastdBoffs3	=		idBoffs3
ilastwave	=		iwave

; Convert dB offsets to scale
kdBoffs1	=		db(kdb + kdBoffs1) / db(kdb)
kdBoffs2	=		db(kdb + kdBoffs2) / db(kdb)
kdBoffs3	=		db(kdb + kdBoffs3) / db(kdb)

kvib		oscili		kvibstr, kvibhz, -1, -iskip	; Vibrato
aosc		oscili		1, kpitch + kvib, iwave, -iskip	; Waveform
anoise		rand		1  	    	  	 	; Noise

; Raw voice signal
asig		=		anoise * kmouth + aosc * (1 - kmouth)

; Filter bank
a1		moogladder	asig, kfqc1, kbw1, iskip
a2		butterbp	asig, kfqc2, kbw2, iskip
a3		butterbp	asig, kfqc3, kbw3, iskip

; Apply dB offsets
a1		*=		kdBoffs1
a2		*=		kdBoffs2
a3		*=		kdBoffs3

; Balance filter outputs to original signal
abal		balance		a1 + a2 + a3, asig, 10, iskip

; Output
kenv		tienv		iatkrel, iatkrel
amix		=		abal * kenv * db(kdb)
		outs		amix, amix
		endin


; FM voice (6 carrier, 1 modulator)
;1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21
;2	start	dur	db	pitch	a/d	port	phonswp	vibatk	vibhz	vibstr	mouth	feedbk	fqc	I	fqc2	index2	dboffs2	fqc3	index3	dboffs3
		instr		2
i1_2pi		=		0.15915494309			; Reciprocal of 2 * pi
idur		=		abs(p3)				; p3 = duration
idb		=		p4				; p4 = amplitude
ipitch		=		p5 < 20 ? cpspch(p5) : p5	; p5 = pitch
iad		=		p6				; p6 = attack/decay
iport		=		p7				; p7 = portamento duration
iphonswp	=		p8				; p8 = phoneme sweep duration
ivibatk		=		p9				; p9 = vibrato attack
ivibhz		=		p10				; p10 = vibrato frequency
ivibstr		=		p11   				; p11 = vibrato strength/depth
imouth		=		p12				; p12 = mouth (wave vs noise mix)
ifeedback	=		p13 * i1_2pi			; p13 = modulation feedback index
ifqc		=		p14				; p14 = formant frequency 1
index		=		p15 * i1_2pi			; p15 = formant 1 modulation index
ifqc2		=		p16				; p16 = formant frequency 2
index2		=		p17 * i1_2pi			; p17 = formant 2 modulation index
idboffs2	=		p18				; p18 = formant 2 amplitude offset
ifqc3		=		p19				; p19 = formant frequency 3
index3		=		p20 * i1_2pi			; p20 = formant 3 modulation index
idboffs3	=		p21				; p21 = formant 3 amplitude offset

; Initialize iterator
ki		init		0

; Initialize sinewave in software BUS
isine		chnget		"sine2^16+1"
if		isine	== 0	then
isine		ftgen		0, 0, 65537, 10, 1
		chnset		isine, "sine2^16+1"
endif

; Tie value
iskip		tival
iendtie		=		(iskip == 1 && p3 > 0) ? 0 : 1		; Multiplier on vibrato fade out

; Cache parameters for sweeping
		tigoto		skip_init
ilastdb		init		idb
ilastpitch	init		ipitch
ilastvibhz	init		ivibhz
ilastvibstr	init		ivibstr
ilastmouth	init		imouth
ilastfeedback	init		ifeedback
ilastfqc	init		ifqc
ilastfqc2	init		ifqc2
ilastfqc3	init		ifqc3
ilastindex	init		index
ilastindex2	init		index2
ilastindex3	init		index3
ilastdboffs2	init		idboffs2
ilastdboffs3	init		idboffs3

; Sweeping envelopes
skip_init:
kvibstr		linseg		iskip * ilastvibstr, ivibatk, ivibstr, idur - ivibatk * 1.5, ivibstr, ivibatk * .5, ivibstr * iendtie
kpitch		linseg		ilastpitch, iport, ipitch, idur - iport, ipitch
kphonswp	linseg		0, iphonswp, 1, idur - iphonswp, 1
kdb		mapk		kphonswp, ilastdb, idb
kvibhz		mapk		kphonswp, ilastvibhz, ivibhz
kmouth		mapk		kphonswp, ilastmouth, imouth
kfeedback	mapk		kphonswp, ilastfeedback, ifeedback
kfqc		mapk		kphonswp, ilastfqc, ifqc
kfqc2		mapk		kphonswp, ilastfqc2, ifqc2
kfqc3		mapk		kphonswp, ilastfqc3, ifqc3
kindex		mapk		kphonswp, ilastindex, index
kindex2		mapk		kphonswp, ilastindex2, index2
kindex3		mapk		kphonswp, ilastindex3, index3
kdboffs2	mapk		kphonswp, ilastdboffs2, idboffs2
kdboffs3	mapk		kphonswp, ilastdboffs3, idboffs3

; Recache sweep parameters
ilastdb		=		idb
ilastpitch	=		ipitch
ilastvibhz	=		ivibhz
ilastvibstr	=		ivibstr
ilastmouth	=		imouth
ilastfeedback	=		ifeedback
ilastfqc	=		ifqc
ilastfqc2	=		ifqc2
ilastfqc3	=		ifqc3
ilastindex	=		index
ilastindex2	=		index2
ilastindex3	=		index3
ilastdboffs2	=		idboffs2
ilastdboffs3	=		idboffs3

; Init quantization arrays
kcenter[]	init		3
klower[]	init		3
kupper[]	init		3
kquant[]	init		3
kdist[]		init		3
klodst[]	init		3
kupdst[]	init		3
kevn[]		init		3
kodd[]		init		3
kevndst[]	init		3
kodddst[]	init		3

; Precalculate center frequencies (ratio)
kcenter[0]	=		kfqc / kpitch
kcenter[1]	=		kfqc2 / kpitch
kcenter[2]	=		kfqc3 / kpitch

; Integer approximations
ki		=		0
quantize:
klower[ki]	=		floor(kcenter[ki])
kupper[ki]	=		ceil(kcenter[ki])
kquant[ki]	=		round(kcenter[ki])
kdist[ki]	=		abs(kquant[ki] - kcenter[ki])
klodst[ki]	=		abs(klower[ki] - kcenter[ki])
kupdst[ki]	=		abs(kupper[ki] - kcenter[ki])
		loop_lt		ki, 1, 3, quantize

; Assign harmonics based on parity
ki		=		0
assign:
if		kupper[ki] % 2 == 0  then
	kevn[ki]	=	kupper[ki]
	kodd[ki]	=	klower[ki]
	kevndst[ki]	=	1 - kupdst[ki]
	kodddst[ki]	=	kupdst[ki]
else
	kevn[ki]	=	klower[ki]
	kodd[ki]	=	kupper[ki]
	kevndst[ki]	=	1 - klodst[ki]
	kodddst[ki]	=	klodst[ki]
endif
		loop_lt		ki, 1, 3, assign

; Vibrato
avib		oscili		kvibstr, kvibhz, isine, -iskip
kpitch		=		kpitch + avib

; Init signal arrays
amod[]		init		3
aevn[]		init		3
aodd[]		init		3
aevnphs[]	init		3
aoddphs[]	init		3
amodulator	init		0

; Phase-synchronous oscillator bank
aphs		phasor		kpitch, -iskip		; Shared phasor
amodulator	tablei		aphs, isine, 1, 0, 1	; Modulator at fundamental frequency
anoise		rand		1
amodulator	=		(amodulator * (1 - kmouth) + anoise * kmouth)

; Apply modulation indices
amod[0]		=		amodulator * kindex
amod[1]		=		amodulator * kindex2
amod[2]		=		amodulator * kindex3

; Bracket formants
; Could be one with an i-rate loop?
aevn[0]		tablei		aphs * kevn[0] + amod[0] + aevn[0] * kfeedback, isine, 1, 0, 1
aevn[1]		tablei		aphs * kevn[1] + amod[1] + aevn[1] * kfeedback, isine, 1, 0, 1
aevn[2]		tablei		aphs * kevn[2] + amod[2] + aevn[2] * kfeedback, isine, 1, 0, 1
aodd[0]		tablei		aphs * kodd[0] + amod[0] + aevn[0] * kfeedback, isine, 1, 0, 1
aodd[1]		tablei		aphs * kodd[1] + amod[1] + aevn[1] * kfeedback, isine, 1, 0, 1
aodd[2]		tablei		aphs * kodd[2] + amod[2] + aevn[2] * kfeedback, isine, 1, 0, 1

; Convert dB offsets to scale
kdboffs2	=		db(kdb - kdboffs2) / db(kdb)
kdboffs3	=		db(kdb - kdboffs3) / db(kdb)

; Crossfade harmonics
; Masks the disconinuities caused by integer shifts in carrier ratios
aevn[0]		=		aevn[0] * kevndst[0]
aevn[1]		=		aevn[1] * kevndst[1] * kdboffs2
aevn[2]		=		aevn[1] * kevndst[2] * kdboffs3
aodd[0]		=		aodd[0] * kodddst[0]
aodd[1]		=		aodd[1] * kodddst[1] * kdboffs2
aodd[2]		=		aodd[2] * kodddst[2] * kdboffs3

; Mix and output
kenv		tienv		iad, iad
amix		init		0
		vincr		amix, aevn[0]
		vincr		amix, aodd[0]
		vincr		amix, aevn[1]
		vincr		amix, aodd[1]
		vincr		amix, aevn[2]
		vincr		amix, aodd[2]		
amix		balance		amix, a(1), 10, iskip
amix		=		amix * kenv * db(kdb)
		outs		amix, amix
		clear		amix
		endin
		
