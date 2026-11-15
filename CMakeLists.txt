cmake_minimum_required (VERSION 3.20)

project (CetoneSynth
    VERSION 1.0.0
    LANGUAGES C CXX
)

# Metadata (for CPack)
set (AUTHOR "AnClark Liu")
set (INSTALL_DIRECTORY ${PROJECT_NAME})
set (ICON_PATH "${PROJECT_SOURCE_DIR}/Screenshot.png")  # TODO: Re-design icon
set (PRODUCTBUILD_IDENTIFIER "com.anclark.cetone.synth")

# Generate dynamic metadata file
configure_file (cmake/metadata.h.in ${PROJECT_BINARY_DIR}/metadata.gen.h @ONLY)

# Options for Cetone build
option (CETONE_ENABLE_POLYPHONY "Enable polyphonic features of Cetone synthesizer" ON)

option (CETONE_BUILD_JACK_STANDALONE "Build JACK standalone format" OFF)
if (CETONE_BUILD_JACK_STANDALONE)
	set (TARGET_JACK jack)
endif ()

# GitHub CI's Ubuntu runner cannot directly run lv2-ttl-generator.exe, even though Wine and wine-binfmt is installed.
# And, LV2 is not so common on Windows (only REAPER, Ardour and Harrison Mixbus support LV2 on Windows).
# So disable LV2 on Windows by default.
option (CONFIG_BUILD_LV2_ON_WINDOWS "Build LV2 on Windows (including cross-compiling)." OFF)
if (WIN32 AND CONFIG_BUILD_LV2_ON_WINDOWS)
    message (STATUS "Win32 environment and LV2 build is enabled, will build LV2 plugin")
    set (TARGET_LV2 lv2)
elseif (WIN32)
    message (STATUS "Win32 environment and LV2 build is disabled. Bypass LV2")
else ()
    message (STATUS "Non-Win32 environment, will build LV2 plugin")
    set (TARGET_LV2 lv2)
endif ()

# Explicitily specify C++ standard to prevent build error on Github CI - macOS build
set (CMAKE_CXX_STANDARD 17)

add_subdirectory (DPF)

include_directories (
    Sources/
    Sources/3rdparty/
    .
	DPF-Widgets/dpf-imgui/
    ${PROJECT_BINARY_DIR}
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
    TARGETS vst2 vst3 ${TARGET_LV2} clap ${TARGET_JACK}
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
        Sources/PresetManager.cpp
        Sources/PresetManagerHelper.cpp
        Images/CetoneArtwork.cpp
        Fonts/CetoneFonts.cpp
        DPF-Widgets/dpf-imgui/DearImGui.cpp
        Sources/Widgets/ImGui_UI.cpp
        Sources/Widgets/ImGui_UI_MessageBox.cpp
        Sources/Widgets/ImGui_UI_FileBrowser.cpp
        Sources/Widgets/ImGui_UI_PresetManager.cpp
)

# NOTE: Do not set ENABLE_POLYPHONY globally, as it only affects plugin source files.
#       This can prevent unexpected rebuild of non-Cetone sources (especially DPF sources).
if (CETONE_ENABLE_POLYPHONY)
    target_compile_definitions (${PROJECT_NAME}-dsp PRIVATE ENABLE_POLYPHONY)
    target_compile_definitions (${PROJECT_NAME}-ui PRIVATE ENABLE_POLYPHONY)
endif ()

# Include factory presets converter utility if enabled
if (CETONE_INCLUDE_FACTORY_PRESETS)
    set (FACTORY_PRESET_DIR ${PROJECT_SOURCE_DIR}/presets/factory)
    set (FACTORY_PRESET_HEADER_FILE ${PROJECT_BINARY_DIR}/factory_presets.h)

    target_compile_definitions (${PROJECT_NAME}-ui PRIVATE INCLUDE_FACTORY_PRESETS)
    target_include_directories (${PROJECT_NAME}-ui PRIVATE ${PROJECT_BINARY_DIR})  # Include directory for generated header

    # Build utility to convert .csynth preset files to C++ header with factory presets
    add_subdirectory (utils)

    # Generate factory presets header at build time
    add_custom_command (
        OUTPUT ${FACTORY_PRESET_HEADER_FILE}
        COMMAND factory_patch_converter ${FACTORY_PRESET_DIR} ${FACTORY_PRESET_HEADER_FILE}
        DEPENDS factory_patch_converter
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating factory presets header from .csynth files..."
    )
    add_custom_target (generate_factory_presets ALL
        DEPENDS ${FACTORY_PRESET_HEADER_FILE}
    )
    add_dependencies (${PROJECT_NAME}-ui generate_factory_presets)
endif ()

# CPack stuff
include (cmake/Packaging.cmake)
