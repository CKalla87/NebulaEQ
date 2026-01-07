#!/bin/bash
# Plugin Packaging Script
# This script packages your plugin for distribution

set -e  # Exit on error

PLUGIN_NAME="MyVSTPlugin"
BUILD_DIR="Builds/MacOSX/build/Release"
DIST_BASE="dist"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Plugin Packaging Script ===${NC}"

# Get version (you can modify this)
read -p "Enter version number (e.g., 1.0.0): " VERSION
VERSION=${VERSION:-1.0.0}

DIST_DIR="${DIST_BASE}/${PLUGIN_NAME}_v${VERSION}"

# Check if Release build exists
if [ ! -d "${BUILD_DIR}" ]; then
    echo -e "${YELLOW}Warning: Release build not found at ${BUILD_DIR}${NC}"
    echo "Building Release version first..."
    
    cd Builds/MacOSX
    xcodebuild -project MyVSTPlugin.xcodeproj \
        -scheme "MyVSTPlugin - All" \
        -configuration Release \
        clean build
    cd ../..
fi

# Create distribution directory
echo "Creating distribution directory..."
rm -rf "${DIST_DIR}"
mkdir -p "${DIST_DIR}/macOS"

# Copy plugins
echo "Copying plugins..."

if [ -d "${BUILD_DIR}/${PLUGIN_NAME}.vst3" ]; then
    cp -R "${BUILD_DIR}/${PLUGIN_NAME}.vst3" "${DIST_DIR}/macOS/"
    echo "  ✓ VST3 copied"
else
    echo "  ✗ VST3 not found"
fi

if [ -d "${BUILD_DIR}/${PLUGIN_NAME}.component" ]; then
    cp -R "${BUILD_DIR}/${PLUGIN_NAME}.component" "${DIST_DIR}/macOS/"
    echo "  ✓ AU copied"
else
    echo "  ✗ AU not found"
fi

if [ -d "${BUILD_DIR}/${PLUGIN_NAME}.aaxplugin" ]; then
    cp -R "${BUILD_DIR}/${PLUGIN_NAME}.aaxplugin" "${DIST_DIR}/macOS/"
    echo "  ✓ AAX copied"
else
    echo "  ⚠ AAX not found (not built)"
fi

# Create README
cat > "${DIST_DIR}/README.txt" << EOF
${PLUGIN_NAME} v${VERSION}
========================

INSTALLATION INSTRUCTIONS
-------------------------

macOS:

1. VST3 Plugin:
   - Copy ${PLUGIN_NAME}.vst3 to:
     ~/Library/Audio/Plug-Ins/VST3/
   - Or for all users:
     /Library/Audio/Plug-Ins/VST3/

2. AU Plugin:
   - Copy ${PLUGIN_NAME}.component to:
     ~/Library/Audio/Plug-Ins/Components/
   - Or for all users:
     /Library/Audio/Plug-Ins/Components/

3. AAX Plugin (Pro Tools):
   - Copy ${PLUGIN_NAME}.aaxplugin to:
     ~/Library/Application Support/Avid/Audio/Plug-Ins/
   - Or for all users:
     /Library/Application Support/Avid/Audio/Plug-Ins/

4. After copying, restart your DAW.

SYSTEM REQUIREMENTS
-------------------
- macOS 10.13 or later
- Compatible DAW (Logic Pro, Ableton Live, Pro Tools, etc.)

SUPPORT
-------
For support, visit: www.example.com

Copyright © 2025 Your Company
EOF

echo "  ✓ README created"

# Create a simple install script
cat > "${DIST_DIR}/macOS/install.sh" << 'EOF'
#!/bin/bash
# Simple installation script

echo "Installing MyVSTPlugin..."

# Create directories if they don't exist
mkdir -p ~/Library/Audio/Plug-Ins/VST3
mkdir -p ~/Library/Audio/Plug-Ins/Components

# Install VST3
if [ -d "MyVSTPlugin.vst3" ]; then
    cp -R MyVSTPlugin.vst3 ~/Library/Audio/Plug-Ins/VST3/
    echo "✓ VST3 installed"
fi

# Install AU
if [ -d "MyVSTPlugin.component" ]; then
    cp -R MyVSTPlugin.component ~/Library/Audio/Plug-Ins/Components/
    echo "✓ AU installed"
fi

# Install AAX
if [ -d "MyVSTPlugin.aaxplugin" ]; then
    mkdir -p ~/Library/Application\ Support/Avid/Audio/Plug-Ins
    cp -R MyVSTPlugin.aaxplugin ~/Library/Application\ Support/Avid/Audio/Plug-Ins/
    echo "✓ AAX installed"
fi

echo ""
echo "Installation complete!"
echo "Please restart your DAW to use the plugin."
EOF

chmod +x "${DIST_DIR}/macOS/install.sh"
echo "  ✓ Install script created"

# Create DMG (optional, requires hdiutil)
read -p "Create DMG installer? (y/n): " CREATE_DMG
if [[ $CREATE_DMG =~ ^[Yy]$ ]]; then
    DMG_NAME="${PLUGIN_NAME}_v${VERSION}.dmg"
    echo "Creating DMG..."
    hdiutil create -volname "${PLUGIN_NAME} v${VERSION}" \
        -srcfolder "${DIST_DIR}" \
        -ov -format UDZO \
        "${DIST_BASE}/${DMG_NAME}"
    echo "  ✓ DMG created: ${DIST_BASE}/${DMG_NAME}"
fi

echo ""
echo -e "${GREEN}✓ Packaging complete!${NC}"
echo "Distribution package: ${DIST_DIR}"
echo ""
echo "Next steps:"
echo "1. Test the package on a clean machine"
echo "2. Code sign the plugins (recommended for macOS)"
echo "3. Upload to your distribution platform"

