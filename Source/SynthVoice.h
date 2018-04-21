/*
  ==============================================================================

    SynthVoice.h
    Created: 18 Apr 2018 10:46:12pm
    Author:  Lamtharn Hantrakul

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound)
    {
        return dynamic_cast<SynthSound*>(sound) != nullptr; // cast otherwise leave as nullptr
    }
    
    //===========================================
    void getParam (float* amp, float* attack, float* decay, float* sustain, float* release)
    {
        // dereference the pointers
        masterAmp = *amp;
        env1.setAttack(double(*attack));
        env1.setDecay(double(*decay));
        env1.setSustain(double(*sustain));
        env1.setRelease(double(*release));
    }
    
    //===========================================
    void getOscType(float* selection)
    {
        theWave = *selection; //dereferenec the selection
    }
    
    double setOscType ()
    {
        if (theWave == 0) {
            return osc1.sinewave(frequency);
        }
        if (theWave == 1) {
            return osc1.saw(frequency);
        }
        if (theWave == 2) {
            return osc1.square(frequency);
        }
        else {
            return osc1.saw(frequency);
        }
    }
    
    //===========================================
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition)
    {
        env1.trigger = 1;
        level = velocity;  // velocity is already normalized
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    
    //===========================================
    void stopNote (float velocity, bool allowTailOff)
    {
        env1.trigger = 0;
        allowTailOff = true;
        
        if (velocity == 0) {
            clearCurrentNote();
        }
        
    }
    
    //===========================================
    void pitchWheelMoved (int newPitchWheelValue)
    {
        
    }
    
    //===========================================
    void controllerMoved (int controllerNumber, int newControllerValue)
    {
        
    }
    
     //===========================================
    void renderNextBlock (AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
    {
        // ADSR level is set by `getParam`.
        // Add the waveforms to the block
        for (int sample = 0; sample < numSamples; sample++) {
            double theSound = env1.adsr(setOscType(), env1.trigger) * level * masterAmp;
            double filteredSound = filter1.lores(theSound, 14000, 0.1);
            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, startSample, theSound);
            }
            ++startSample;
        }
    }
private:
    double level;
    double frequency;
    double masterAmp;
    int theWave;  // wave type selection index
    
    maxiOsc osc1;
    maxiEnv env1;
    maxiFilter filter1;
};
