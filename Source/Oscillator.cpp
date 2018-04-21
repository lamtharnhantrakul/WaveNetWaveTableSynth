/*
  ==============================================================================

    Oscillator.cpp
    Created: 21 Apr 2018 10:21:32am
    Author:  Lamtharn Hantrakul

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator()
{
    setSize(200,200);
    
    oscMenu.addItem("Sine", 1);
    oscMenu.addItem("Saw", 2);
    oscMenu.addItem("Square", 3);
    addAndMakeVisible(&oscMenu);
    oscMenu.addListener(this);
    
    oscMenu.setJustificationType(Justification::centred);
}

Oscillator::~Oscillator()
{
}

void Oscillator::paint (Graphics& g)
{
    
}

void Oscillator::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40);
    oscMenu.setBounds(area.removeFromTop(20));
}

void Oscillator::comboBoxChanged(ComboBox* box)
{
}
