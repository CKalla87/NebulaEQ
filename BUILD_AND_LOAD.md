# Building and Loading NebulaEQ Plugin

## Quick Start (Recommended)

### Option 1: Build in Xcode (Easiest)

1. **Open the Xcode project:**
   ```bash
   open "Builds/MacOSX/MyVSTPlugin.xcodeproj"
   ```

2. **In Xcode:**
   - Select the **"MyVSTPlugin - All"** scheme from the scheme dropdown (top toolbar)
   - Select **"Debug"** configuration
   - Press **⌘B** (Cmd+B) to build, or go to **Product → Build**

3. **The plugin will be automatically copied to your plugin directories:**
   - VST3: `~/Library/Audio/Plug-Ins/VST3/MyVSTPlugin.vst3`
   - AU: `~/Library/Audio/Plug-Ins/Components/MyVSTPlugin.component`

4. **Rescan plugins in your DAW** and NebulaEQ should appear!

### Option 2: Build from Command Line

```bash
cd "Builds/MacOSX"
xcodebuild -project MyVSTPlugin.xcodeproj -scheme "MyVSTPlugin - All" -configuration Debug build
```

## Loading in Your DAW

### Supported Formats:
- **VST3** (recommended for most DAWs)
- **AU** (Audio Unit - macOS only, works in Logic Pro, GarageBand, etc.)

### Common DAWs:

#### Logic Pro / GarageBand:
1. Open Logic/GarageBand
2. Create a new track or select an existing one
3. Click the **Plug-ins** button
4. Navigate to **Audio Units → Your Company → NebulaEQ**
5. The plugin should appear with the space-themed interface!

#### Reaper:
1. Open Reaper
2. Add the plugin to a track (FX button)
3. Go to **VST3 → Your Company → NebulaEQ**

#### Ableton Live:
1. Open Ableton
2. Add the plugin to an audio track
3. Look for **NebulaEQ** in your VST3 plugins

#### Pro Tools:
1. Pro Tools supports AAX format (requires separate build configuration)
2. For VST3, you may need to use a VST-to-AAX wrapper

#### Other DAWs:
- Look for **NebulaEQ** or **MyVSTPlugin** in your VST3 or AU plugin list
- The plugin should appear in the plugin browser/menu

## Troubleshooting

### Plugin doesn't appear in DAW:
1. **Make sure you've built the plugin successfully** (check for errors in Xcode)
2. **Rescan plugins** in your DAW (usually in Preferences/Settings → Plug-ins)
3. **Check the plugin location:**
   - VST3: `~/Library/Audio/Plug-Ins/VST3/MyVSTPlugin.vst3`
   - AU: `~/Library/Audio/Plug-Ins/Components/MyVSTPlugin.component`
4. **Restart your DAW** after building

### Plugin crashes:
- Check Xcode console for error messages
- Make sure you've built the Debug configuration first
- Try building a Release version: Change scheme to "Release" and rebuild

### Need to rebuild after code changes:
- Simply press **⌘B** in Xcode again
- The plugin will be automatically updated in the plugin directories
- Rescan in your DAW (or restart the DAW)

## Using the Plugin

Once loaded:
1. You'll see the space-themed interface with "NebulaEQ" at the top
2. **5 frequency bands** with controls:
   - **Low** (20-200 Hz)
   - **Low-Mid** (100-800 Hz)
   - **Mid** (300-3000 Hz)
   - **High-Mid** (2000-8000 Hz)
   - **High** (6000-20000 Hz)
3. Each band has:
   - **Frequency knob** (rotary) - adjust the center frequency
   - **Gain slider** (vertical) - boost or cut in dB (-24 to +24 dB)

Enjoy your space-themed EQ! 🚀✨


