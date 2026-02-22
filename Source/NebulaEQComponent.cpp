#include "NebulaEQComponent.h"

static juce::Colour colourFromHex (int hex)
{
    return juce::Colour::fromRGB ((juce::uint8) ((hex >> 16) & 0xff),
                                  (juce::uint8) ((hex >> 8) & 0xff),
                                  (juce::uint8) (hex & 0xff));
}

NebulaEQComponent::NebulaEQComponent (juce::AudioProcessorValueTreeState& apvts)
{
    setLookAndFeel (&lookAndFeel);
    setOpaque (true);

    bandSpecs = {{
        { "low", "60 Hz", colourFromHex (0xef4444) },
        { "low-mid", "250 Hz", colourFromHex (0xf97316) },
        { "mid", "1 kHz", colourFromHex (0xeab308) },
        { "high-mid", "4 kHz", colourFromHex (0x22c55e) },
        { "high", "12 kHz", colourFromHex (0x3b82f6) }
    }};

    std::array<juce::Colour, 5> colours {};
    for (size_t i = 0; i < bandSpecs.size(); ++i)
    {
        colours[i] = bandSpecs[i].colour;
        meters[i].setBandColour (bandSpecs[i].colour);
        addAndMakeVisible (meters[i]);
    }

    response.setBandColours (colours);
    addAndMakeVisible (response);

    for (size_t i = 0; i < bandSliders.size(); ++i)
    {
        auto& slider = bandSliders[i];
        slider.setSliderStyle (juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        slider.setRange (-12.0, 12.0, 0.5);
        slider.setValue (0.0, juce::dontSendNotification);
        slider.setPopupDisplayEnabled (false, false, this);
        slider.setScrollWheelEnabled (false);

        slider.onValueChange = [this]
        {
            updateBandVisuals();
        };

        addAndMakeVisible (slider);
    }

    gainAttachments[0] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, "LOW_GAIN", bandSliders[0]);
    gainAttachments[1] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, "LOWMID_GAIN", bandSliders[1]);
    gainAttachments[2] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, "MID_GAIN", bandSliders[2]);
    gainAttachments[3] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, "HIGHMID_GAIN", bandSliders[3]);
    gainAttachments[4] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, "HIGH_GAIN", bandSliders[4]);

    resetButton.setComponentID ("reset");
    resetButton.onClick = [this] { resetEQ(); };
    addAndMakeVisible (resetButton);

    powerButton.setComponentID ("power");
    powerButton.setClickingTogglesState (true);
    powerButton.setToggleState (true, juce::dontSendNotification);
    powerButton.onClick = [this]
    {
        isPoweredOn = powerButton.getToggleState();
        powerButton.setButtonText (isPoweredOn ? "On" : "Off");
        updatePowerState();
    };
    addAndMakeVisible (powerButton);

    updatePowerState();
    updateBandVisuals();
}

NebulaEQComponent::~NebulaEQComponent()
{
    setLookAndFeel (nullptr);
}

bool NebulaEQComponent::isPowered() const
{
    return isPoweredOn;
}

void NebulaEQComponent::paint (juce::Graphics& g)
{
    auto backgroundTop = colourFromHex (0x0f172a);
    auto backgroundMid = colourFromHex (0x1e293b);
    auto backgroundBottom = colourFromHex (0x0f172a);

    juce::ColourGradient backgroundGradient (backgroundTop, 0.0f, 0.0f,
                                             backgroundBottom, 0.0f, (float) getHeight(), false);
    backgroundGradient.addColour (0.5, backgroundMid);
    g.setGradientFill (backgroundGradient);
    g.fillAll();

    auto shadowColour = juce::Colours::black.withAlpha (0.45f);
    juce::DropShadow (shadowColour, 22, { 0, 8 }).drawForRectangle (g, panelArea);

    auto panelGradient = juce::ColourGradient (colourFromHex (0x1e293b), (float) panelArea.getX(),
                                               (float) panelArea.getY(),
                                               colourFromHex (0x0f172a), (float) panelArea.getRight(),
                                               (float) panelArea.getBottom(), false);
    g.setGradientFill (panelGradient);
    g.fillRoundedRectangle (panelArea.toFloat(), 18.0f);

    g.setColour (colourFromHex (0x334155).withAlpha (0.6f));
    g.drawRoundedRectangle (panelArea.toFloat().reduced (0.5f), 18.0f, 1.0f);

    // Header icon tile
    auto headerBounds = headerArea;
    auto headerLeft = headerBounds.removeFromLeft (headerBounds.getWidth() * 0.6f);
    auto iconSlot = headerLeft.removeFromLeft (60);

    // Header text
    auto textArea = headerLeft.withTrimmedLeft (8);
    const int titleHeight = 50;
    const int subtitleHeight = 18;
    const int totalTextHeight = titleHeight + subtitleHeight;
    const int subtitleBottomPadding = 8;
    const int textTop = textArea.getCentreY() - totalTextHeight / 2 - subtitleBottomPadding;
    auto textBlock = juce::Rectangle<int> (textArea.getX(), textTop, textArea.getWidth(), totalTextHeight);
    auto titleArea = juce::Rectangle<int> (textArea.getX(), textTop, textArea.getWidth(), titleHeight);
    auto subtitleArea = juce::Rectangle<int> (textArea.getX(), textTop + titleHeight,
                                              textArea.getWidth(), subtitleHeight);

    auto iconBounds = iconSlot.withSizeKeepingCentre (48, 48).toFloat();
    iconBounds.setCentre (iconBounds.getCentreX(), (float) textBlock.getCentreY());
    juce::ColourGradient iconGradient (colourFromHex (0x22d3ee), iconBounds.getX(), iconBounds.getY(),
                                       colourFromHex (0x2563eb), iconBounds.getRight(), iconBounds.getBottom(), false);
    g.setGradientFill (iconGradient);
    g.fillRoundedRectangle (iconBounds, 12.0f);

    g.setColour (juce::Colours::white);
    auto waveArea = iconBounds.reduced (10.0f);
    juce::Path waves;
    const float waveStep = waveArea.getHeight() / 3.5f;
    for (int i = 0; i < 3; ++i)
    {
        const float y = waveArea.getY() + waveStep * (i + 0.5f);
        waves.startNewSubPath (waveArea.getX(), y);
        waves.quadraticTo (waveArea.getCentreX(), y - 4.0f, waveArea.getRight(), y);
    }
    g.strokePath (waves, juce::PathStrokeType (2.2f, juce::PathStrokeType::curved,
                                               juce::PathStrokeType::rounded));

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (46.0f, juce::Font::bold));
    g.drawText ("NebulaEQ", titleArea, juce::Justification::left, false);

    g.setColour (colourFromHex (0x94a3b8));
    g.setFont (juce::Font (15.0f, juce::Font::plain));
    g.drawText ("Professional 5-Band Equalizer", subtitleArea, juce::Justification::left, false);

    // Band labels and values
    for (size_t i = 0; i < bandSpecs.size(); ++i)
    {
        const float gainValue = (float) bandSliders[i].getValue();
        const bool highlight = isPoweredOn && std::abs (gainValue) > 0.001f;

        juce::Colour valueColour = highlight ? bandSpecs[i].colour : colourFromHex (0x94a3b8);

        juce::Font valueFont (juce::Font::getDefaultMonospacedFontName(),
                              24.0f, juce::Font::bold);
        g.setFont (valueFont);

        if (highlight)
        {
            g.setColour (bandSpecs[i].colour.withAlpha (0.35f));
            auto glowBounds = valueBounds[i].toFloat();
            g.drawText (juce::String (gainValue >= 0.0f ? "+" : "") + juce::String (gainValue, 1),
                        glowBounds.translated (0, 1), juce::Justification::centred, false);
            g.drawText (juce::String (gainValue >= 0.0f ? "+" : "") + juce::String (gainValue, 1),
                        glowBounds.translated (1, 0), juce::Justification::centred, false);
        }

        g.setColour (valueColour);
        g.drawText (juce::String (gainValue >= 0.0f ? "+" : "") + juce::String (gainValue, 1),
                    valueBounds[i], juce::Justification::centred, false);

        g.setFont (juce::Font (13.0f, juce::Font::bold));
        g.setColour (colourFromHex (0x94a3b8));
        g.drawText (bandSpecs[i].frequency, freqLabelBounds[i], juce::Justification::centred, false);

        g.setFont (juce::Font (11.0f, juce::Font::plain));
        g.setColour (colourFromHex (0x475569));
        g.drawText (bandSpecs[i].id.replace ("-", " ").toUpperCase(),
                    bandLabelBounds[i], juce::Justification::centred, false);
    }

    // Graph container background
    g.setColour (colourFromHex (0x020617).withAlpha (0.55f));
    g.fillRoundedRectangle (graphArea.toFloat(), 14.0f);
    g.setColour (colourFromHex (0x334155).withAlpha (0.5f));
    g.drawRoundedRectangle (graphArea.toFloat().reduced (0.5f), 14.0f, 1.0f);

    // Graph container heading and labels
    auto graphHeader = graphArea.reduced (18).removeFromTop (20);
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.setColour (colourFromHex (0xcbd5e1));
    g.drawText ("Frequency Response", graphHeader, juce::Justification::left, false);

    g.setFont (juce::Font (11.0f, juce::Font::plain));
    g.setColour (colourFromHex (0x64748b));
    auto legendArea = graphHeader.removeFromRight (160);
    g.drawText ("+12 dB", legendArea.removeFromLeft (50), juce::Justification::right, false);
    g.drawText ("0 dB", legendArea.removeFromLeft (50), juce::Justification::centred, false);
    g.drawText ("-12 dB", legendArea, juce::Justification::left, false);

    // Status bar
    auto statusBounds = statusArea;
    auto statusLeft = statusBounds.removeFromLeft (statusBounds.getWidth() * 0.6f);
    g.setColour (isPoweredOn ? colourFromHex (0x34d399) : colourFromHex (0x475569));
    constexpr float statusDotSize = 8.0f;
    constexpr float statusDotOffset = -4.0f;
    auto dot = juce::Rectangle<float> ((float) statusLeft.getX() + statusDotOffset,
                                       (float) statusLeft.getCentreY() - (statusDotSize * 0.5f),
                                       statusDotSize,
                                       statusDotSize);
    g.fillEllipse (dot);

    g.setFont (juce::Font (12.0f, juce::Font::plain));
    g.setColour (colourFromHex (0x94a3b8));
    auto statusTextArea = statusLeft.withTrimmedLeft (10);
    g.drawText ("Status:", statusTextArea.removeFromLeft (32), juce::Justification::left, false);

    g.setColour (isPoweredOn ? colourFromHex (0x34d399) : colourFromHex (0x64748b));
    g.drawText (isPoweredOn ? "Active" : "Standby", statusTextArea, juce::Justification::left, false);

    auto statusRight = statusBounds.removeFromRight (140);
    g.setColour (colourFromHex (0x475569));
    g.drawText ("Latency:", statusRight.removeFromLeft (70), juce::Justification::right, false);
    g.setColour (colourFromHex (0x94a3b8));
    g.drawText ("2.3ms", statusRight, juce::Justification::left, false);
}

void NebulaEQComponent::resized()
{
    auto bounds = getLocalBounds().reduced (28);
    headerArea = bounds.removeFromTop (84);

    auto buttonArea = headerArea.removeFromRight (240);
    const int buttonHeight = 36;
    const int buttonWidth = 110;
    const int buttonGap = 12;
    auto resetBounds = buttonArea.removeFromLeft (buttonWidth);
    buttonArea.removeFromLeft (buttonGap);
    auto powerBounds = buttonArea.removeFromLeft (buttonWidth);

    resetButton.setBounds (resetBounds.withSizeKeepingCentre (buttonWidth, buttonHeight));
    powerButton.setBounds (powerBounds.withSizeKeepingCentre (buttonWidth, buttonHeight));

    panelArea = bounds;

    auto panelContent = panelArea.reduced (28);
    const int statusHeight = 28;
    const int spacing = 40;
    const int minGraphHeight = 200;
    const int availableHeight = panelContent.getHeight() - statusHeight - spacing;

    int bandHeight = juce::jlimit (240, 320, (int) (availableHeight * 0.5f));
    if (availableHeight - bandHeight < minGraphHeight)
        bandHeight = juce::jmax (200, availableHeight - minGraphHeight);

    bandArea = panelContent.removeFromTop (bandHeight);
    panelContent.removeFromTop (spacing);
    statusArea = panelContent.removeFromBottom (statusHeight);
    graphArea = panelContent;

    auto graphContent = graphArea.reduced (18);
    graphContent.removeFromTop (24);
    graphPlotArea = graphContent.removeFromTop (juce::jlimit (120, 140, graphContent.getHeight() - 8));
    response.setBounds (graphPlotArea);

    const int gap = juce::jlimit (16, 24, bandArea.getWidth() / 25);
    const int columnWidth = (bandArea.getWidth() - gap * 4) / 5;
    const int meterHeight = juce::jlimit (150, 180, (int) (bandArea.getHeight() * 0.55f));
    const int sliderHeight = 22;
    const int valueHeight = 32;
    const int freqHeight = 18;
    const int bandLabelHeight = 16;

    for (int i = 0; i < 5; ++i)
    {
        int x = bandArea.getX() + i * (columnWidth + gap);
        int y = bandArea.getY();

        meterBounds[i] = { x, y, columnWidth, meterHeight };
        meters[i].setBounds (meterBounds[i]);
        y += meterHeight + 12;

        sliderBounds[i] = { x, y, columnWidth, sliderHeight };
        bandSliders[i].setBounds (sliderBounds[i]);
        y += sliderHeight + 10;

        valueBounds[i] = { x, y, columnWidth, valueHeight };
        y += valueHeight + 4;

        freqLabelBounds[i] = { x, y, columnWidth, freqHeight };
        y += freqHeight + 2;

        bandLabelBounds[i] = { x, y, columnWidth, bandLabelHeight };
    }
}

void NebulaEQComponent::updateBandVisuals()
{
    std::array<float, 5> gains {};

    for (size_t i = 0; i < bandSliders.size(); ++i)
    {
        gains[i] = (float) bandSliders[i].getValue();
        meters[i].setGain (gains[i]);
    }

    response.setGains (gains);
    repaint();
}

void NebulaEQComponent::updatePowerState()
{
    for (auto& slider : bandSliders)
        slider.setEnabled (isPoweredOn);

    for (auto& meter : meters)
        meter.setPoweredOn (isPoweredOn);

    response.setPoweredOn (isPoweredOn);
    repaint();
}

void NebulaEQComponent::resetEQ()
{
    for (auto& slider : bandSliders)
        slider.setValue (0.0, juce::sendNotificationAsync);
}
