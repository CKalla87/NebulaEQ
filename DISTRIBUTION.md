# Plugin Distribution Guide

This guide explains how to package and distribute your VST plugin for use on other machines.

## 1. Build Release Version

**Important:** Always build in **Release** mode for distribution (optimized, smaller, faster).

### In Xcode:
1. Select the scheme (e.g., "MyVSTPlugin - VST3", "MyVSTPlugin - AU", etc.)
2. Change build configuration from "Debug" to **"Release"**
   - Product → Scheme → Edit Scheme → Run → Build Configuration → Release
3. Press ⌘B to build
4. The Release build will be in: `Builds/MacOSX/build/Release/`

### From Command Line:
```bash
cd Builds/MacOSX
xcodebuild -project MyVSTPlugin.xcodeproj \
  -scheme "MyVSTPlugin - VST3" \
  -configuration Release
```

## 2. Plugin Formats and Installation Locations

### macOS:

| Format | File Extension | Installation Location |
|--------|---------------|----------------------|
| **VST3** | `.vst3` | `~/Library/Audio/Plug-Ins/VST3/` (user)<br>`/Library/Audio/Plug-Ins/VST3/` (system) |
| **AU** | `.component` | `~/Library/Audio/Plug-Ins/Components/` (user)<br>`/Library/Audio/Plug-Ins/Components/` (system) |
| **AAX** | `.aaxplugin` | `~/Library/Application Support/Avid/Audio/Plug-Ins/` (user)<br>`/Library/Application Support/Avid/Audio/Plug-Ins/` (system) |
| **Standalone** | `.app` | Anywhere (like a regular app) |

### Windows:

| Format | File Extension | Installation Location |
|--------|---------------|----------------------|
| **VST3** | `.vst3` | `C:\Program Files\Common Files\VST3\` |
| **AAX** | `.aaxplugin` | `C:\Program Files\Common Files\Avid\Audio\Plug-Ins\` |

### Linux:

| Format | File Extension | Installation Location |
|--------|---------------|----------------------|
| **VST3** | `.vst3` | `~/.vst3/` or `/usr/lib/vst3/` |
| **LV2** | `.lv2` | `~/.lv2/` or `/usr/lib/lv2/` |

## 3. What Files to Distribute

### For Each Plugin Format:
- **VST3**: The entire `.vst3` bundle (it's a folder, but macOS treats it as a single file)
- **AU**: The entire `.component` bundle
- **AAX**: The entire `.aaxplugin` bundle
- **Standalone**: The `.app` bundle

### Example Distribution Package Structure:
```
MyVSTPlugin_v1.0/
├── macOS/
│   ├── MyVSTPlugin.vst3
│   ├── MyVSTPlugin.component
│   └── README.txt
├── Windows/
│   ├── MyVSTPlugin.vst3
│   └── README.txt
└── README.md
```

## 4. Code Signing (macOS - Recommended)

For macOS distribution, code signing is **highly recommended** (and required for notarization):

```bash
# Sign the plugin
codesign --force --deep --sign "Developer ID Application: Your Name" \
  Builds/MacOSX/build/Release/MyVSTPlugin.vst3

# Verify signature
codesign --verify --verbose Builds/MacOSX/build/Release/MyVSTPlugin.vst3
```

### Notarization (for distribution outside App Store):
1. Create a `.zip` of your signed plugin
2. Submit to Apple: `xcrun notarytool submit MyVSTPlugin.vst3.zip --keychain-profile "AC_PASSWORD"`
3. Staple the ticket: `xcrun stapler staple MyVSTPlugin.vst3`

## 5. Creating Installers

### macOS - Create a DMG:
```bash
# Create a disk image
hdiutil create -volname "MyVSTPlugin Installer" \
  -srcfolder MyVSTPlugin_v1.0 \
  -ov -format UDZO MyVSTPlugin_v1.0.dmg
```

### macOS - Using Packages.app:
1. Download Packages.app (free)
2. Create a new distribution package
3. Add your plugins to appropriate locations
4. Build installer

### Windows - Using Inno Setup or NSIS:
- Create an installer that copies files to the correct locations
- Add uninstaller support

## 6. Quick Distribution Script

Create a script to package everything:

```bash
#!/bin/bash
# package_plugin.sh

VERSION="1.0"
PLUGIN_NAME="MyVSTPlugin"
BUILD_DIR="Builds/MacOSX/build/Release"
DIST_DIR="dist/${PLUGIN_NAME}_v${VERSION}"

# Create distribution directory
mkdir -p "${DIST_DIR}/macOS"

# Copy plugins
cp -R "${BUILD_DIR}/${PLUGIN_NAME}.vst3" "${DIST_DIR}/macOS/"
cp -R "${BUILD_DIR}/${PLUGIN_NAME}.component" "${DIST_DIR}/macOS/"

# Create README
cat > "${DIST_DIR}/README.txt" << EOF
MyVSTPlugin v${VERSION}

Installation:
- VST3: Copy MyVSTPlugin.vst3 to ~/Library/Audio/Plug-Ins/VST3/
- AU: Copy MyVSTPlugin.component to ~/Library/Audio/Plug-Ins/Components/

Restart your DAW after installation.
EOF

echo "Packaged to: ${DIST_DIR}"
```

## 7. Testing on Other Machines

### Before Distribution:
1. **Test on a clean machine** (without development tools)
2. **Test in multiple DAWs** (Logic, Ableton, Pro Tools, etc.)
3. **Test on different macOS versions** (if possible)
4. **Verify all plugin formats work**

### Common Issues:
- **Missing dependencies**: Ensure all required libraries are included
- **Code signing errors**: Sign your plugins properly
- **Permission issues**: Users may need to allow unsigned plugins in System Preferences

## 8. Version Information

Update version info in Projucer:
1. Open `MyVSTPlugin.jucer` in Projucer
2. Set version number in project settings
3. This will be embedded in the plugin

## 9. Documentation

Include with your distribution:
- **README.txt**: Installation instructions
- **User Manual**: How to use the plugin
- **System Requirements**: Minimum macOS/Windows versions, DAW compatibility
- **Changelog**: What's new in each version

## 10. Distribution Methods

- **Direct Download**: Host on your website
- **GitHub Releases**: Free hosting for open source projects
- **Plugin Marketplaces**: Plugin Boutique, KVR Audio, etc.
- **Your Own Website**: Full control over distribution

## Quick Reference Commands

```bash
# Build Release version
cd Builds/MacOSX
xcodebuild -project MyVSTPlugin.xcodeproj \
  -scheme "MyVSTPlugin - All" \
  -configuration Release

# Copy to user plugin directory
cp -R build/Release/MyVSTPlugin.vst3 ~/Library/Audio/Plug-Ins/VST3/
cp -R build/Release/MyVSTPlugin.component ~/Library/Audio/Plug-Ins/Components/

# Create distribution package
mkdir -p dist
cp -R build/Release/MyVSTPlugin.vst3 dist/
cp -R build/Release/MyVSTPlugin.component dist/
```

