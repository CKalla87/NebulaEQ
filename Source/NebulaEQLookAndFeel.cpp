#include "NebulaEQLookAndFeel.h"

static juce::Colour colourFromHex (int hex)
{
    return juce::Colour::fromRGB ((juce::uint8) ((hex >> 16) & 0xff),
                                  (juce::uint8) ((hex >> 8) & 0xff),
                                  (juce::uint8) (hex & 0xff));
}

void NebulaEQLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button,
                                                const juce::Colour&,
                                                bool shouldDrawButtonAsHighlighted,
                                                bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    const float cornerSize = 10.0f;

    const bool isPower = button.getComponentID() == "power";
    const bool isOn = isPower && button.getToggleState();

    juce::Colour base = isOn ? colourFromHex (0x059669) : colourFromHex (0x334155).withAlpha (0.5f);
    juce::Colour border = isOn ? colourFromHex (0x10b981) : colourFromHex (0x475569);

    if (shouldDrawButtonAsDown)
        base = base.darker (0.1f);
    else if (shouldDrawButtonAsHighlighted)
        base = base.brighter (0.08f);

    if (! button.isEnabled())
    {
        base = base.withAlpha (0.3f);
        border = border.withAlpha (0.3f);
    }

    g.setColour (base);
    g.fillRoundedRectangle (bounds, cornerSize);

    g.setColour (border.withAlpha (0.8f));
    g.drawRoundedRectangle (bounds, cornerSize, 1.0f);
}

void NebulaEQLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button,
                                          bool, bool)
{
    auto bounds = button.getLocalBounds();
    const bool isPower = button.getComponentID() == "power";
    const bool isOn = isPower && button.getToggleState();
    const bool isReset = button.getComponentID() == "reset";

    juce::Colour textColour = isOn ? juce::Colours::white : colourFromHex (0xcbd5e1);
    if (! button.isEnabled())
        textColour = textColour.withAlpha (0.4f);

    g.setColour (textColour);
    auto font = juce::Font (15.0f, juce::Font::bold);
    g.setFont (font);

    const float iconSize = juce::jlimit (18.0f, 20.0f, bounds.getHeight() * 0.5f);
    const float gap = juce::jlimit (10.0f, 12.0f, bounds.getHeight() * 0.3f);
    const int textWidth = font.getStringWidth (button.getButtonText());
    const float totalWidth = iconSize + gap + (float) textWidth;
    const float startX = (float) bounds.getCentreX() - totalWidth * 0.5f;

    auto contentRow = juce::Rectangle<float> (startX, (float) bounds.getY(),
                                              totalWidth, (float) bounds.getHeight());
    auto iconBounds = contentRow.removeFromLeft (iconSize)
                                  .withSizeKeepingCentre (iconSize, iconSize);
    contentRow.removeFromLeft (gap);

    juce::Path iconPath;
    bool drewImageIcon = false;
    if (isPower)
    {
        iconPath.addCentredArc (iconBounds.getCentreX(), iconBounds.getCentreY(),
                                iconBounds.getWidth() * 0.45f, iconBounds.getHeight() * 0.45f,
                                0.0f, juce::MathConstants<float>::pi * 0.25f,
                                juce::MathConstants<float>::pi * 1.75f, true);
        iconPath.startNewSubPath (iconBounds.getCentreX(), iconBounds.getY());
        iconPath.lineTo (iconBounds.getCentreX(), iconBounds.getCentreY());
    }
    else if (isReset)
    {
        auto resetBounds = iconBounds.withSizeKeepingCentre (iconBounds.getWidth() * 3.0f,
                                                             iconBounds.getHeight() * 3.0f);
        static const juce::Image resetIcon = juce::ImageCache::getFromMemory (BinaryData::reset_arrow_png,
                                                                              BinaryData::reset_arrow_pngSize);

        if (resetIcon.isValid())
        {
            g.drawImageWithin (resetIcon,
                               (int) resetBounds.getX(),
                               (int) resetBounds.getY(),
                               (int) resetBounds.getWidth(),
                               (int) resetBounds.getHeight(),
                               juce::RectanglePlacement::centred);
            drewImageIcon = true;
        }
    }

    if (! drewImageIcon)
    {
        const float strokeThickness = iconSize * 0.11f;
        g.strokePath (iconPath, juce::PathStrokeType (strokeThickness,
                                                      juce::PathStrokeType::curved,
                                                      juce::PathStrokeType::rounded));
    }

    auto textBounds = contentRow.toNearestInt();
    g.drawFittedText (button.getButtonText(), textBounds, juce::Justification::centredLeft, 1);
}

void NebulaEQLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const juce::Slider::SliderStyle style,
                                            juce::Slider& slider)
{
    juce::ignoreUnused (minSliderPos, maxSliderPos);

    if (style != juce::Slider::LinearHorizontal)
    {
        LookAndFeel_V4::drawLinearSlider (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos,
                                          style, slider);
        return;
    }

    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    auto trackHeight = juce::jlimit (6.0f, 10.0f, bounds.getHeight() * 0.35f);
    auto trackY = bounds.getCentreY() - trackHeight * 0.5f;
    auto track = juce::Rectangle<float> (bounds.getX(), trackY, bounds.getWidth(), trackHeight);

    auto base = colourFromHex (0x0b1220);
    auto highlight = colourFromHex (0x1e293b);

    if (! slider.isEnabled())
    {
        base = base.withAlpha (0.4f);
        highlight = highlight.withAlpha (0.4f);
    }

    g.setColour (base);
    g.fillRoundedRectangle (track, trackHeight * 0.5f);

    g.setColour (highlight.withAlpha (0.6f));
    g.drawRoundedRectangle (track, trackHeight * 0.5f, 1.0f);

    const float thumbRadius = juce::jlimit (6.0f, 9.0f, bounds.getHeight() * 0.35f);
    auto thumbCentre = juce::Point<float> (sliderPos, bounds.getCentreY());

    if (slider.isEnabled())
    {
        g.setColour (juce::Colours::black.withAlpha (0.35f));
        g.fillEllipse (thumbCentre.x - thumbRadius - 1.0f, thumbCentre.y - thumbRadius + 1.0f,
                       (thumbRadius + 1.0f) * 2.0f, (thumbRadius + 1.0f) * 2.0f);
    }

    g.setColour (colourFromHex (0xe2e8f0));
    g.fillEllipse (thumbCentre.x - thumbRadius, thumbCentre.y - thumbRadius,
                   thumbRadius * 2.0f, thumbRadius * 2.0f);

    g.setColour (colourFromHex (0x0f172a));
    g.drawEllipse (thumbCentre.x - thumbRadius, thumbCentre.y - thumbRadius,
                   thumbRadius * 2.0f, thumbRadius * 2.0f, 1.0f);
}
