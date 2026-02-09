cmake_minimum_required (VERSION 3.20)

project (cetone_synth
    VERSION 1.0.0
    LANGUAGES C CXX
)

# Options for Cetone build
option (CETONE_ENABLE_POLYPHONY "Enable polyphonic features of Cetone synthesizer" ON)

# Explicitily specify C++ standard to prevent build error on Github CI - macOS build
set (CMAKE_CXX_STANDARD 17)

add_subdirectory (DPF)

include_directories (
    Sources/
    .
)

set (SRC_BACKEND
    Sources/Filter303.cpp
    Sources/Filter8580.cpp
    Sources/FilterButterworth24db.cpp
    Sources/FilterCh12db.cpp
    Sources/FilterDirty.cpp
    Sources/FilterMoog.cpp
    Sources/FilterMoog2.cpp
    Sources/GlobalFunctions.cpp
    Sources/MidiStack.cpp
    Sources/SynthEnvelope.cpp
    Sources/SynthLfo.cpp
    Sources/SynthOscillator.cpp
)

if (CETONE_ENABLE_POLYPHONY)
    message (STATUS "Cetone build: Polyphony mode")
    set (SRC_POLYPHONY
        Sources/CetoneSynthVoice.cpp
        Sources/CetoneSynthPolyphony.cpp
    )
else ()
    message (STATUS "Cetone build: Monophonic mode (original Cetone behavior)")
endif ()

dpf_add_plugin (${PROJECT_NAME}
    TARGETS vst2 vst3 lv2 clap jack
    FILES_DSP
        Sources/CetoneSynth.cpp
        Sources/CetoneSynthDPF.cpp
        Sources/CetoneSynthMain.cpp
        Sources/CetoneSynthPlugin.cpp
        ${SRC_POLYPHONY}
        ${SRC_BACKEND}
    FILES_UI
        Sources/CetoneUI.cpp
        Sources/CetoneUIHelper.cpp
        Images/CetoneArtwork.cpp
        Fonts/CetoneFonts.cpp
)

# NOTE: Do not set ENABLE_POLYPHONY globally, as it only affects plugin source files.
#       This can prevent unexpected rebuild of non-Cetone sources (especially DPF sources).
if (CETONE_ENABLE_POLYPHONY)
    target_compile_definitions (${PROJECT_NAME}-dsp PRIVATE ENABLE_POLYPHONY)
    target_compile_definitions (${PROJECT_NAME}-ui PRIVATE ENABLE_POLYPHONY)
endif ()
