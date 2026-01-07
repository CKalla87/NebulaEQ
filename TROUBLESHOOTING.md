# Troubleshooting: Build Succeeds But Nothing Loads

If your build succeeds in Xcode but the plugin doesn't load/show, try these steps:

## 1. Check Xcode Console

- In Xcode, look at the **bottom panel** (Console/Output)
- When you run (⌘R), check for any error messages or crash reports
- Look for messages like "EXC_BAD_ACCESS", "segmentation fault", or assertion failures

## 2. Check if the App is Running

- Open **Activity Monitor** (Applications → Utilities → Activity Monitor)
- Search for "MyVSTPlugin" or "NebulaEQ"
- If you see the process running but no window, the app may have crashed silently

## 3. Run from Terminal to See Errors

Try running the app directly from Terminal to see error messages:

```bash
cd "/Users/christopherkalla/Software Projects/NebulaEQ/Builds/MacOSX/build/Debug"
./MyVSTPlugin.app/Contents/MacOS/MyVSTPlugin
```

This will show any error messages in the terminal.

## 4. Check Scheme Settings

- Make sure you selected **"MyVSTPlugin - Standalone Plugin"** scheme
- Not "MyVSTPlugin - All" or other schemes
- The scheme dropdown is at the top of Xcode next to the play button

## 5. Clean and Rebuild

Sometimes Xcode needs a clean build:

- In Xcode: **Product → Clean Build Folder** (Shift+⌘+K)
- Then build again: **Product → Build** (⌘+B)
- Then run: **Product → Run** (⌘+R)

## 6. Check for Code Issues

Common issues that prevent loading:
- Crashes in constructor (check editor initialization)
- Missing parameter IDs
- Uninitialized variables
- Memory access violations

## 7. Check Console.app

- Open **Console.app** (Applications → Utilities → Console)
- Filter for "MyVSTPlugin" or crash reports
- Look for any system-level error messages

## Next Steps

If you see specific error messages, share them and we can fix the code!


