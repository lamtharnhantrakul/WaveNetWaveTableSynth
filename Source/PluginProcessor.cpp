/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveNetWaveTableAudioProcessor::WaveNetWaveTableAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
attackTime(0.1f),
tree (*this, nullptr)
#endif
{
    // >>> For Updating slider changes... a DAW requires everything to be normalized from 0.0 - 1.0
    NormalisableRange<float> attackParam(0.1f, 5000.0f);
    NormalisableRange<float> decayParam(0.1f, 5000.0f);
    NormalisableRange<float> sustainParam(0.1f, 5000.0f);
    NormalisableRange<float> releaseParam(0.1f, 5000.0f);
    NormalisableRange<float> ampParam(0.0f, 1.0f);
    
    // The tree object is used to communicate values between the `PluginEditor` and the `PluginProcessor`
    tree.createAndAddParameter(("attack"), "Attack", "Attack", attackParam, 0.1f, nullptr, nullptr);
    tree.createAndAddParameter(("decay"), "Decay", "Decay", decayParam, 0.1f, nullptr, nullptr);
    tree.createAndAddParameter(("sustain"), "Sustain", "sustain", sustainParam, 5000.0f, nullptr, nullptr);
    tree.createAndAddParameter(("release"), "Release", "Release", releaseParam, 0.0f, nullptr, nullptr);
    tree.createAndAddParameter(("amp"), "Amp", "Amp", ampParam, 0.8f, nullptr, nullptr);
    
    mySynth.clearVoices();
    // >>> For Synthesizer
    // Add synth voices (max number of voices)
    for (int i = 0; i < 8; i++) {
        mySynth.addVoice(new SynthVoice());
    }
    
    mySynth.clearSounds();
    mySynth.addSound(new SynthSound());
    
    // >>> For Ppm meter
    CPpm::createInstance(m_pCPpm);
}

WaveNetWaveTableAudioProcessor::~WaveNetWaveTableAudioProcessor()
{
    CPpm::destroyInstance(m_pCPpm);
}

//==============================================================================
const String WaveNetWaveTableAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveNetWaveTableAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveNetWaveTableAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveNetWaveTableAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveNetWaveTableAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveNetWaveTableAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveNetWaveTableAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveNetWaveTableAudioProcessor::setCurrentProgram (int index)
{
}

const String WaveNetWaveTableAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveNetWaveTableAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WaveNetWaveTableAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(lastSampleRate);
    
    // Initialize CPpm object
    m_pCPpm->initInstance(sampleRate, getTotalNumInputChannels());
    
    // Create an array to store the output Vppm per channel that is computed everytime we get a block
    m_iTotalNumInputChannels = getTotalNumInputChannels();
    m_pfOutputVppm = new float [m_iTotalNumInputChannels];
}

void WaveNetWaveTableAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveNetWaveTableAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void WaveNetWaveTableAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // how to send tree value information from slider to processor is from tutorial https://www.youtube.com/watch?v=PaKXRm6RZF4&index=25&list=PLLgJJsrdwhPxa6-02-CeHW8ocwSwl2jnu
    for (int v = 0; v < mySynth.getNumVoices(); v++) {
        if ((myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(v))))
        {
            myVoice->getParam(tree.getRawParameterValue("amp"), tree.getRawParameterValue("attack"), tree.getRawParameterValue("decay"), tree.getRawParameterValue("sustain"), tree.getRawParameterValue("release"));
        }
    }
    
    // Make sure there are no left over bits in the buffer
    buffer.clear();
    // Write audio from synth to buffer
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    // Process the audio buffer to color the meter
    const float **readPointers = buffer.getArrayOfReadPointers();
    //DBG(readPointers[0][1500]);
    m_pCPpm->process(readPointers, m_pfOutputVppm, buffer.getNumSamples());
    
    
}

//==============================================================================
bool WaveNetWaveTableAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaveNetWaveTableAudioProcessor::createEditor()
{
    return new WaveNetWaveTableAudioProcessorEditor (*this);
}

//==============================================================================
void WaveNetWaveTableAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveNetWaveTableAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
float WaveNetWaveTableAudioProcessor::getVppm()
{
    // Output mean of Vppm across channels
    float sum = 0.0f;
    for (int c = 0; c < m_iTotalNumInputChannels; c++) {
        sum += m_pfOutputVppm[c];
    }
    return sum / static_cast<float>(m_iTotalNumInputChannels);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveNetWaveTableAudioProcessor();
}
