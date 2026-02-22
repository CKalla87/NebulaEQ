#include "FrequencyResponseComponent.h"

FrequencyResponseComponent::FrequencyResponseComponent() = default;

FrequencyResponseComponent::FrequencyResponseComponent (std::array<juce::Colour, 5> colours)
    : bandColours (colours)
{
}

void FrequencyResponseComponent::setPoweredOn (bool shouldBeOn)
{
    poweredOn = shouldBeOn;
    repaint();
}

void FrequencyResponseComponent::setGains (const std::array<float, 5>& newGains)
{
    gains = newGains;
    repaint();
}

void FrequencyResponseComponent::setBandColours (const std::array<juce::Colour, 5>& newColours)
{
    bandColours = newColours;
    repaint();
}

void FrequencyResponseComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    auto background = juce::Colour::fromRGB (15, 23, 42);
    auto border = juce::Colour::fromRGB (51, 65, 85).withAlpha (0.5f);

    g.setColour (background);
    g.fillRoundedRectangle (bounds, 8.0f);

    g.setColour (border);
    g.drawRoundedRectangle (bounds.reduced (0.5f), 8.0f, 1.0f);

    auto plot = bounds.reduced (6.0f);
    const auto midY = plot.getCentreY();

    const float dash1[] = { 4.0f, 4.0f };
    const float dash2[] = { 2.0f, 2.0f };

    g.setColour (juce::Colour::fromRGB (51, 65, 85));
    g.drawDashedLine ({ plot.getX(), midY, plot.getRight(), midY }, dash1, 2, 1.0f);

    g.setColour (juce::Colour::fromRGB (30, 41, 59));
    g.drawDashedLine ({ plot.getX(), plot.getY() + plot.getHeight() * 0.25f, plot.getRight(),
                        plot.getY() + plot.getHeight() * 0.25f }, dash2, 2, 1.0f);
    g.drawDashedLine ({ plot.getX(), plot.getY() + plot.getHeight() * 0.75f, plot.getRight(),
                        plot.getY() + plot.getHeight() * 0.75f }, dash2, 2, 1.0f);

    if (! poweredOn)
        return;

    const int numBands = static_cast<int> (gains.size());
    const float scale = (plot.getHeight() * 0.5f) / 12.0f * 0.8f;

    juce::Path curve;
    curve.startNewSubPath (plot.getX(), midY);

    for (int i = 0; i < numBands; ++i)
    {
        const float x = plot.getX() + (plot.getWidth() * i) / (numBands - 1);
        const float y = midY - gains[static_cast<size_t> (i)] * scale;

        if (i == 0)
        {
            curve.lineTo (x, y);
        }
        else
        {
            const float prevX = plot.getX() + (plot.getWidth() * (i - 1)) / (numBands - 1);
            const float prevY = midY - gains[static_cast<size_t> (i - 1)] * scale;
            const float cpX = (prevX + x) * 0.5f;
            curve.quadraticTo (cpX, prevY, x, y);
        }
    }

    juce::ColourGradient strokeGradient (bandColours[0], plot.getX(), midY,
                                         bandColours[4], plot.getRight(), midY, false);
    for (int i = 1; i < numBands; ++i)
    {
        const double position = static_cast<double> (i) / (numBands - 1);
        strokeGradient.addColour (position, bandColours[static_cast<size_t> (i)]);
    }

    juce::Path fillPath = curve;
    fillPath.lineTo (plot.getRight(), plot.getBottom());
    fillPath.lineTo (plot.getX(), plot.getBottom());
    fillPath.closeSubPath();

    auto blendedColour = juce::Colour::fromRGB (
        static_cast<juce::uint8> ((bandColours[0].getRed() + bandColours[4].getRed()) / 2),
        static_cast<juce::uint8> ((bandColours[0].getGreen() + bandColours[4].getGreen()) / 2),
        static_cast<juce::uint8> ((bandColours[0].getBlue() + bandColours[4].getBlue()) / 2));

    juce::ColourGradient fillGradient (blendedColour.withAlpha (0.25f), plot.getX(), plot.getY(),
                                       blendedColour.withAlpha (0.0f), plot.getX(), plot.getBottom(), false);
    g.setGradientFill (fillGradient);
    g.fillPath (fillPath);

    g.setGradientFill (strokeGradient);
    g.strokePath (curve, juce::PathStrokeType (3.0f, juce::PathStrokeType::JointStyle::curved,
                                               juce::PathStrokeType::EndCapStyle::rounded));

    for (int i = 0; i < numBands; ++i)
    {
        const float x = plot.getX() + (plot.getWidth() * i) / (numBands - 1);
        const float y = midY - gains[static_cast<size_t> (i)] * scale;
        const float radius = 5.0f;

        g.setColour (bandColours[static_cast<size_t> (i)].withAlpha (0.4f));
        g.fillEllipse (x - radius - 2.0f, y - radius - 2.0f, (radius + 2.0f) * 2.0f,
                       (radius + 2.0f) * 2.0f);

        g.setColour (bandColours[static_cast<size_t> (i)]);
        g.fillEllipse (x - radius, y - radius, radius * 2.0f, radius * 2.0f);

        g.setColour (juce::Colours::white);
        g.drawEllipse (x - radius, y - radius, radius * 2.0f, radius * 2.0f, 2.0f);
    }
}
