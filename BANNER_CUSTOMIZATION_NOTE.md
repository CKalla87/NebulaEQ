# Notification Banner Customization

The yellow notification banner ("Audio input is muted to avoid feedback loop") is drawn by JUCE's StandaloneFilterWindow class and uses hardcoded colors that can't be easily customized from plugin code.

**Options:**
1. **Accept it** - It only shows when input is muted (to prevent feedback)
2. **Create custom standalone app** - Requires defining `JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP` and creating a custom wrapper (more complex)

The banner colors are hardcoded in JUCE's source code:
- Background: `Colours::lightgoldenrodyellow` 
- Border: `Colours::darkgoldenrod`

Since this is framework code, we can't modify it without changing JUCE itself (not recommended).

**Note:** When you load the plugin in a DAW (not standalone), this banner won't appear at all - it's only for the standalone version.


