#pragma once

#include <JuceHeader.h>

class FrequencyResponseComponent : public juce::Component
{
public:
    FrequencyResponseComponent();
    explicit FrequencyResponseComponent (std::array<juce::Colour, 5> colours);

    void setPoweredOn (bool shouldBeOn);
    void setGains (const std::array<float, 5>& newGains);
    void setBandColours (const std::array<juce::Colour, 5>& newColours);

    void paint (juce::Graphics& g) override;

private:
    std::array<float, 5> gains { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    std::array<juce::Colour, 5> bandColours {
        juce::Colour::fromRGB (239, 68, 68),
        juce::Colour::fromRGB (249, 115, 22),
        juce::Colour::fromRGB (234, 179, 8),
        juce::Colour::fromRGB (34, 197, 94),
        juce::Colour::fromRGB (59, 130, 246)
    };
    bool poweredOn = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyResponseComponent)
};
