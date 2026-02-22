/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "NebulaEQComponent.h"

//==============================================================================
/**
*/
class NebulaEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NebulaEQAudioProcessorEditor (NebulaEQAudioProcessor&);
    ~NebulaEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NebulaEQAudioProcessor& audioProcessor;
    NebulaEQComponent nebulaEQ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NebulaEQAudioProcessorEditor)
};

