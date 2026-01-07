# Running NebulaEQ in Xcode (Standalone Mode)

Yes! You can run and test your plugin directly in Xcode without needing a DAW. Here's how:

## Steps to Run in Xcode:

1. **Open the Xcode project:**
   - Double-click `Builds/MacOSX/MyVSTPlugin.xcodeproj` in Finder, OR
   - Open Xcode → File → Open → Navigate to the project

2. **Select the Standalone Plugin scheme:**
   - At the top of Xcode, click on the scheme dropdown (next to the play/stop buttons)
   - Select **"MyVSTPlugin - Standalone Plugin"**

3. **Build and Run:**
   - Press **⌘R** (Cmd+R) or click the **▶️ Play button**
   - Xcode will build the plugin and launch it as a standalone app!

4. **The plugin window will open** showing your space-themed NebulaEQ interface!

## Benefits of Standalone Mode:

- ✅ **Fast testing** - No need to open a DAW
- ✅ **Quick iterations** - Build and run instantly after code changes
- ✅ **Debugging** - Set breakpoints and debug directly in Xcode
- ✅ **Audio I/O** - The standalone version will use your Mac's default audio input/output

## Tips:

- **To stop the plugin:** Press **⌘.** (Cmd+Period) or click the Stop button in Xcode
- **After code changes:** Just press **⌘R** again to rebuild and relaunch
- **Debugging:** Set breakpoints in your code, then run - Xcode will pause at them
- **Console output:** Check the Xcode console at the bottom for any debug messages

## Note:

The standalone version runs as a regular macOS app, so you can test the UI and audio processing without a DAW. However, for final testing with actual audio tracks, you'll still want to load it in your DAW.

Enjoy testing! 🚀


