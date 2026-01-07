# Fix Plugin Name - Quick Steps

The .jucer file has been updated with the plugin name. Now you need to:

## Steps to Fix:

1. **Open NebulaEQ.jucer in JUCE Projucer**

2. **Click "Save and Open in IDE"** (or File → Save Project)
   - This regenerates JucePluginDefines.h with "NebulaEQ" instead of "MyVSTPlugin"

3. **Rebuild the plugin in Xcode:**
   - Clean: Shift+⌘+K
   - Build: ⌘+B

4. **Remove old plugin from Ableton:**
   - Delete: `~/Library/Audio/Plug-Ins/VST3/MyVSTPlugin.vst3`
   - Delete: `~/Library/Audio/Plug-Ins/Components/MyVSTPlugin.component`

5. **Install new plugin:**
   - Copy the newly built plugin from:
     - `Builds/MacOSX/build/Release/NebulaEQ.vst3` → `~/Library/Audio/Plug-Ins/VST3/`
     - `Builds/MacOSX/build/Release/NebulaEQ.component` → `~/Library/Audio/Plug-Ins/Components/`

6. **Rescan plugins in Ableton:**
   - Preferences → Plug-ins → Rescan
   - The plugin should now appear as "NebulaEQ" instead of "MyVSTPlugin"

The key is step 2 - saving in Projucer regenerates all the defines files with the correct name!

