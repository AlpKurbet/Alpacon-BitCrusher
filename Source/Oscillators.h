//
//  Oscillators.h
//  Week4MoreClasses
//
//  Created by tmudd on 03/02/2020.
//  Copyright Â© 2020 tmudd. All rights reserved.
//

#ifndef Oscillators_h
#define Oscillators_h

/**
 Base oscillator class
 
 outputs the phase directly in the range: 0-1
 */
class Phasor
{
public:
    
    /// empty destructor function
    virtual ~Phasor() {};
    
    // Our parent oscillator class does the key things required for most oscillators:
    // -- handles phase
    // -- handles setters and getters for frequency and samplerate
    
    /// update the phase and output the next sample from the oscillator
    float process()
    {
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return output(phase);
    }
    
    /// this function is the one that we will replace in the classes that inherit from Phasor
    virtual float output(float p)
    {
        return p;
    }
    
    /**
     set the sample rate in Hz (e.g. 44100). Must be set BEFORE calling setFrequency()!!!
     
     @param sr sample rate in Hz
     */
    void setSampleRate(float sr)
    {
        sampleRate = sr;
    }
    
    /**
     sets the oscillator frequency in Hz (e.g. 440). Must be set AFTER calling setSampleRate()!!!
     
     @param freq oscillator frequency in Hz
     */
    void setFrequency(float freq)
    {
        frequency = freq;
        phaseDelta = frequency / sampleRate;
    }

    
    
    /// for phase modulation:
    void setPhaseOffset(float _phaseOffset)
    {
        phaseOffset = _phaseOffset;
    }
    
private:
    float frequency;
    float sampleRate;
    float phase = 0.0f;
    float phaseDelta;
    
    float phaseOffset = 0.0;        // for phase modulation
};

/**
 Triangle oscillator built on Phasor base class
 
 Note that the output is quiet by default: ±0.25
 */
class TriOsc : public Phasor
{
    // redefine (override) the output function so that we can return a different function of the phase (p)
    float output(float p) override
    {
        return fabsf(p - 0.5f) - 0.5f;
    }
};



/**
 Sine Oscillator built on Phasor base class
 */
class SinOsc : public Phasor
{
    float output(float p) override
    {
        return std::sin(p * 2.0 * 3.14159);
    }
};

/**
 Squarewave Oscillator built on Phasor base class
 
 Includes setPulseWidth to change the waveform shape
 */
class SquareOsc : public Phasor
{
public:
    float output(float p) override
    {
        float outVal = 0.5;
        if (p > pulseWidth)
            outVal = -0.5;
        return outVal;
    }
    
    /// set square wave pulse width (0-1))
    void setPulseWidth(float pw)
    {
        pulseWidth = pw;
    }
private:
    float pulseWidth = 0.5f;
};




#endif /* Oscillators_h */
