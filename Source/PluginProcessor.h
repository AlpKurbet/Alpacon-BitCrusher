/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MySynth.h"
#include "NoiseGenerator.h"

//==============================================================================
/**
*/
class MidiTryAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MidiTryAudioProcessor();
    ~MidiTryAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState parameters;
private:

    
    //Two new buffers to hold the generated noise and resampled and bit depthed samples
    juce::AudioSampleBuffer noiseBuffer, currentOutputBuffer;

    // Atomic floats for ValueTree parameters
    std::atomic<float>* detuneParam;
    
    std::atomic<float>* detuneMix;
    std::atomic<float>* rateOfLFO;

    std::atomic<float>* detuneOn;
    
    std::atomic<float>* noiseParam;
    std::atomic<float>* rateParam;
    std::atomic<float>* bitsParam;
    
    
    //Calling JUCE's synth class
    juce::Synthesiser synth;
    
    
    //Calling Noise Class
    NoiseGenerators noiseMaker;
    
    //For the polyphony and how many sounds can be processed at once on the output buffer
    int voiceCount = 20;
   
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTryAudioProcessor)
};
