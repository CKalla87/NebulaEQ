/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NebulaEQAudioProcessor::NebulaEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Get parameter pointers
    lowFreqParam = apvts.getRawParameterValue("LOW_FREQ");
    lowGainParam = apvts.getRawParameterValue("LOW_GAIN");
    lowMidFreqParam = apvts.getRawParameterValue("LOWMID_FREQ");
    lowMidGainParam = apvts.getRawParameterValue("LOWMID_GAIN");
    midFreqParam = apvts.getRawParameterValue("MID_FREQ");
    midGainParam = apvts.getRawParameterValue("MID_GAIN");
    highMidFreqParam = apvts.getRawParameterValue("HIGHMID_FREQ");
    highMidGainParam = apvts.getRawParameterValue("HIGHMID_GAIN");
    highFreqParam = apvts.getRawParameterValue("HIGH_FREQ");
    highGainParam = apvts.getRawParameterValue("HIGH_GAIN");
}

NebulaEQAudioProcessor::~NebulaEQAudioProcessor()
{
}

//==============================================================================
const juce::String NebulaEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NebulaEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NebulaEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NebulaEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NebulaEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NebulaEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NebulaEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NebulaEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NebulaEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void NebulaEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NebulaEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    
    for (int channel = 0; channel < 2; ++channel)
    {
        filters[channel].prepare (spec);
    }
    
    updateFilters();
}

void NebulaEQAudioProcessor::releaseResources()
{
    for (int channel = 0; channel < 2; ++channel)
    {
        filters[channel].reset();
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NebulaEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void NebulaEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateFilters();

    juce::dsp::AudioBlock<float> block (buffer);
    
    for (int channel = 0; channel < totalNumInputChannels && channel < 2; ++channel)
    {
        auto channelBlock = block.getSingleChannelBlock (static_cast<size_t> (channel));
        juce::dsp::ProcessContextReplacing<float> context (channelBlock);
        filters[channel].process (context);
    }
}

//==============================================================================
bool NebulaEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NebulaEQAudioProcessor::createEditor()
{
    return new NebulaEQAudioProcessorEditor (*this);
}

//==============================================================================
void NebulaEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void NebulaEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
void NebulaEQAudioProcessor::updateFilters()
{
    for (int channel = 0; channel < 2; ++channel)
    {
        // Low band
        *filters[channel].get<0>().coefficients = *Coefficients::makePeakFilter (
            currentSampleRate,
            lowFreqParam->load(),
            1.0f,
            juce::Decibels::decibelsToGain (lowGainParam->load())
        );

        // Low-Mid band
        *filters[channel].get<1>().coefficients = *Coefficients::makePeakFilter (
            currentSampleRate,
            lowMidFreqParam->load(),
            1.0f,
            juce::Decibels::decibelsToGain (lowMidGainParam->load())
        );

        // Mid band
        *filters[channel].get<2>().coefficients = *Coefficients::makePeakFilter (
            currentSampleRate,
            midFreqParam->load(),
            1.0f,
            juce::Decibels::decibelsToGain (midGainParam->load())
        );

        // High-Mid band
        *filters[channel].get<3>().coefficients = *Coefficients::makePeakFilter (
            currentSampleRate,
            highMidFreqParam->load(),
            1.0f,
            juce::Decibels::decibelsToGain (highMidGainParam->load())
        );

        // High band
        *filters[channel].get<4>().coefficients = *Coefficients::makePeakFilter (
            currentSampleRate,
            highFreqParam->load(),
            1.0f,
            juce::Decibels::decibelsToGain (highGainParam->load())
        );
    }
}

//==============================================================================
// Parameter layout creation
juce::AudioProcessorValueTreeState::ParameterLayout NebulaEQAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Low band (80 Hz) - version hint 1 for first version
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("LOW_FREQ", 1), "Low Frequency",
        juce::NormalisableRange<float> (20.0f, 200.0f, 1.0f, 0.3f),
        80.0f, "Hz"
    ));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("LOW_GAIN", 1), "Low Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f, "dB"
    ));

    // Low-Mid band (300 Hz)
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("LOWMID_FREQ", 1), "Low-Mid Frequency",
        juce::NormalisableRange<float> (100.0f, 800.0f, 1.0f, 0.3f),
        300.0f, "Hz"
    ));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("LOWMID_GAIN", 1), "Low-Mid Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f, "dB"
    ));

    // Mid band (1000 Hz)
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("MID_FREQ", 1), "Mid Frequency",
        juce::NormalisableRange<float> (300.0f, 3000.0f, 1.0f, 0.3f),
        1000.0f, "Hz"
    ));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("MID_GAIN", 1), "Mid Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f, "dB"
    ));

    // High-Mid band (5000 Hz)
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("HIGHMID_FREQ", 1), "High-Mid Frequency",
        juce::NormalisableRange<float> (2000.0f, 8000.0f, 1.0f, 0.3f),
        5000.0f, "Hz"
    ));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("HIGHMID_GAIN", 1), "High-Mid Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f, "dB"
    ));

    // High band (12000 Hz)
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("HIGH_FREQ", 1), "High Frequency",
        juce::NormalisableRange<float> (6000.0f, 20000.0f, 1.0f, 0.3f),
        12000.0f, "Hz"
    ));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("HIGH_GAIN", 1), "High Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f, "dB"
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NebulaEQAudioProcessor();
}

