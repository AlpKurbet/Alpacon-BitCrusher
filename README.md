# Alpacon BitCrusher 🎛️  
*A Plugin / DSP Effect for lo-fi distortion via bit-depth and sample rate reduction*

---

## 🎯 Overview

**Alpacon BitCrusher** is an audio plugin built using **JUCE / C++**, designed to reduce bit depth and sample rate to generate gritty, lo-fi distortion effects. It’s a creative DSP tool for musicians, sound designers, and developers who want to add saturated, artifact-rich character to audio.

---

## 🧠 DSP Theory & Design

### Bit Crushing Explained
- **Bit depth reduction**: By quantizing audio samples to fewer bits (e.g. 8 bits, 4 bits), you introduce quantization noise, which is perceived as distortion / “crush.”  
- **Sample rate reduction / downsampling**: By holding sample values over multiple output samples, you simulate a lower sampling rate, introducing aliasing / warble artifacts.

### Architecture & Signal Flow

- **Pre-filter / smoothing**: Helps reduce harsh aliasing when downsampling aggressively.  
- **Interpolation / subdivision**: Some modes may allow interpolation between crushed values to soften transitions.  
- **Dry/Wet mixing**: Controls how much of the processed (crushed) signal vs. original is output.  
- **Gain compensation / level matching**: Because crushing often reduces amplitude, a post-gain node recovers perceived loudness.

### Plugin Parameters
- `bitDepth`: e.g. 1 to 16 bits  
- `sampleRateReductionFactor`: e.g. 1x (no reduction) up to 8x or 16x slowdown  
- `mix`: Dry / Wet mix  
- `gain`: Output gain compensation  
- `filterMode`: None / Low-pass / Smoother / Anti-alias  

---

## 🏗 Architecture & Code Organization
```├── Source/
│   ├── PluginProcessor.cpp / .h   # Core audio processing
│   ├── PluginEditor.cpp / .h      # GUI / parameter bindings
│   ├── DSP/                        # DSP helper classes & modules
│   │   ├── BitCrusherEngine.h/.cpp
│   │   ├── SampleHoldModule.h/.cpp
│   │   └── FilterModule.h/.cpp
│   ├── Utils/                      # Utility functions (clamp, interpolation, etc)
│   └── Common/                     # Shared headers & constants
└── Build / Projucer / CMake files
```
---

## 🧪 Building & Usage

### Requirements
- JUCE (7.0+)  
- C++17  
- A supported DAW / host (VST3, AU)  
- CMake or Projucer project files

### Build Instructions (example using CMake)
```bash
cd Alpacon-BitCrusher
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DJUCE_PATH=/path/to/JUCE ../
cmake --build . --config Release
