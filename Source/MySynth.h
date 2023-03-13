/*
  ==============================================================================

    MySynth.h
    Created: 7 Mar 2020 4:27:57pm
    Author:  Tom Mudd

  ==============================================================================
*/

#pragma once

#include "Oscillators.h"
#include  "DelayLine.h"




// ===========================
// ===========================
// SOUND
class MySynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// Synthesiser Voice - your synth code goes in here

/*!
 @class MySynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MySynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 
    Using "Synth" header file from Learn page as a basis to my "MySynth" file. I comment about every modification and extention I have.
 */
class MySynthVoice : public juce::SynthesiserVoice
{
public:
    MySynthVoice() {}
    
    //Set the Sample Rate & init the parameters for the ADSR
    void init(float sampleRate)
    {
        //sampleRate
        osc.setSampleRate(sampleRate);
        detuneOsc.setSampleRate(sampleRate);
        LFO.setSampleRate(sampleRate);
        env.setSampleRate(sampleRate);
        
        //ADSR
        /*
         I didn't want a controllable ADSR since it is can be a noisy with the bitcrusher Synth. Rather, I wanted to have a decent
         attack and decay parameters for smooth and unpoped sound.This is just to smoothen the sound.
         */
        juce::ADSR::Parameters envParams;
        envParams.attack = 0.1;
        envParams.decay = 0.2;
        envParams.sustain = 0.1;
        envParams.release = 0.1;
        
        env.setParameters(envParams);
    }
    
    //This function is to switch the bool value @detuneIsOn to "true" or "false" ot turn on or off the detuning
    void openDetune(float detuneOn)
    {
        if(detuneOn > 0.5f)
        {
            detuneIsOn = true;
        }
        else
        {
            detuneIsOn = false;
        }
    }
    
    //This function sets how much the second signal will be detuned from the original signal's freq
    void setDetune(float detuneIn)
    {
        detuneAmount = detuneIn;
    }
    
    //To determine how much "detuned" sound the master sound will have with respect to the original "tuned" sound
    void setDetuneMix(float mix)
    {
        mixForDetune = mix;
    }

    //Control the speed of the LFO which controls the detunining
    void setLFOFreq(float freq)
    {
        LFO.setFrequency(freq);
    }
    
    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        playing = true;
        ending = false;
        freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        osc.setFrequency(freq);
        
        env.reset();
        env.noteOn();
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        env.noteOff();
        ending = true;
//        clearCurrentNote();
//        playing = false;

    }
    
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (playing) // check to see if this voice should be playing
        {
            
            /*Set the detune osc's frequency. The frequency depends on the LFO's output. It is scaled with +1 and then
            scaled with detuneAmount multiplied to have a good but not wierd range of osciallation that is still a detuned
             sounds rather than another sound entirely different
            */
            detuneOsc.setFrequency(freq - ( (LFO.process() + 1.0f) * detuneAmount));
            
            // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
            for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
            {
                
                // The ADSR Envelope values
                float envVal = env.getNextSample();
                
                //The original "tuned" sound
                float basicSample = osc.process();

                //"Detuned" sound mixed with the original "tuned" sound. @mixForDetuen parameter control the mix
                float detuneSample = (((osc.process())* (1.0f - mixForDetune) ) + ((detuneOsc.process()) * mixForDetune ));
                
                
                // for each channel, write the currentSample float to the output
                for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
                {
                
                    //If the user selects "detune" option as 1 through UI
                    if(detuneIsOn)
                    {
                        //add a "detuned" + "tuned" sample modulated by ADSR Envelope and channel it thorugh the outputBuffer with
                        //appropriate sampleIndex
                        outputBuffer.addSample (chan, sampleIndex, detuneSample * envVal);
                    }

                    //If "detune" is not selected
                    else
                    {
                        //add "tuned" sample modulated by ADSR Envelope and channel it thorugh the outputBuffer with
                        //appropriate sampleIndex
                        outputBuffer.addSample (chan, sampleIndex, basicSample * envVal);
                    }
                        
                    
                }
                if(ending)
                {
                    //To stop the sound when the enveloped sound is finished. ATTENTION: Don't write envVal = 0 as it could never reach
                    //zero sometimes
                    if(envVal < 0.001f)
                    {
                        clearCurrentNote();
                        playing = false;
                    }
                }
                
            }
        }
    }
    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of MySynthSound
     */
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<MySynthSound*> (sound) != nullptr;
    }
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;
    bool ending = false;

    
    // Bool to control the Detune on/off
    bool detuneIsOn = false;

    //To hold "Hertz" values from "MIDI" values
    float freq;
    
    //Necessary Oscillators with Specific Duties
    TriOsc osc;
    SinOsc detuneOsc;
    SinOsc LFO;
    
    //The parameters for Detuning
    float detuneAmount = 2.0f;
    float mixForDetune;
    
    //ADSR
    juce::ADSR env;
    

};
