#pragma once

struct SynthVoice
{
	float			Volume;
	int				Coarse;
	int				Fine;
	int				Wave;
	int				Pw;
	bool			Ring;
	bool			Sync;
};

struct SynthModulation
{
	int				Source;
	int				Destination;
	float			Amount;
	float			Multiplicator;
};

#ifdef ENABLE_POLYPHONY
// Modulation values to pass to voice rendering
struct VoiceModulation
{
	int				mainPitch;		// Main pitch modulation (affects all oscillators)
	int				mainCoarse;		// Main coarse tuning (semitones)
	int				mainFine;		// Main fine tuning (cents)
	int				oscPitch[4];	// Per-oscillator pitch modulation (4 oscillators)
	float			oscVol[4];		// Per-oscillator volume modulation
	int				oscPw[4];		// Per-oscillator pulse width modulation
	float			lfoSpeed[2];	// LFO speed modulation (2 LFOs)
	float			lfoPitch[2];	// LFO pitch modulation amount
};
#endif

struct SynthProgram
{
	char			Name[64];

	// Global

	float			Volume;
	float			Panning;

	int				Coarse;
	int				Fine;

	// Filter

	float			Cutoff;
	float			Resonance;
	int				FilterType;
	int				FilterMode;

	// C64er Arpeggio

	int				ArpMode;
	int				ArpSpeed;
#ifdef ENABLE_POLYPHONY
	bool			ArpPoly;		// Polyphonic arpeggiator (each voice has independent arp)
#endif

	// Portamento

	bool			PortaMode;
	float			PortaSpeed;

	// Envelopes

	float			Attack[3];
	float			Hold[3];
	float			Decay[3];
	float			Sustain[3];
	float			Release[3];
	
	// LFO's

	float			LfoSpeed[2];
	int				LfoWave[2];
	int				LfoPw[2];
	bool			LfoTrigger[2];

	// Voices

	SynthVoice		Voice[4];
	
	// Modulations

	SynthModulation	Modulations[8];

	float			EnvMod;
};

enum PARAMETERS
{
	pMidiChan = 0,
	pVolume,
	pPanning,
	pCoarse,
	pFine,

	pFilterType,	// 5
	pFilterMode,
	pCutoff,
	pResonance,

	pPortaMode,		// 9
	pPortaSpeed,

	pArpMode,		// 11
	pArpSpeed,
#ifdef ENABLE_POLYPHONY
	pArpPoly,		// Polyphonic arpeggiator mode (0=mono, 1=poly)
#endif

	pOsc1Coarse,	// 14 (if polyphony enabled) or 13
	pOsc1Fine,
	pOsc1Wave,
	pOsc1Pw,
	pOsc1Volume,
	pOsc1Ring,
	pOsc1Sync,

	pOsc2Coarse,	// 20 (if polyphony enabled) or 19
	pOsc2Fine,
	pOsc2Wave,
	pOsc2Pw,
	pOsc2Volume,
	pOsc2Ring,
	pOsc2Sync,

	pOsc3Coarse,	// 27 (if polyphony enabled) or 26
	pOsc3Fine,
	pOsc3Wave,
	pOsc3Pw,
	pOsc3Volume,
	pOsc3Ring,
	pOsc3Sync,

	pEnv1A,			// 34 (if polyphony enabled) or 33
	pEnv1H,
	pEnv1D,
	pEnv1S,
	pEnv1R,

	pEnv2A,			// 39 (if polyphony enabled) or 38
	pEnv2H,
	pEnv2D,
	pEnv2S,
	pEnv2R,

	pEnv3A,			// 44 (if polyphony enabled) or 43
	pEnv3H,	
	pEnv3D,
	pEnv3S,
	pEnv3R,

	pLfo1Speed,		// 49 (if polyphony enabled) or 48
	pLfo1Wave,
	pLfo1Pw,
	pLfo1Trig,

	pLfo2Speed,		// 53 (if polyphony enabled) or 52
	pLfo2Wave,
	pLfo2Pw,
	pLfo2Trig,

	pHfoCoarse,		// 57 (if polyphony enabled) or 56
	pHfoFine,
	pHfoWave,
	pHfoPw,

	pMod1Src,		// 61 (if polyphony enabled) or 60
	pMod1Dest,
	pMod1Amount,
	pMod1Mul,

	pMod2Src,
	pMod2Dest,
	pMod2Amount,
	pMod2Mul,

	pMod3Src,
	pMod3Dest,
	pMod3Amount,
	pMod3Mul,

	pMod4Src,
	pMod4Dest,
	pMod4Amount,
	pMod4Mul,

	pMod5Src,
	pMod5Dest,
	pMod5Amount,
	pMod5Mul,

	pMod6Src,
	pMod6Dest,
	pMod6Amount,
	pMod6Mul,

	pMod7Src,
	pMod7Dest,
	pMod7Amount,
	pMod7Mul,

	pMod8Src,
	pMod8Dest,
	pMod8Amount,
	pMod8Mul,

	pFilterMod,

#ifdef ENABLE_POLYPHONY
	pMaxPolyphony,	// Maximum number of polyphonic voices (1-16)
#endif

	pParameters
};