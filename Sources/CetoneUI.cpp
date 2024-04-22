#include "CetoneUI.hpp"
#include "Defines.h"
#include "Fonts/CetoneFonts.hpp"
#include "Images/CetoneArtwork.hpp"
#include "Structures.h"

namespace Art = CetoneArtwork;
namespace Fonts = CetoneFonts;

CCetoneUI::CCetoneUI()
	: DISTRHO::UI(Art::guiWidth, Art::guiHeight)
	, fImgBackground(Art::guiData, Art::guiWidth, Art::guiHeight, kImageFormatBGR)
	, fImgKnob(Art::knobData, Art::knobWidth, Art::knobHeight, kImageFormatBGRA)
	, fImgSwitchButton_ON(Art::buttons_onData, Art::buttons_onWidth, Art::buttons_onHeight, kImageFormatBGR)
	, fImgSwitchButton_OFF(Art::buttons_offData, Art::buttons_offWidth, Art::buttons_offHeight, kImageFormatBGR)
{
	/* Initialize NanoVG font and text buffer */
	NanoVG::FontId font = fNanoText.createFontFromMemory("Source Sans Regular", Fonts::SourceSans3_RegularData, Fonts::SourceSans3_RegularDataSize, false);
	fNanoText.fontFaceId(font);
	memset(fLabelBuffer, '\0', sizeof(char) * (32 + 1));

	/* Initialize knobs */
	// NOTICE: Default values comes from CCetoneSynth::InitSynthParameters().
	//         To fit with DSP side on actual run, some values here are different from InitSynthParameters().
	_createKnob(fKnobVolume, pVolume, 212, 40, 0.2f);
	_createKnob(fKnobPanning, pPanning, 212 + 48, 40, 0.5f);
	_createKnob(fKnobOsc1Volume, pOsc1Volume, 212 + 48 * 2, 40, 0.2f);
	_createKnob(fKnobOsc2Volume, pOsc2Volume, 212 + 48 * 3, 40, 0.2f);
	_createKnob(fKnobOsc3Volume, pOsc3Volume, 212 + 48 * 4, 40, 0.2f);

	_createKnob(fKnobFilterType, pFilterType, 464, 40, _pi2f(FTYPE_NONE, FTYPE_MAX));
	_createKnob(fKnobFilterMode, pFilterMode, 464 + 48, 40, _pi2f(FMODE_LOW, FMODE_MAX));
	_createKnob(fKnobCutoff, pCutoff, 464 + 48 * 2, 40, 1.0f);
	_createKnob(fKnobResonance, pResonance, 464 + 48 * 3, 40, 0.0f);
	_createKnob(fKnobFilterParameter, pFilterMod, 464 + 48 * 4, 40, 0.5f);

	_createKnob(fKnobCoarse, pCoarse, 716, 40, 0.5f);
	_createKnob(fKnobFine, pFine, 716 + 48, 40, 0.5f);

	_createKnob(fKnobOsc1Coarse, pOsc1Coarse, 8, 150, 0.5f); // 0
	_createKnob(fKnobOsc1Fine, pOsc1Fine, 8 + 48, 150, 0.5f); // 0
	_createKnob(fKnobOsc1Waveform, pOsc1Wave, 8 + 48 * 2, 150, _pi2f(OWAVE_SAW, OWAVE_MAX));
	_createKnob(fKnobOsc1PulseWidth, pOsc1Pw, 8 + 48 * 3, 150, 0.5f); // 32768

	_createKnob(fKnobOsc2Coarse, pOsc2Coarse, 8, 260, 0.62f); // +12
	_createKnob(fKnobOsc2Fine, pOsc2Fine, 8 + 48, 260, 0.5f); // 0
	_createKnob(fKnobOsc2Waveform, pOsc2Wave, 8 + 48 * 2, 260, _pi2f(OWAVE_SAW, OWAVE_MAX));
	_createKnob(fKnobOsc2PulseWidth, pOsc2Pw, 8 + 48 * 3, 260, 0.5f); // 32768

	_createKnob(fKnobOsc3Coarse, pOsc3Coarse, 8, 370, 0.38f); // -12
	_createKnob(fKnobOsc3Fine, pOsc3Fine, 8 + 48, 370, 0.5f); // 0
	_createKnob(fKnobOsc3Waveform, pOsc3Wave, 8 + 48 * 2, 370, _pi2f(OWAVE_SAW, OWAVE_MAX));
	_createKnob(fKnobOsc3PulseWidth, pOsc3Pw, 8 + 48 * 3, 370, 0.5f); // 32768

	_createKnob(fKnobGlideSpeed, pPortaSpeed, 716 + 48, 150, 0.02f); // 2.0

	_createKnob(fAmpAttack, pEnv1A, 260, 150, 0.001f);
	_createKnob(fAmpHold, pEnv1H, 260 + 48, 150, 0.002f);
	_createKnob(fAmpDecay, pEnv1D, 260 + 48 * 2, 150, 0.023f);
	_createKnob(fAmpSustain, pEnv1S, 260 + 48 * 3, 150, 0.75f);
	_createKnob(fAmpRelease, pEnv1R, 260 + 48 * 4, 150, 0.05f);

	_createKnob(fMod1Attack, pEnv2A, 260, 260, 0.f);
	_createKnob(fMod1Hold, pEnv2H, 260 + 48, 260, 0.f);
	_createKnob(fMod1Decay, pEnv2D, 260 + 48 * 2, 260, 0.f);
	_createKnob(fMod1Sustain, pEnv2S, 260 + 48 * 3, 260, 0.f);
	_createKnob(fMod1Release, pEnv2R, 260 + 48 * 4, 260, 0.f);

	_createKnob(fMod2Attack, pEnv3A, 260, 370, 0.f);
	_createKnob(fMod2Hold, pEnv3H, 260 + 48, 370, 0.f);
	_createKnob(fMod2Decay, pEnv3D, 260 + 48 * 2, 370, 0.f);
	_createKnob(fMod2Sustain, pEnv3S, 260 + 48 * 3, 370, 0.f);
	_createKnob(fMod2Release, pEnv3R, 260 + 48 * 4, 370, 0.f);

	_createKnob(fLfo1Speed, pLfo1Speed, 512, 150, 0.001f);
	_createKnob(fLfo1Waveform, pLfo1Wave, 512 + 48, 150, _pi2f(WAVE_SINE, WAVE_MAX));
	_createKnob(fLfo1PulseWidth, pLfo1Pw, 512 + 48 * 2, 150, 0.5f); // 32768

	_createKnob(fLfo2Speed, pLfo2Speed, 512, 260, 0.001f);
	_createKnob(fLfo2Waveform, pLfo2Wave, 512 + 48, 260, _pi2f(WAVE_SINE, WAVE_MAX));
	_createKnob(fLfo2PulseWidth, pLfo2Pw, 512 + 48 * 2, 260, 0.5f); // 32768

	_createKnob(fHfoCoarse, pHfoCoarse, 512, 370, 0.5f); // 0
	_createKnob(fHfoFine, pHfoFine, 512 + 48, 370, 0.5f); // 0
	_createKnob(fHfoWaveform, pHfoWave, 512 + 48 * 2, 370, _pi2f(OWAVE_SAW, OWAVE_MAX));
	_createKnob(fHfoPulseWidth, pHfoPw, 512 + 48 * 3, 370, 0.5f); // 32768

	constexpr auto DEFAULT_ARP_MODE = -1;
	_createKnob(fArpMode, pArpMode, 716, 260, _pi2f(DEFAULT_ARP_MODE + 1, ARP_MAX + 1)); // See CCetoneSynth::getParameter();
	_createKnob(fArpSpeed, pArpSpeed, 716 + 48, 260, 0.04f); // 480

	_createKnob(fMod1Source, pMod1Src, 8, 480, 0.f);
	_createKnob(fMod1Destination, pMod1Dest, 8 + 48, 480, 0.f);
	_createKnob(fMod1Amount, pMod1Amount, 8 + 48 * 2, 480, 0.5f);
	_createKnob(fMod1Multiply, pMod1Mul, 8 + 48 * 3, 480, 0.01f);

	_createKnob(fMod2Source, pMod2Src, 212, 480, 0.f);
	_createKnob(fMod2Destination, pMod2Dest, 212 + 48, 480, 0.f);
	_createKnob(fMod2Amount, pMod2Amount, 212 + 48 * 2, 480, 0.5f);
	_createKnob(fMod2Multiply, pMod2Mul, 212 + 48 * 3, 480, 0.01f);

	_createKnob(fMod3Source, pMod3Src, 416, 480, 0.f);
	_createKnob(fMod3Destination, pMod3Dest, 416 + 48, 480, 0.f);
	_createKnob(fMod3Amount, pMod3Amount, 416 + 48 * 2, 480, 0.5f);
	_createKnob(fMod3Multiply, pMod3Mul, 416 + 48 * 3, 480, 0.01f);

	_createKnob(fMod4Source, pMod4Src, 620, 480, 0.f);
	_createKnob(fMod4Destination, pMod4Dest, 620 + 48, 480, 0.f);
	_createKnob(fMod4Amount, pMod4Amount, 620 + 48 * 2, 480, 0.5f);
	_createKnob(fMod4Multiply, pMod4Mul, 620 + 48 * 3, 480, 0.01f);

	_createKnob(fMod5Source, pMod5Src, 8, 590, 0.f);
	_createKnob(fMod5Destination, pMod5Dest, 8 + 48, 590, 0.f);
	_createKnob(fMod5Amount, pMod5Amount, 8 + 48 * 2, 590, 0.5f);
	_createKnob(fMod5Multiply, pMod5Mul, 8 + 48 * 3, 590, 0.01f);

	_createKnob(fMod6Source, pMod6Src, 212, 590, 0.f);
	_createKnob(fMod6Destination, pMod6Dest, 212 + 48, 590, 0.f);
	_createKnob(fMod6Amount, pMod6Amount, 212 + 48 * 2, 590, 0.5f);
	_createKnob(fMod6Multiply, pMod6Mul, 212 + 48 * 3, 590, 0.01f);

	_createKnob(fMod7Source, pMod7Src, 416, 590, 0.f);
	_createKnob(fMod7Destination, pMod7Dest, 416 + 48, 590, 0.f);
	_createKnob(fMod7Amount, pMod7Amount, 416 + 48 * 2, 590, 0.5f);
	_createKnob(fMod7Multiply, pMod7Mul, 416 + 48 * 3, 590, 0.01f);

	_createKnob(fMod8Source, pMod8Src, 620, 590, 0.f);
	_createKnob(fMod8Destination, pMod8Dest, 620 + 48, 590, 0.f);
	_createKnob(fMod8Amount, pMod8Amount, 620 + 48 * 2, 590, 0.5f);
	_createKnob(fMod8Multiply, pMod8Mul, 620 + 48 * 3, 590, 0.01f);

	_createKnob(fMidiChannel, pMidiChan, 740, 370, 0.0f);

	/* Initialize switches */
	_createSwitchButton(fBtnOsc1Ring, pOsc1Ring, 200, 150);
	_createSwitchButton(fBtnOsc2Ring, pOsc2Ring, 200, 260);
	_createSwitchButton(fBtnOsc3Ring, pOsc3Ring, 200, 370);

	_createSwitchButton(fBtnOsc1Sync, pOsc1Sync, 200, 200);
	_createSwitchButton(fBtnOsc2Sync, pOsc2Sync, 200, 310);
	_createSwitchButton(fBtnOsc3Sync, pOsc3Sync, 200, 420);

	_createSwitchButton(fBtnLfo1Trigger, pLfo1Trig, 656, 150);
	_createSwitchButton(fBtnLfo2Trigger, pLfo2Trig, 656, 260);

	_createSwitchButton(fBtnGlideState, pPortaMode, 716, 150);
}

void CCetoneUI::parameterChanged(uint32_t index, float value)
{
	switch (index) {
	// Mixer
	case pVolume:
		fKnobVolume->setValue(value);
		break;
	case pPanning:
		fKnobPanning->setValue(value);
		break;
	case pOsc1Volume:
		fKnobOsc1Volume->setValue(value);
		break;
	case pOsc2Volume:
		fKnobOsc2Volume->setValue(value);
		break;
	case pOsc3Volume:
		fKnobOsc3Volume->setValue(value);
		break;

	// Filter
	case pFilterType:
		fKnobFilterType->setValue(value);
		break;
	case pFilterMode:
		fKnobFilterMode->setValue(value);
		break;
	case pCutoff:
		fKnobCutoff->setValue(value);
		break;
	case pResonance:
		fKnobResonance->setValue(value);
		break;
	case pFilterMod:
		fKnobFilterParameter->setValue(value);
		break;

	// Oscillator 1
	case pOsc1Coarse:
		fKnobOsc1Coarse->setValue(value);
		break;
	case pOsc1Fine:
		fKnobOsc1Fine->setValue(value);
		break;
	case pOsc1Wave:
		fKnobOsc1Waveform->setValue(value);
		break;
	case pOsc1Pw:
		fKnobOsc1PulseWidth->setValue(value);
		break;
	case pOsc1Ring:
		fBtnOsc1Ring->setDown(value == 1.0f ? true : false);
		break;
	case pOsc1Sync:
		fBtnOsc1Sync->setDown(value == 1.0f ? true : false);
		break;

	// Oscillator 2
	case pOsc2Coarse:
		fKnobOsc2Coarse->setValue(value);
		break;
	case pOsc2Fine:
		fKnobOsc2Fine->setValue(value);
		break;
	case pOsc2Wave:
		fKnobOsc2Waveform->setValue(value);
		break;
	case pOsc2Pw:
		fKnobOsc2PulseWidth->setValue(value);
		break;
	case pOsc2Ring:
		fBtnOsc2Ring->setDown(value == 1.0f ? true : false);
		break;
	case pOsc2Sync:
		fBtnOsc2Sync->setDown(value == 1.0f ? true : false);
		break;

	// Oscillator 3
	case pOsc3Coarse:
		fKnobOsc3Coarse->setValue(value);
		break;
	case pOsc3Fine:
		fKnobOsc3Fine->setValue(value);
		break;
	case pOsc3Wave:
		fKnobOsc3Waveform->setValue(value);
		break;
	case pOsc3Pw:
		fKnobOsc3PulseWidth->setValue(value);
		break;
	case pOsc3Ring:
		fBtnOsc3Ring->setDown(value == 1.0f ? true : false);
		break;
	case pOsc3Sync:
		fBtnOsc3Sync->setDown(value == 1.0f ? true : false);
		break;

	// Tuning
	case pCoarse:
		fKnobCoarse->setValue(value);
		break;
	case pFine:
		fKnobFine->setValue(value);
		break;

	// Glide (portamento)
	case pPortaMode:
		fBtnGlideState->setDown(value == 1.0f ? true : false);
		break;
	case pPortaSpeed:
		fKnobGlideSpeed->setValue(value);
		break;

	// Amplifier Envelope
	case pEnv1A:
		fAmpAttack->setValue(value);
		break;
	case pEnv1H:
		fAmpHold->setValue(value);
		break;
	case pEnv1D:
		fAmpDecay->setValue(value);
		break;
	case pEnv1S:
		fAmpSustain->setValue(value);
		break;
	case pEnv1R:
		fAmpRelease->setValue(value);
		break;

	// Modulation Envelope 1
	case pEnv2A:
		fMod1Attack->setValue(value);
		break;
	case pEnv2H:
		fMod1Hold->setValue(value);
		break;
	case pEnv2D:
		fMod1Decay->setValue(value);
		break;
	case pEnv2S:
		fMod1Sustain->setValue(value);
		break;
	case pEnv2R:
		fMod1Release->setValue(value);
		break;

	// Modulation Envelope 2
	case pEnv3A:
		fMod2Attack->setValue(value);
		break;
	case pEnv3H:
		fMod2Hold->setValue(value);
		break;
	case pEnv3D:
		fMod2Decay->setValue(value);
		break;
	case pEnv3S:
		fMod2Sustain->setValue(value);
		break;
	case pEnv3R:
		fMod2Release->setValue(value);
		break;

	// LFO 1
	case pLfo1Speed:
		fLfo1Speed->setValue(value);
		break;
	case pLfo1Wave:
		fLfo1Waveform->setValue(value);
		break;
	case pLfo1Pw:
		fLfo1PulseWidth->setValue(value);
		break;
	case pLfo1Trig:
		fBtnLfo1Trigger->setDown(value == 1.0f ? true : false);
		break;

	// LFO 2
	case pLfo2Speed:
		fLfo2Speed->setValue(value);
		break;
	case pLfo2Wave:
		fLfo2Waveform->setValue(value);
		break;
	case pLfo2Pw:
		fLfo2PulseWidth->setValue(value);
		break;
	case pLfo2Trig:
		fBtnLfo2Trigger->setDown(value == 1.0f ? true : false);
		break;

	// HFO
	case pHfoCoarse:
		fHfoCoarse->setValue(value);
		break;
	case pHfoFine:
		fHfoFine->setValue(value);
		break;
	case pHfoWave:
		fHfoWaveform->setValue(value);
		break;
	case pHfoPw:
		fHfoPulseWidth->setValue(value);
		break;

	// Arp
	case pArpMode:
		fArpMode->setValue(value);
		break;
	case pArpSpeed:
		fArpSpeed->setValue(value);
		break;

	// Modulation Slot 1
	case pMod1Src:
		fMod1Source->setValue(value);
		break;
	case pMod1Dest:
		fMod1Destination->setValue(value);
		break;
	case pMod1Amount:
		fMod1Amount->setValue(value);
		break;
	case pMod1Mul:
		fMod1Multiply->setValue(value);
		break;

	// Modulation Slot 2
	case pMod2Src:
		fMod2Source->setValue(value);
		break;
	case pMod2Dest:
		fMod2Destination->setValue(value);
		break;
	case pMod2Amount:
		fMod2Amount->setValue(value);
		break;
	case pMod2Mul:
		fMod2Multiply->setValue(value);
		break;

	// Modulation Slot 3
	case pMod3Src:
		fMod3Source->setValue(value);
		break;
	case pMod3Dest:
		fMod3Destination->setValue(value);
		break;
	case pMod3Amount:
		fMod3Amount->setValue(value);
		break;
	case pMod3Mul:
		fMod3Multiply->setValue(value);
		break;

	// Modulation Slot 4
	case pMod4Src:
		fMod4Source->setValue(value);
		break;
	case pMod4Dest:
		fMod4Destination->setValue(value);
		break;
	case pMod4Amount:
		fMod4Amount->setValue(value);
		break;
	case pMod4Mul:
		fMod4Multiply->setValue(value);
		break;

	// Modulation Slot 5
	case pMod5Src:
		fMod5Source->setValue(value);
		break;
	case pMod5Dest:
		fMod5Destination->setValue(value);
		break;
	case pMod5Amount:
		fMod5Amount->setValue(value);
		break;
	case pMod5Mul:
		fMod5Multiply->setValue(value);
		break;

	// Modulation Slot 6
	case pMod6Src:
		fMod6Source->setValue(value);
		break;
	case pMod6Dest:
		fMod6Destination->setValue(value);
		break;
	case pMod6Amount:
		fMod6Amount->setValue(value);
		break;
	case pMod6Mul:
		fMod6Multiply->setValue(value);
		break;

	// Modulation Slot 7
	case pMod7Src:
		fMod7Source->setValue(value);
		break;
	case pMod7Dest:
		fMod7Destination->setValue(value);
		break;
	case pMod7Amount:
		fMod7Amount->setValue(value);
		break;
	case pMod7Mul:
		fMod7Multiply->setValue(value);
		break;

	// Modulation Slot 8
	case pMod8Src:
		fMod8Source->setValue(value);
		break;
	case pMod8Dest:
		fMod8Destination->setValue(value);
		break;
	case pMod8Amount:
		fMod8Amount->setValue(value);
		break;
	case pMod8Mul:
		fMod8Multiply->setValue(value);
		break;

	// MIDI Channel
	case pMidiChan:
		fMidiChannel->setValue(value);
		break;

	default:
		d_stderr2("WARNING: unrecognized parameter %d", index);
		break;
	}

	// Explicitly ask DPF to redraw UI (for updating labels)
	repaint();
}

// -------------------------------------------------------------------
// Widget Callbacks

void CCetoneUI::imageButtonClicked(ImageButton* button, int)
{
#if 0
    switch (button->getId()) {
    case BTN_PANIC: {
        panic();
        break;
    }
    }
#endif
}

void CCetoneUI::imageSwitchClicked(ImageSwitch* button, bool down)
{
	setParameterValue(button->getId(), down);
}

void CCetoneUI::imageKnobDragStarted(ImageKnob* knob)
{
	editParameter(knob->getId(), true);
}

void CCetoneUI::imageKnobDragFinished(ImageKnob* knob)
{
	editParameter(knob->getId(), false);
}

void CCetoneUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
	setParameterValue(knob->getId(), value);

	// Explicitly ask DPF to redraw UI (for updating labels)
	repaint();
}

void CCetoneUI::imageSliderDragStarted(ImageSlider* slider)
{
	editParameter(slider->getId(), true);
}

void CCetoneUI::imageSliderDragFinished(ImageSlider* slider)
{
	editParameter(slider->getId(), false);
}

void CCetoneUI::imageSliderValueChanged(ImageSlider* slider, float value)
{
	setParameterValue(slider->getId(), value);
}

void CCetoneUI::onDisplay()
{
	/**
	 * Draw background
	 */
	const GraphicsContext& context(getGraphicsContext());

	fImgBackground.draw(context);

	/**
	 * Draw labels
	 * NOTICE: Must invoke `repaint()` when tuning widgets, otherwise UI won't update.
	 */
	constexpr float r = 255.0f;
	constexpr float g = 255.0f;
	constexpr float b = 255.0f;

	fNanoText.beginFrame(this);
	fNanoText.fontSize(18);
	fNanoText.textAlign(NanoVG::ALIGN_CENTER | NanoVG::ALIGN_MIDDLE);

	fNanoText.fillColor(Color(r, g, b));

	/* Mixer */
	{
		// Volume
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobVolume->getValue() * 100.0f);
		fNanoText.textBox(214, 96, 45.0f, fLabelBuffer);

		// Panning
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobPanning->getValue() * 100.0f - 50.0f);
		fNanoText.textBox(214 + 48, 96, 45.0f, fLabelBuffer);

		// OSC Volumes
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobOsc1Volume->getValue() * 100.0f);
		fNanoText.textBox(214 + 48 * 2, 96, 45.0f, fLabelBuffer);

		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobOsc2Volume->getValue() * 100.0f);
		fNanoText.textBox(214 + 48 * 3, 96, 45.0f, fLabelBuffer);

		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobOsc3Volume->getValue() * 100.0f);
		fNanoText.textBox(214 + 48 * 4, 96, 45.0f, fLabelBuffer);
	}

	/* Filter */
	{
		// Type
		std::snprintf(fLabelBuffer, 32, "%s", _filterType2Str(_pf2i(fKnobFilterType->getValue(), FTYPE_MAX)));
		fNanoText.textBox(466, 96, 47.0f, fLabelBuffer);

		// Mode
		// FIXME: Some filter types does not support setting mode.
		const auto _filterMode = _pf2i(fKnobFilterMode->getValue(), FMODE_MAX);
		std::snprintf(fLabelBuffer, 32, "%s", _filterMode2str(_filterMode));
		fNanoText.textBox(466 + 48, 96, 45.0f, fLabelBuffer);

		// Cutoff
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobCutoff->getValue() * 100.0f);
		fNanoText.textBox(466 + 48 * 2, 96, 45.0f, fLabelBuffer);

		// Resonance
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobResonance->getValue() * 100.0f);
		fNanoText.textBox(466 + 48 * 3, 96, 45.0f, fLabelBuffer);

		// Filter Parameter (Filter Mod)
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobFilterParameter->getValue() * 100.0f);
		fNanoText.textBox(466 + 48 * 4, 96, 45.0f, fLabelBuffer);
	}

	/* Tuning */
	{
		// Coarse
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2coarse(fKnobCoarse->getValue()));
		fNanoText.textBox(718, 96, 45.0f, fLabelBuffer);

		// Fine
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2fine(fKnobFine->getValue()));
		fNanoText.textBox(718 + 48, 96, 45.0f, fLabelBuffer);
	}

	/* Oscillator 1 */
	{
		// Osc1 Coarse
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2coarse(fKnobOsc1Coarse->getValue()));
		fNanoText.textBox(10, 206, 45.0f, fLabelBuffer);

		// Osc1 Fine
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2fine(fKnobOsc1Fine->getValue()));
		fNanoText.textBox(10 + 48, 206, 45.0f, fLabelBuffer);

		// Osc1 Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _OscWave2Str(_pf2i(fKnobOsc1Waveform->getValue(), OWAVE_MAX)));
		fNanoText.textBox(10 + 48 * 2, 206, 45.0f, fLabelBuffer);

		// Osc1 Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fKnobOsc1PulseWidth->getValue()));
		fNanoText.textBox(10 + 48 * 3, 206, 45.0f, fLabelBuffer);
	}

	/* Oscillator 2 */
	{
		// Osc2 Coarse
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2coarse(fKnobOsc2Coarse->getValue()));
		fNanoText.textBox(10, 316, 45.0f, fLabelBuffer);

		// Osc2 Fine
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2fine(fKnobOsc2Fine->getValue()));
		fNanoText.textBox(10 + 48, 316, 45.0f, fLabelBuffer);

		// Osc2 Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _OscWave2Str(_pf2i(fKnobOsc2Waveform->getValue(), OWAVE_MAX)));
		fNanoText.textBox(10 + 48 * 2, 316, 45.0f, fLabelBuffer);

		// Osc2 Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fKnobOsc2PulseWidth->getValue()));
		fNanoText.textBox(10 + 48 * 3, 316, 45.0f, fLabelBuffer);
	}

	/* Oscillator 3 */
	{
		// Osc3 Coarse
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2coarse(fKnobOsc3Coarse->getValue()));
		fNanoText.textBox(10, 426, 45.0f, fLabelBuffer);

		// Osc3 Fine
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2fine(fKnobOsc3Fine->getValue()));
		fNanoText.textBox(10 + 48, 426, 45.0f, fLabelBuffer);

		// Osc3 Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _OscWave2Str(_pf2i(fKnobOsc3Waveform->getValue(), OWAVE_MAX)));
		fNanoText.textBox(10 + 48 * 2, 426, 45.0f, fLabelBuffer);

		// Osc3 Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fKnobOsc3PulseWidth->getValue()));
		fNanoText.textBox(10 + 48 * 3, 426, 45.0f, fLabelBuffer);
	}

	/* Glide (portamento) */
	{
		// Speed
		std::snprintf(fLabelBuffer, 32, "%.1f", fKnobGlideSpeed->getValue() * 100.0f);
		fNanoText.textBox(766, 206, 45.0f, fLabelBuffer);
	}

	/* Amplifier Envelope */
	{
		// Attack
		std::snprintf(fLabelBuffer, 32, "%.2f", fAmpAttack->getValue() * 100.0f);
		fNanoText.textBox(262, 206, 45.0f, fLabelBuffer);

		// Hold
		std::snprintf(fLabelBuffer, 32, "%.2f", fAmpHold->getValue() * 100.0f);
		fNanoText.textBox(262 + 48, 206, 45.0f, fLabelBuffer);

		// Decay
		std::snprintf(fLabelBuffer, 32, "%.2f", fAmpDecay->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 2, 206, 45.0f, fLabelBuffer);

		// Sustain
		std::snprintf(fLabelBuffer, 32, "%.2f", fAmpSustain->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 3, 206, 45.0f, fLabelBuffer);

		// Release
		std::snprintf(fLabelBuffer, 32, "%.2f", fAmpRelease->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 4, 206, 45.0f, fLabelBuffer);
	}

	/* Modulation Envelope 1 */
	{
		// Attack
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod1Attack->getValue() * 100.0f);
		fNanoText.textBox(262, 316, 45.0f, fLabelBuffer);

		// Hold
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod1Hold->getValue() * 100.0f);
		fNanoText.textBox(262 + 48, 316, 45.0f, fLabelBuffer);

		// Decay
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod1Decay->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 2, 316, 45.0f, fLabelBuffer);

		// Sustain
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod1Sustain->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 3, 316, 45.0f, fLabelBuffer);

		// Release
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod1Release->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 4, 316, 45.0f, fLabelBuffer);
	}

	/* Modulation Envelope 2 */
	{
		// Attack
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod2Attack->getValue() * 100.0f);
		fNanoText.textBox(262, 426, 45.0f, fLabelBuffer);

		// Hold
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod2Hold->getValue() * 100.0f);
		fNanoText.textBox(262 + 48, 426, 45.0f, fLabelBuffer);

		// Decay
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod2Decay->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 2, 426, 45.0f, fLabelBuffer);

		// Sustain
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod2Sustain->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 3, 426, 45.0f, fLabelBuffer);

		// Release
		std::snprintf(fLabelBuffer, 32, "%.2f", fMod2Release->getValue() * 100.0f);
		fNanoText.textBox(262 + 48 * 4, 426, 45.0f, fLabelBuffer);
	}

	/* LFO 1 */
	{
		// Speed
		std::snprintf(fLabelBuffer, 32, "%.2f", fLfo1Speed->getValue() * 100.0f);
		fNanoText.textBox(514, 206, 45.0f, fLabelBuffer);

		// Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _wave2Str(_pf2i(fLfo1Waveform->getValue(), WAVE_MAX)));
		fNanoText.textBox(514 + 48, 206, 45.0f, fLabelBuffer);

		// Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fLfo1PulseWidth->getValue()));
		fNanoText.textBox(514 + 48 * 2, 206, 45.0f, fLabelBuffer);
	}

	/* LFO 2 */
	{
		// Speed
		std::snprintf(fLabelBuffer, 32, "%.2f", fLfo2Speed->getValue() * 100.0f);
		fNanoText.textBox(514, 316, 45.0f, fLabelBuffer);

		// Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _wave2Str(_pf2i(fLfo2Waveform->getValue(), WAVE_MAX)));
		fNanoText.textBox(514 + 48, 316, 45.0f, fLabelBuffer);

		// Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fLfo2PulseWidth->getValue()));
		fNanoText.textBox(514 + 48 * 2, 316, 45.0f, fLabelBuffer);
	}

	/* HFO */
	{
		// HFO Coarse
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2coarse(fHfoCoarse->getValue()));
		fNanoText.textBox(514, 426, 45.0f, fLabelBuffer);

		// HFO Fine
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2fine(fHfoFine->getValue()));
		fNanoText.textBox(514 + 48, 426, 45.0f, fLabelBuffer);

		// HFO Waveform
		std::snprintf(fLabelBuffer, 32, "%s", _OscWave2Str(_pf2i(fHfoWaveform->getValue(), OWAVE_MAX)));
		fNanoText.textBox(514 + 48 * 2, 426, 45.0f, fLabelBuffer);

		// HFO Pulse Width
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2pw(fHfoPulseWidth->getValue()));
		fNanoText.textBox(514 + 48 * 3, 426, 45.0f, fLabelBuffer);
	}

	/* Arp */
	{
		// Mode
		std::snprintf(fLabelBuffer, 32, "%s", _arpMode2str(_pf2i(fArpMode->getValue(), ARP_MAX + 1) - 1));
		fNanoText.textBox(718, 316, 45.0f, fLabelBuffer);

		// Speed
		std::snprintf(fLabelBuffer, 32, "%d", (int)((1.f - fArpSpeed->getValue()) * 500.f + 0.5f));
		fNanoText.textBox(718 + 48, 316, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 1 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod1Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(10, 536, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod1Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(10 + 48, 536, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod1Amount->getValue()));
		fNanoText.textBox(10 + 48 * 2, 536, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod1Multiply->getValue()));
		fNanoText.textBox(10 + 48 * 3, 536, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 2 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod2Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(214, 536, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod2Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(214 + 48, 536, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod2Amount->getValue()));
		fNanoText.textBox(214 + 48 * 2, 536, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod2Multiply->getValue()));
		fNanoText.textBox(214 + 48 * 3, 536, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 3 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod3Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(418, 536, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod3Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(418 + 48, 536, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod3Amount->getValue()));
		fNanoText.textBox(418 + 48 * 2, 536, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod3Multiply->getValue()));
		fNanoText.textBox(418 + 48 * 3, 536, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 4 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod4Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(622, 536, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod4Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(622 + 48, 536, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod4Amount->getValue()));
		fNanoText.textBox(622 + 48 * 2, 536, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod4Multiply->getValue()));
		fNanoText.textBox(622 + 48 * 3, 536, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 5 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod5Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(10, 646, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod5Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(10 + 48, 646, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod5Amount->getValue()));
		fNanoText.textBox(10 + 48 * 2, 646, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod5Multiply->getValue()));
		fNanoText.textBox(10 + 48 * 3, 646, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 6 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod6Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(214, 646, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod6Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(214 + 48, 646, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod6Amount->getValue()));
		fNanoText.textBox(214 + 48 * 2, 646, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod6Multiply->getValue()));
		fNanoText.textBox(214 + 48 * 3, 646, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 7 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod7Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(418, 646, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod7Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(418 + 48, 646, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod7Amount->getValue()));
		fNanoText.textBox(418 + 48 * 2, 646, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod7Multiply->getValue()));
		fNanoText.textBox(418 + 48 * 3, 646, 45.0f, fLabelBuffer);
	}

	/* Modulation Slot 8 */
	{
		// Source
		std::snprintf(fLabelBuffer, 32, "%s", _modSrc2str(_pf2i(fMod8Source->getValue(), MOD_SRC_MAX)));
		fNanoText.textBox(622, 646, 45.0f, fLabelBuffer);

		// Destination
		std::snprintf(fLabelBuffer, 32, "%s", _modDest2str(_pf2i(fMod8Destination->getValue(), MOD_DEST_MAX)));
		fNanoText.textBox(622 + 48, 646, 48.0f, fLabelBuffer);

		// Amount
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modAmount(fMod8Amount->getValue()));
		fNanoText.textBox(622 + 48 * 2, 646, 45.0f, fLabelBuffer);

		// Multiplicator
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2modMul(fMod8Multiply->getValue()));
		fNanoText.textBox(622 + 48 * 3, 646, 45.0f, fLabelBuffer);
	}

	/* MIDI */
	{
		// Channel
		std::snprintf(fLabelBuffer, 32, "%d", _c_val2midiChannel(fMidiChannel->getValue()));
		fNanoText.textBox(742, 426, 45.0f, fLabelBuffer);
	}

	fNanoText.endFrame();
}

void CCetoneUI::idleCallback() { }

// -----------------------------------------------------------------------

START_NAMESPACE_DISTRHO

UI* createUI()
{
	return new CCetoneUI();
}

END_NAMESPACE_DISTRHO
