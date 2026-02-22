/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NebulaEQAudioProcessorEditor::NebulaEQAudioProcessorEditor (NebulaEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), nebulaEQ (p.apvts)
{
    setSize (1000, 700);
    addAndMakeVisible (nebulaEQ);
}

NebulaEQAudioProcessorEditor::~NebulaEQAudioProcessorEditor()
{
}

//==============================================================================
void NebulaEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void NebulaEQAudioProcessorEditor::resized()
{
    nebulaEQ.setBounds (getLocalBounds());
}
