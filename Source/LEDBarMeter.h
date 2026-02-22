#pragma once

#include <JuceHeader.h>

class LEDBarMeter : public juce::Component
{
public:
    LEDBarMeter();
    explicit LEDBarMeter (juce::Colour bandColour);

    void setBandColour (juce::Colour newColour);
    void setGain (float newGain);
    void setPoweredOn (bool shouldBeOn);

    void paint (juce::Graphics& g) override;

private:
    juce::Colour bandColour { juce::Colour::fromRGB (239, 68, 68) };
    float gain = 0.0f;
    bool poweredOn = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LEDBarMeter)
};
