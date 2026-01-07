/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class NebulaEQAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NebulaEQAudioProcessor();
    ~NebulaEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter management
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    // EQ parameters (5 bands)
    std::atomic<float>* lowFreqParam = nullptr;
    std::atomic<float>* lowGainParam = nullptr;
    std::atomic<float>* lowMidFreqParam = nullptr;
    std::atomic<float>* lowMidGainParam = nullptr;
    std::atomic<float>* midFreqParam = nullptr;
    std::atomic<float>* midGainParam = nullptr;
    std::atomic<float>* highMidFreqParam = nullptr;
    std::atomic<float>* highMidGainParam = nullptr;
    std::atomic<float>* highFreqParam = nullptr;
    std::atomic<float>* highGainParam = nullptr;

private:
    //==============================================================================
    // DSP processing chain
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    using FilterChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter, Filter>;

    void updateFilters();

    FilterChain filters[2]; // One for each channel
    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NebulaEQAudioProcessor)
};

