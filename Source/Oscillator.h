/*
  ==============================================================================

    Oscillator.h
    Created: 21 Apr 2018 10:21:32am
    Author:  Lamtharn Hantrakul

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Oscillator    : public Component, public ComboBox::Listener
{
public:
    Oscillator();
    ~Oscillator();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox* box) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
    
    ComboBox oscMenu;
};
