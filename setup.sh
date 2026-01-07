#!/bin/bash

# Setup script for VST Plugin Project
# This script initializes the JUCE submodule

echo "Setting up VST Plugin Project..."
echo ""

# Check if git is available
if ! command -v git &> /dev/null; then
    echo "Error: git is not installed. Please install git first."
    exit 1
fi

# Initialize and update submodules
echo "Initializing JUCE submodule..."
git submodule update --init --recursive

if [ $? -eq 0 ]; then
    echo ""
    echo "Setup complete!"
    echo ""
    echo "Next steps:"
    echo "1. Open MyVSTPlugin.jucer in JUCE Projucer"
    echo "2. Click 'Save and Open in IDE' to generate Xcode/Visual Studio project"
    echo "3. Build the project in your IDE"
    echo ""
    echo "Alternatively, if you have CMake:"
    echo "  cmake -B build"
    echo "  cmake --build build"
else
    echo ""
    echo "Error: Failed to initialize submodules."
    echo "Make sure you're in a git repository or clone with:"
    echo "  git clone --recursive <your-repo-url>"
    exit 1
fi

