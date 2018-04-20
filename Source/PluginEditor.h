/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"

//==============================================================================
/**
*/
class WaveNetWaveTableAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener,  public Timer
{
public:
    WaveNetWaveTableAudioProcessorEditor (WaveNetWaveTableAudioProcessor&);
    ~WaveNetWaveTableAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    
    void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaveNetWaveTableAudioProcessor& processor;

    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> attackTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> releaseTree;
    
    Slider attackSlider;
    Slider releaseSlider;
    
    Label attackLabel;
    Label releaseLabel;
    
    MeterComponent Meter;
    
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveNetWaveTableAudioProcessorEditor)
};