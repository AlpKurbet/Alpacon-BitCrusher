/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//Needed to add "processor(p) to use it as an object in which I can reach parameters

MidiTryAudioProcessorEditor::MidiTryAudioProcessorEditor (MidiTryAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p) , processor(p)

{
    /*!
     
     IMPORTANT NOTICE
     
     I get inspired by Aaron Leese's tutorials on Youtube on "UI" :https://www.youtube.com/watch?v=ggJRCQK27B8
     Landon Viator's Youtube tutorials on "Bit Crusher" :https://www.youtube.com/watch?v=iVDlGFhbqw4&list=PL7Ysno2nO-6JVw5dTSYCmxdvxFS42kuve&index=11&ab_channel=DrBruisin

     My thanks to these people .
     */
    
    
    
    
    /*!
        Each slider goes  visible on the UI first. Then the sceond line indicates what type of a slider it is (linear, vertical etc.)
        Third line gives a text box below it to show the results. Fourth line gives a range that the user can slide through.
                
        Then there come the label lines. Again, first line makes it visible on UI. The second label is for the actual string. In this case,
        I don't allow notifications from the user. The third line attaches the label to its related slider. Fourth line is for allignent according to
        object it is attached to.
     
        The  last line links the slider with the related parameter on "ValueTree" in accordance with the type of the object.
     */
    
    
    // BIT DEPTH SLIDER
    addAndMakeVisible(bitSlider);
    bitSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    bitSlider.setRange(0.0,32.0,0.1);
    
    addAndMakeVisible(bitLabeL);
    bitLabeL.setText("Bit Depth", juce::dontSendNotification);
    bitLabeL.attachToComponent( &bitSlider,false);
    bitLabeL.setJustificationType(juce::Justification::centred);
    
    bitSlide = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"bits",bitSlider);
    
    //SAMPLE RATE SLIDER
    addAndMakeVisible(sampleSlider);
    sampleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sampleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    sampleSlider.setRange(0.0,50.0,0.1);
    
    addAndMakeVisible(sampleLabel);
    sampleLabel.setText("Sample Size", juce::dontSendNotification);
    sampleLabel.attachToComponent( &sampleSlider,false);
    sampleLabel.setJustificationType(juce::Justification::centred);
    
    sampleSlide = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"rate",sampleSlider);
    
    //DETUNE OPENER SLIDER
    addAndMakeVisible(detuneOpener);
    detuneOpener.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    detuneOpener.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    detuneOpener.setRange(0.0,1.1,1.0);
    
    addAndMakeVisible(detuneLabel);
    detuneLabel.setText("Detune Open", juce::dontSendNotification);
    detuneLabel.attachToComponent( &detuneOpener,false);
    detuneLabel.setJustificationType(juce::Justification::centredBottom);
    
    detuneOpen = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"detuneOn",detuneOpener);
    
    //DETUNE AMOUNT
    addAndMakeVisible(DetuneAmounter);
    DetuneAmounter.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DetuneAmounter.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    DetuneAmounter.setRange(0.0,20,2.0);
    
    addAndMakeVisible(detuneAmountLabel);
    detuneAmountLabel.setText("Detune Amount", juce::dontSendNotification);
    detuneAmountLabel.attachToComponent( &DetuneAmounter,false);
    detuneAmountLabel.setJustificationType(juce::Justification::centred);
    
    detuneAmount = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"detune",DetuneAmounter);
    
    //DETUNE MIX
    addAndMakeVisible(DetuneMixer);
    DetuneMixer.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DetuneMixer.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    DetuneMixer.setRange(0.0,100.0,2.0);
    
    addAndMakeVisible(detuneLabel);
    detuneLabel.setText("Detune Mix", juce::dontSendNotification);
    detuneLabel.attachToComponent( &DetuneMixer,false);
    detuneLabel.setJustificationType(juce::Justification::centred);
    
    mixOfDetune = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"detuneMix",DetuneMixer);
    
    //LFO SPEED CONTROL
    addAndMakeVisible(LFOController);
    LFOController.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    LFOController.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    LFOController.setRange(0.0,400.0,10.0);
    
    addAndMakeVisible(LFOLabel);
    LFOLabel.setText("LFO Rate", juce::dontSendNotification);
    LFOLabel.attachToComponent( &LFOController,false);
    LFOLabel.setJustificationType(juce::Justification::centred);
    
    LFOControl = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"lfoSpeed",LFOController);
    
    //NOISE CONTROL
    addAndMakeVisible(NoiseAmounter);
    NoiseAmounter.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    NoiseAmounter.setTextBoxStyle(juce::Slider::TextBoxBelow, true,  40, 20);
    NoiseAmounter.setRange(0.0,100.0,2.0);
    
    addAndMakeVisible(LFOLabel);
    noiseLabel.setText("Noise", juce::dontSendNotification);
    noiseLabel.attachToComponent( &NoiseAmounter,false);
    noiseLabel.setJustificationType(juce::Justification::centred);
    
    noiseAmount = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"noise",NoiseAmounter);
    
    //GENERAL SETTINGS
    setResizable(true, true);                                   //Allows resizing the UI
    getConstrainer()->setFixedAspectRatio(2.0);                 //Fixes the aspect ratio so that it can change accordingly when resized
    setSize (1000, 500);                                        //Setting the defaut size
}

MidiTryAudioProcessorEditor::~MidiTryAudioProcessorEditor()
{
}

//==============================================================================
void MidiTryAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Adding the background image from the "Assets" folder. Image is from : https://hdqwalls.com/pixel-art-wallpaper
    //I do not own this iamge, it is free to download and I am not using it for commercial purposes. The author is
    //anonymous so I cannot site it.
    background = juce::ImageCache::getFromMemory(BinaryData::pluginBackground_png,BinaryData::pluginBackground_pngSize);

    //Setting the UI parameters for the background and title
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    g.setColour (juce::Colours::darkorange);                                                                   //Setting title colour
    g.setFont(juce::Font::bold);                                                                               //Setting the font
    g.setFont (35.0f);                                                                                         //Setting the font size
    g.drawFittedText ("Alpacon BitCrusher", getLocalBounds(), juce::Justification::centredBottom,2);           //Write & Allign the text
}
void MidiTryAudioProcessorEditor::resized()
{
    //Parameters to set the first slider
    auto leftMargin = getWidth() * 0.02;
    auto topMargin = getHeight() * 0.04;
    
    //Setting the positions of the sliders with respect to each other
    //The function .setBounds() should get the x,y components as arguments; then get the size information
    bitSlider.setBounds(leftMargin,topMargin,256,256);
    sampleSlider.setBounds(bitSlider.getX() + bitSlider.getWidth() + leftMargin, topMargin, 256, 256);
    
    LFOController.setBounds(sampleSlider.getX() + sampleSlider.getWidth() + leftMargin, topMargin, 150, 150);
    
    DetuneAmounter.setBounds(LFOController.getX() + LFOController.getWidth() + leftMargin, topMargin, 150, 150);
    detuneOpener.setBounds( bitSlider.getX() + 2 * leftMargin , 18 *topMargin , 70, 70);
    DetuneMixer.setBounds( LFOController.getX() , 14 *topMargin , 150, 150);
    
    NoiseAmounter.setBounds( DetuneAmounter.getX() , 14 *topMargin , 150, 150);
}
