/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveNetWaveTableAudioProcessorEditor::WaveNetWaveTableAudioProcessorEditor (WaveNetWaveTableAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // AttackSlider
    attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    attackSlider.setRange(0.1f,5000.0f); // 0.1ms - 5000ms attack time
    attackSlider.setValue(0.1f);
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.getNumDecimalPlacesToDisplay();
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40.0, 20.0);
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    
    attackTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "attack", attackSlider);  // the correct way to interface the slider in editor with processor.
    
    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, false);
    
    // ReleaseSlider
    releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    releaseSlider.setRange(0.1f,5000.0f); // 0.1ms - 5000ms release time
    releaseSlider.setValue(0.1f);
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.getNumDecimalPlacesToDisplay();
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40.0, 20.0);
    releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);
    
    releaseTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "release", releaseSlider);  // the correct way to interface the slider in editor with processor.
    
    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);
    
    // AmpSlider
    ampSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    ampSlider.setRange(0.0f,1.0f); // 0.0 - 1.0 master amp
    ampSlider.setValue(0.5f);
    ampSlider.getNumDecimalPlacesToDisplay();
    ampSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40.0, 20.0);
    ampSlider.addListener(this);
    addAndMakeVisible(ampSlider);
    
    ampTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "amp", ampSlider);  // the correct way to interface the slider in editor with processor.
    
    addAndMakeVisible(ampLabel);
    releaseLabel.setText("Amp", dontSendNotification);
    releaseLabel.attachToComponent(&ampSlider, false);
    
    // Change look and feel of all sliders
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::deeppink);
    getLookAndFeel().setColour(Slider::trackColourId, Colours::lightskyblue);
    
    // Ppm Meter
    addAndMakeVisible(Meter);
    startTimer(10);  // start the timer to tick every 10ms
}

WaveNetWaveTableAudioProcessorEditor::~WaveNetWaveTableAudioProcessorEditor()
{
//    delete attackTree;
//    delete releaseTree;
}

//==============================================================================
void WaveNetWaveTableAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("WaveNet WaveTable Synth by Hanoi Hantrakul x Richard Yang", getLocalBounds(), Justification::bottom, 1);
}

void WaveNetWaveTableAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto area = getLocalBounds();
    auto headerHeight = 20;
    auto footerHeight = 20;
    
    area.removeFromTop    (headerHeight);
    area.removeFromBottom (footerHeight);
    
    // >>> Sliders
    auto sliderWidth = 70;
    attackSlider.setBounds(area.removeFromLeft(sliderWidth));
    releaseSlider.setBounds(area.removeFromLeft(sliderWidth));
    ampSlider.setBounds(area.removeFromLeft(sliderWidth));
    
    // >>> Peak Meter
    auto peakMeterWidth = 30;
    Meter.setBounds(area.removeFromLeft(peakMeterWidth));
    
}

//==============================================================================
void WaveNetWaveTableAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &attackSlider) {
        processor.attackTime = attackSlider.getValue();
    }
    else if (slider == &releaseSlider) {
        processor.releaseTime = releaseSlider.getValue();
    }
    else if (slider == &ampSlider) {
        processor.ampValue = ampSlider.getValue();
    }
    
}

//==============================================================================
void WaveNetWaveTableAudioProcessorEditor::timerCallback()
{
    //DBG(processor.getVppm());
    Meter.setValue(processor.getVppm());
}
