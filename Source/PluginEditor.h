/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiTryAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidiTryAudioProcessorEditor (MidiTryAudioProcessor&);
    ~MidiTryAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
   
    //The background image is from : https://hdqwalls.com/pixel-art-wallpaper
    juce::Image background;
    
    //Defining The Audio Processor
    MidiTryAudioProcessor& audioProcessor;
    
    //The Sliders For Related Parameters
    juce::Slider bitSlider;
    juce::Slider sampleSlider;
    juce::Slider LFOController;
    juce::Slider NoiseAmounter;
    
    /* NOTE: JUCE toggle button works a bit wierd. The ParamTree and the button does not communicate via "bool" messages,
     but rather with floats. So I used a slider with range of (0,1) with normalization of 1 so that it only can have 0 or 1 as
     a value. This is then goes to the  MySynth" file's "openDetune" function.
     */
    juce::Slider detuneOpener;
    juce::Slider DetuneAmounter;
    juce::Slider DetuneMixer;
    
    //The Labels To Attach to the Sliders
    juce::Label bitLabeL;
    juce::Label sampleLabel;
    juce::Label detuneLabel;
    juce::Label LFOLabel;
    juce::Label detuneAmountLabel;
    juce::Label noiseLabel;
    juce::Label detuneMixLabel;
    
    //Unique Pointers to Point Values at The ValueTree for Sliders
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitSlide;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sampleSlide;
   
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LFOControl;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneOpen;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneAmount;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixOfDetune;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseAmount;
    
    //Defining The Processor
    //Need this line to use it as an object to reach TreeValue parameters
    MidiTryAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTryAudioProcessorEditor)
};
