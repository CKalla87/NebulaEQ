# Rebuilding After Code Changes

If you see the old plugin when running, you need to do a **clean build**:

## In Xcode:

1. **Clean Build Folder:**
   - Press **Shift+⌘+K** (or go to **Product → Clean Build Folder**)
   - This removes all compiled files

2. **Rebuild:**
   - Press **⌘+B** to build (or **Product → Build**)
   - Wait for build to complete

3. **Run:**
   - Press **⌘+R** to run (or **Product → Run**)

## From Terminal:

```bash
cd "/Users/christopherkalla/Software Projects/NebulaEQ/Builds/MacOSX"

# Clean
xcodebuild -project MyVSTPlugin.xcodeproj -scheme "MyVSTPlugin - Standalone Plugin" -configuration Debug clean

# Build
xcodebuild -project MyVSTPlugin.xcodeproj -scheme "MyVSTPlugin - Standalone Plugin" -configuration Debug build

# Run
open build/Debug/MyVSTPlugin.app
```

## Why This Happens:

- Xcode sometimes uses cached builds
- The executable might not be updated if only some files changed
- Always clean before rebuilding after code changes

After cleaning and rebuilding, you should see the new space-themed NebulaEQ interface! 🚀


