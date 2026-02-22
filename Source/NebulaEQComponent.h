#pragma once

#include <JuceHeader.h>
#include "LEDBarMeter.h"
#include "FrequencyResponseComponent.h"
#include "NebulaEQLookAndFeel.h"

class NebulaEQComponent : public juce::Component
{
public:
    explicit NebulaEQComponent (juce::AudioProcessorValueTreeState& apvts);
    ~NebulaEQComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    bool isPowered() const;

private:
    struct BandSpec
    {
        juce::String id;
        juce::String frequency;
        juce::Colour colour;
    };

    void updateBandVisuals();
    void updatePowerState();
    void resetEQ();

    juce::TextButton resetButton { "Reset" };
    juce::TextButton powerButton { "On" };

    std::array<juce::Slider, 5> bandSliders;
    std::array<LEDBarMeter, 5> meters;
    FrequencyResponseComponent response;

    std::array<BandSpec, 5> bandSpecs;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 5> gainAttachments;

    std::array<juce::Rectangle<int>, 5> meterBounds;
    std::array<juce::Rectangle<int>, 5> sliderBounds;
    std::array<juce::Rectangle<int>, 5> valueBounds;
    std::array<juce::Rectangle<int>, 5> freqLabelBounds;
    std::array<juce::Rectangle<int>, 5> bandLabelBounds;

    juce::Rectangle<int> headerArea;
    juce::Rectangle<int> panelArea;
    juce::Rectangle<int> bandArea;
    juce::Rectangle<int> graphArea;
    juce::Rectangle<int> graphPlotArea;
    juce::Rectangle<int> statusArea;

    NebulaEQLookAndFeel lookAndFeel;
    bool isPoweredOn = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NebulaEQComponent)
};
