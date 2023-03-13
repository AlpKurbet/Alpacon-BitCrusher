/*
  ==============================================================================

    NoiseGenerator.h
    Created: 13 Dec 2022 1:56:58am

  ==============================================================================
*/

#pragma once



/**
 Having the noise samples as arrays are very handy since at the end, we work with two different buffers than the original output bufffer.
 We do vector calculations (.add, .multiply etc.) with buffers so we can add an array of noise to them.
 
 This class produces two types of  noise: white and simple. The WhiteNoiseGenerator uses Muler Generation Methods. For more details refer to https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
 
*/

class NoiseGenerators
{
public:
    //A static array funtion of simpleNoise
    static juce::Array<float> getSimpleNoise(int numSamples)
    {
        //Add a random number to the array
        juce::Random r = juce::Random::getSystemRandom();
        juce::Array<float> randomSample;

        for (int s = 0; s < numSamples; s++)
        {
            //Scale the random numbers
            randomSample.add((r.nextFloat() - 0.5f) * 2.0f);
        }

        //Return the array
        return randomSample;

    }
    
    //A static array function of WhiteNoise
    static juce::Array<float> getWhiteNoise(int numSamples)
    {
        juce::Array<float> randomSample;

        //Variables fot the Muller Generation Method
        float z0 = 0;
        float z1 = 0;
        bool generate = false;

        float mu = 0; // center (0)
        float sigma = 1; // spread -1 <-> 1

        float output = 0;
        float u1 = 0;
        float u2 = 0;

        juce::Random r = juce::Random::getSystemRandom();
        r.setSeed(juce::Time::getCurrentTime().getMilliseconds());
        const float epsilon = std::numeric_limits<float>::min();

        for (int s = 0; s < numSamples; s++)
        {

            // GENERATE
            // using box muller method
            generate = !generate;

            if (!generate)
                output = z1 * sigma + mu;
            else
            {
                do
                {
                    u1 = r.nextFloat();
                    u2 = r.nextFloat();
                } while (u1 <= epsilon);

                z0 = sqrtf(-2.f * logf(u1)) * cosf(2.f * juce::float_Pi * u2);
                z1 = sqrtf(-2.f * logf(u1)) * sinf(2.f * juce::float_Pi * u2);

                output = z0 * sigma + mu;
            }

            // NAN check ...
            jassert(output == output);
            jassert(output > -50 && output < 50);

            //
            randomSample.add(output);

        }
        return randomSample;
    }

    
private:
    
};

