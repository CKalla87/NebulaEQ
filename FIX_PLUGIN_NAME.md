# Fix Plugin Name in Ableton

The plugin is showing as "MyVSTPlugin" instead of "NebulaEQ" in Ableton. Here's how to fix it:

## Method 1: Fix in JUCE Projucer (Recommended)

1. **Open NebulaEQ.jucer in JUCE Projucer**

2. **Click on "NebulaEQ" in the left sidebar** (the main project node)

3. **In the right panel, look for "Plugin Name" field**
   - It should show "NebulaEQ" 
   - If it shows "MyVSTPlugin", change it to "NebulaEQ"

4. **Also check "Plugin Description"**
   - Should be "NebulaEQ" or "5-Band Parametric Equalizer"

5. **Save the project:**
   - File → Save Project (⌘+S)
   - OR click "Save and Open in IDE"

6. **Rebuild the plugin:**
   - In Xcode: Clean Build Folder (Shift+⌘+K)
   - Build (⌘+B)
   - This will regenerate JucePluginDefines.h with the correct name

7. **Reinstall the plugin:**
   - Delete the old plugin from:
     - `~/Library/Audio/Plug-Ins/VST3/MyVSTPlugin.vst3`
     - `~/Library/Audio/Plug-Ins/Components/MyVSTPlugin.component`
   - Copy the newly built plugin
   - Rescan plugins in Ableton

## Method 2: Quick Fix (If Projucer isn't available)

The .jucer file already has `name="NebulaEQ"`, so the issue is that the generated files are out of date. You need to:

1. Open the .jucer file in Projucer
2. Click "Save and Open in IDE" 
3. This regenerates all the defines files with the correct name

## After Fixing

Once rebuilt, the plugin should appear as "NebulaEQ" in Ableton's plugin browser instead of "MyVSTPlugin".

