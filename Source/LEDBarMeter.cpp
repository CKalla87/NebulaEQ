#include "LEDBarMeter.h"

LEDBarMeter::LEDBarMeter() = default;

LEDBarMeter::LEDBarMeter (juce::Colour bandColourToUse)
    : bandColour (bandColourToUse)
{
}

void LEDBarMeter::setBandColour (juce::Colour newColour)
{
    bandColour = newColour;
    repaint();
}

void LEDBarMeter::setGain (float newGain)
{
    gain = newGain;
    repaint();
}

void LEDBarMeter::setPoweredOn (bool shouldBeOn)
{
    poweredOn = shouldBeOn;
    repaint();
}

void LEDBarMeter::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto background = juce::Colour::fromRGB (11, 18, 32).withAlpha (0.7f);
    auto border = juce::Colour::fromRGB (51, 65, 85).withAlpha (0.5f);

    g.setColour (background);
    g.fillRoundedRectangle (bounds, 8.0f);

    g.setColour (border);
    g.drawRoundedRectangle (bounds.reduced (0.5f), 8.0f, 1.0f);

    auto content = bounds.reduced (10.0f);
    const int numBars = 12;
    const float gap = 4.0f;
    const float barHeight = (content.getHeight() - gap * (numBars - 1)) / numBars;
    const auto inactiveBase = juce::Colour::fromRGB (51, 65, 85);

    for (int i = 0; i < numBars; ++i)
    {
        const float y = content.getBottom() - (i + 1) * barHeight - i * gap;
        auto barRect = juce::Rectangle<float> (content.getX(), y, content.getWidth(), barHeight);

        const float threshold = (static_cast<float> (i) - 6.0f) * 2.0f;
        const bool isActive = poweredOn && ((gain >= 0.0f && threshold <= gain)
                                            || (gain < 0.0f && threshold >= gain));

        if (isActive)
        {
            g.setColour (bandColour.withAlpha (0.35f));
            g.fillRoundedRectangle (barRect.expanded (1.5f, 1.0f), 2.0f);
        }

        g.setColour ((isActive ? bandColour : inactiveBase).withAlpha (isActive ? 1.0f : 0.2f));
        g.fillRoundedRectangle (barRect, 2.0f);
    }
}
