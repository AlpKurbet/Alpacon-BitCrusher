/*
  ==============================================================================

    DelayLine.h
    Created: 2 Nov 2022 11:46:13am
    Author:  Tom Mudd

  ==============================================================================
*/

#pragma once


class DelayLine
{
    
public:
    
    /**
     IMPORTANT. We didn't cover this in the class, but we need
     to make sure we clean up the dynamically allocated array
     when our object is destroyed!
     */
    ~DelayLine()
    {
        if (data != nullptr)
            delete[] data;
    }
    
    /// set the maximum size of the delay line
    void setSizeInSamples(int newSize)
    {
        size = newSize;             // store the new size
        
        // free up existing data
        if (data != nullptr)
            delete[] data;
        
        data = new float[size];     // initialise our array
        
        // set all values to zero
        for (int i=0; i<size; i++)
        {
            data[i] = 0.0;
        }
    }
    
    
    /// set the delay time in samples
    void setDelayTimeInSamples(float newDelayTime)
    {
        delayTime = newDelayTime;
        
        // set the read index to be behind the write index
        readIndex = writeIndex - delayTime;
        
        if (readIndex < 0)
            readIndex = readIndex + size;
        
    }
    
    
    
    /// call every sample to:
    ///--  store the new sample
    ///-- advance read index and write index
    ///-- return the value at the read index
    float process(float inputSample)
    {
        // get value of data at green read index
        float outputSample = linearInterpolation();// data[readIndex];
        
        // store value at red write index
        data[writeIndex] = inputSample  +  outputSample * feedback;
        
    
        // advance the read and write indexes
        // wrap them back into the correct range
        readIndex += 1;
        if (readIndex >= size)
            readIndex -= size;
        
        writeIndex += 1;
        if (writeIndex >= size)
            writeIndex -= size;
        
        
        return outputSample;
        
    }
    
    
    float linearInterpolation()
    {
        // readIndex = 2.3;
        
        // get the two indexes around our read index
        int indexA = int(readIndex);        // e.g. 2
        int indexB = indexA + 1;            // e.g. 3
        
        // wrap indexB if necessary
        if (indexB > size)
            indexB -= size;
        
        // get values at data indexes
        float valA = data[indexA];          // e.g. data[2]
        float valB = data[indexB];          // e.g. data[3]
        
        
        // calculate remainder
        float remainder = readIndex - indexA;   // e.g. 2.3 - 2 = 0.3
        
        // work out interpolated sample between two indexes
        float interpolatedSample = (1-remainder) * valA   +   remainder * valB;
        
        
        return interpolatedSample;
    }
    
    
    void setFeedback(float _feedback)
    {
        feedback = _feedback;
        if (feedback < 0)
            feedback = 0;
        if (feedback > 1.0)
            feedback = 1.0;
    }
    
    
    
    
private:
    
    float readIndex = 0;  // read position as an index
    int writeIndex = 0; // write position as an index
    
    int size;           // maximum possible delay time
    
    float* data = nullptr;        // store input audio data
    
    int delayTime;      // delay time in samples
    
    
    float feedback = 0;     // must be between 0 and 1!!!
    
};
