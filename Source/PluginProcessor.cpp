/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

/*!
 
 IMPORTANT NOTICE
 
 I get inspired by Aaron Leese's tutorials on Youtube on "BitCrusher" :https://www.youtube.com/watch?v=1PLn8IAKEb4,
 Joshua Hodge's Youtube tutorials on "Bit Crusher" :https://www.youtube.com/channel/UCpKb02FsH4WH4X_2xhIoJ1A/about
 Daniele Filaretti's "BitCrusher Demo" project  GitHub :https://github.com/dfilaretti.
 Especially for the "NoiseGenerator.h" header file, getting the "bit depth" and "sample rate" changes parts.
 I made some alterations and improvements on them as well as adding  a LFO controlled detuner on "MySynth" header file.
 
 Also used https://en.wikipedia.org/wiki/Bitcrusher to understand the concept of "BitCrusher" better.
 
 My thanks to all these people and sources as well as our instructors for the "Oscillators.h , DelayLine.h files and base of  MySynth.h file.
 */

//==============================================================================
MidiTryAudioProcessor::MidiTryAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this,nullptr,"ParamTreeID",{
    
    //The amount of detunining appiled
    std::make_unique<juce::AudioParameterFloat>("detune","Detune(Hz)",0.0f,20.0f,2.0f),
    
    //The noise volume
    std::make_unique<juce::AudioParameterFloat>("noise","Noise",juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),2.0f),
    
    //Sample Rate and Bit Depth
    std::make_unique<juce::AudioParameterFloat>("rate", "Rate", juce::NormalisableRange<float>(0.0f, 50.0f, 1.0f), 0.0f),
    std::make_unique<juce::AudioParameterFloat>("bits", "Bits", juce::NormalisableRange<float>(1.0f, 32.0f, 1.0f), 24.0f),
    
    //To switch the detune on and off
    std::make_unique<juce::AudioParameterFloat>("detuneOn","Detune On",juce::NormalisableRange<float>(0.0f, 1.1f, 1.0f),0.1f),
    
    //The amount of "Detuned" signal on the final mix
    std::make_unique<juce::AudioParameterFloat>("detuneMix","Detune Mix",0.0f, 1.0f ,0.7f),
    
    //The speed of LFO that controls the detuning
    std::make_unique<juce::AudioParameterFloat>("lfoSpeed","LFO Rate",0.0f, 400.0f ,10.0f),
    
})
{
    //Assigning each atomic floats with it's respected ValueTree Parameter
    detuneParam = parameters.getRawParameterValue("detune");
    
    
    noiseParam = parameters.getRawParameterValue("noise");
    
    rateParam = parameters.getRawParameterValue("rate");
    bitsParam = parameters.getRawParameterValue("bits");
    
    rateOfLFO = parameters.getRawParameterValue("lfoSpeed");
    detuneOn = parameters.getRawParameterValue("detuneOn");
    
    detuneMix = parameters.getRawParameterValue("detuneMix");
    
    //Here we use the voiceCount to hold that much of sounds at once to process through
    for ( int voices = 0; voices < voiceCount; voices ++ )
    {
        synth.addVoice(new MySynthVoice);

    }
    synth.addSound((new MySynthSound));
}

MidiTryAudioProcessor::~MidiTryAudioProcessor()
{
}

//==============================================================================
const juce::String MidiTryAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiTryAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiTryAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiTryAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiTryAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiTryAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiTryAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiTryAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiTryAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiTryAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiTryAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Setting the sample rate of the synth
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    //Sending the sampleRate to the "MySyth.h" header as well by dynamic casting
    for(int i=0; i < voiceCount; i ++)
    {
        MySynthVoice* v= dynamic_cast<MySynthVoice*>(synth.getVoice(i));
        v -> init(sampleRate);
    }
    
}

void MidiTryAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiTryAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MidiTryAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    //So that no abrubt sounds or pops
    buffer.clear();
    

    //Noise parameterss and variables
    float noiseAmt = -120 + 120 * (*noiseParam / 100); // dB
    noiseAmt = juce::jlimit<float>(-120, 0, noiseAmt);       // limit (?)
    noiseAmt = juce::Decibels::decibelsToGain(noiseAmt);     // dB to gain
    
    //Estabishing the factors and variables for resampling and bit crushing
    float bitDepth = *bitsParam;
    auto rateDivide = static_cast<int> (*rateParam);
    int numSamples = buffer.getNumSamples();
    
    //MAYBE ADD GAIN VOLUME AS WELL FOR FUTURE USE
    
    //This is for security reasons, making sure that if the host changes the buffer sizes without calling prepareToPlay,
    //it would not crash
    if (noiseBuffer.getNumSamples() != numSamples)
    {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clears
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears
    }
    
    //Copy Of Current Buffer to Populate With Resampled Array
    currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
    
    //For Stereo
    if (buffer.getNumChannels() > 1)
        currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);
    
    noiseBuffer.clear();
    
    //Building Noise
    juce::Array<float> noise;

    //Chose to Use White Noise,Could Be Simple Noise As Well
    noise = noiseMaker.getWhiteNoise(numSamples);
    
    // Range Bound fo Noise
    noiseAmt = juce::jlimit<float>(0, 1, noiseAmt);
    
    
    
    // Controlling the Noise Volume
    juce::FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);
    
    //Adding The Noise to Noise Buffer In Stereo
    juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
    juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples);
    
    //Dynamic Casting the Variables to Respected Functions in "MySynth.h" Header File
    for(int i=0; i < voiceCount; i ++)
    {
            
        MySynthVoice* v= dynamic_cast<MySynthVoice*>(synth.getVoice(i));
        
        v -> setDetune(*detuneParam);
        v->setLFOFreq(*rateOfLFO);
        v -> openDetune(*detuneOn);
        v-> setDetuneMix(*detuneMix);
      
    }

    //Adding Our Midi Samples from our Synth to currentOutputBuffer
    synth.renderNextBlock(currentOutputBuffer, midiMessages, 0, numSamples);
    
    //Multiply the Noise Buffer with Current Buffer (This can be ignored if wished) I find it better to both add and multiply
    juce::FloatVectorOperations::multiply(noiseBuffer.getWritePointer(0), currentOutputBuffer.getWritePointer(0), numSamples);
    juce::FloatVectorOperations::multiply(noiseBuffer.getWritePointer(1), currentOutputBuffer.getWritePointer(1), numSamples);

    //Then Add Noise As Well
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
    currentOutputBuffer.addFrom(1, 0, noiseBuffer.getReadPointer(1), numSamples);
    
    //Here the resampling and bit crushing occurs, this is a very generic procedure for bitcrushing as my research shows
    for (int chan = 0; chan < currentOutputBuffer.getNumChannels(); chan++)
    {
        float* data = currentOutputBuffer.getWritePointer(chan);

        for (int i = 0; i < numSamples; i++)
        {
           
            // REDUCE BIT DEPTH
            float totalQLevels = powf(2 , bitDepth);
            float val = data[i];
            float remainder = fmodf(val, 1 / totalQLevels);

            // Quantize ...
            data[i] = val - remainder;

            if (rateDivide > 1)
            {
                if (i%rateDivide != 0)
                    data[i] = data[i - i % rateDivide];
            }
        }
    }

    //Copy the currentOutputbuffer to our final main buffer
    buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);
    buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);

    
    //This can be used for future use as well, so let this stay here
//    juce::FloatVectorOperations::add(buffer.getWritePointer(0), currentOutputBuffer.getReadPointer(0), numSamples);
//    juce::FloatVectorOperations::add(buffer.getWritePointer(1), currentOutputBuffer.getReadPointer(1), numSamples);
}

//==============================================================================
bool MidiTryAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiTryAudioProcessor::createEditor()
{
    return new MidiTryAudioProcessorEditor (*this);
}

//==============================================================================
void MidiTryAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiTryAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiTryAudioProcessor();
}
