/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NebulaEQAudioProcessorEditor::NebulaEQAudioProcessorEditor (NebulaEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 600);

    // Setup EQ bands
    setupBand (0, "LOW_FREQ", "LOW_GAIN", "Low");
    setupBand (1, "LOWMID_FREQ", "LOWMID_GAIN", "Low-Mid");
    setupBand (2, "MID_FREQ", "MID_GAIN", "Mid");
    setupBand (3, "HIGHMID_FREQ", "HIGHMID_GAIN", "High-Mid");
    setupBand (4, "HIGH_FREQ", "HIGH_GAIN", "High");
}

NebulaEQAudioProcessorEditor::~NebulaEQAudioProcessorEditor()
{
}

void NebulaEQAudioProcessorEditor::setupBand (int index, const juce::String& freqParam, const juce::String& gainParam, const juce::String& name)
{
    auto& band = bands[index];
    band.name = name;

    // Frequency slider
    band.freqSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    band.freqSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    band.freqSlider.setPopupDisplayEnabled (true, false, this);
    band.freqSlider.setTextValueSuffix (" Hz");
    band.freqSlider.setColour (juce::Slider::rotarySliderFillColourId, nebulaPurple);
    band.freqSlider.setColour (juce::Slider::rotarySliderOutlineColourId, spaceMedium);
    band.freqSlider.setColour (juce::Slider::thumbColourId, nebulaBlue);
    band.freqSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    band.freqSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceDark);
    addAndMakeVisible (&band.freqSlider);

    // Gain slider
    band.gainSlider.setSliderStyle (juce::Slider::LinearVertical);
    band.gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
    band.gainSlider.setPopupDisplayEnabled (true, false, this);
    band.gainSlider.setTextValueSuffix (" dB");
    band.gainSlider.setRange (-24.0, 24.0, 0.1);
    band.gainSlider.setValue (0.0);
    band.gainSlider.setColour (juce::Slider::trackColourId, nebulaPurple);
    band.gainSlider.setColour (juce::Slider::thumbColourId, nebulaBlue);
    band.gainSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    band.gainSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceDark);
    band.gainSlider.setColour (juce::Slider::backgroundColourId, spaceMedium);
    addAndMakeVisible (&band.gainSlider);

    // Labels
    band.freqLabel.setText (name, juce::dontSendNotification);
    band.freqLabel.setJustificationType (juce::Justification::centred);
    band.freqLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (&band.freqLabel);

    band.gainLabel.setText ("Gain", juce::dontSendNotification);
    band.gainLabel.setJustificationType (juce::Justification::centred);
    band.gainLabel.setColour (juce::Label::textColourId, juce::Colours::white.withAlpha (0.7f));
    addAndMakeVisible (&band.gainLabel);

    // Attach to parameters
    band.freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, freqParam, band.freqSlider);
    band.gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, gainParam, band.gainSlider);
}

//==============================================================================
void NebulaEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Create space gradient background
    juce::ColourGradient gradient (spaceMedium, 0, 0,
                                   spaceDark, 0, (float) getHeight(),
                                   false);
    gradient.addColour (0.5, nebulaPurple.withAlpha (0.3f));
    g.setGradientFill (gradient);
    g.fillAll();

    // Draw stars
    drawStars (g);

    // Draw title with glow effect
    g.setFont (juce::Font (48.0f, juce::Font::bold));
    juce::Font titleFont (48.0f, juce::Font::bold);

    // Draw glow effect (multiple passes for blur)
    g.setColour (nebulaPurple.withAlpha (0.3f));
    for (int i = -3; i <= 3; ++i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            g.drawText ("NebulaEQ", getWidth() / 2 + i - 150, 20 + j, 300, 60,
                       juce::Justification::centred, false);
        }
    }

    // Draw main title
    g.setColour (juce::Colours::white);
    g.drawText ("NebulaEQ", getWidth() / 2 - 150, 20, 300, 60,
               juce::Justification::centred, false);

    // Draw subtitle
    g.setFont (16.0f);
    g.setColour (nebulaBlue.withAlpha (0.8f));
    g.drawText ("5-Band Parametric Equalizer", getWidth() / 2 - 150, 75, 300, 25,
               juce::Justification::centred, false);

    // Draw decorative lines
    g.setColour (nebulaPurple.withAlpha (0.5f));
    g.drawLine (50.0f, 110.0f, getWidth() - 50.0f, 110.0f, 2.0f);
}

void NebulaEQAudioProcessorEditor::drawStars (juce::Graphics& g)
{
    // Draw random stars
    juce::Random random (12345); // Fixed seed for consistent star positions
    g.setColour (starColor);

    for (int i = 0; i < 50; ++i)
    {
        float x = random.nextFloat() * getWidth();
        float y = random.nextFloat() * getHeight();
        float size = random.nextFloat() * 2.0f + 0.5f;
        float alpha = random.nextFloat() * 0.5f + 0.3f;

        g.setColour (starColor.withAlpha (alpha));
        g.fillEllipse (x, y, size, size);
    }
}

void NebulaEQAudioProcessorEditor::resized()
{
    const int bandWidth = 150;
    const int startX = 50;
    const int startY = 140;
    const int knobSize = 100;
    const int gainHeight = 250;
    const int spacing = (getWidth() - 100 - (bandWidth * 5)) / 4;

    for (int i = 0; i < 5; ++i)
    {
        int x = startX + i * (bandWidth + spacing);
        
        // Frequency knob
        bands[i].freqSlider.setBounds (x + 25, startY, knobSize, knobSize);
        bands[i].freqLabel.setBounds (x, startY + knobSize + 5, bandWidth, 20);

        // Gain slider
        bands[i].gainSlider.setBounds (x + 50, startY + knobSize + 35, 50, gainHeight);
        bands[i].gainLabel.setBounds (x, startY + knobSize + gainHeight + 40, bandWidth, 20);
    }
}
