/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

    // Space-themed colors
    juce::Colour spaceDark = juce::Colour::fromFloatRGBA (0.05f, 0.05f, 0.15f, 1.0f);
    juce::Colour spaceMedium = juce::Colour::fromFloatRGBA (0.15f, 0.1f, 0.25f, 1.0f);
    juce::Colour nebulaPurple = juce::Colour::fromFloatRGBA (0.6f, 0.3f, 0.9f, 1.0f);
    juce::Colour nebulaBlue = juce::Colour::fromFloatRGBA (0.3f, 0.4f, 0.9f, 1.0f);
    juce::Colour starColor = juce::Colours::white.withAlpha (0.8f);

    // EQ Band Controls (5 bands)
    struct EQBand
    {
        juce::Slider freqSlider;
        juce::Slider gainSlider;
        juce::Label freqLabel;
        juce::Label gainLabel;
        juce::String name;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    };

    EQBand bands[5];

    void drawStars (juce::Graphics& g);
    void setupBand (int index, const juce::String& freqParam, const juce::String& gainParam, const juce::String& name);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NebulaEQAudioProcessorEditor)
};

