# VST Plugin Project

This is a JUCE-based VST plugin project based on the FreeCodeCamp tutorial.

## Setup Instructions

### Prerequisites
- JUCE Framework (will be cloned as a submodule)
- Xcode (for macOS) or Visual Studio (for Windows)
- JUCE Projucer (optional, for editing .jucer files)

### Getting Started

1. **Clone the repository with submodules:**
   ```bash
   git clone --recursive <your-repo-url>
   ```
   
   Or if you've already cloned:
   ```bash
   git submodule update --init --recursive
   ```

2. **Open the project:**
   - **Option A (Recommended):** Open `MyVSTPlugin.jucer` in JUCE Projucer and click "Save and Open in IDE"
   - **Option B:** Use CMake (if you have a CMakeLists.txt):
     ```bash
     cmake -B build
     cmake --build build
     ```

3. **Build the plugin:**
   - In Xcode: Select the "MyVSTPlugin - All" scheme and build
   - In Visual Studio: Build the solution

4. **Install the plugin:**
   - macOS: Copy the `.component` (AU) or `.vst3` file to:
     - `~/Library/Audio/Plug-Ins/Components/` (AU)
     - `~/Library/Audio/Plug-Ins/VST3/` (VST3)
   - Windows: Copy the `.vst3` file to:
     - `C:\Program Files\Common Files\VST3\`

## Project Structure

```
.
├── Source/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   └── PluginEditor.cpp
├── JUCE/              (submodule)
├── MyVSTPlugin.jucer  (JUCE project file)
└── README.md
```

## Resources

- [JUCE Tutorials](https://juce.com/learn/tutorials)
- [FreeCodeCamp VST Plugin Tutorial](https://www.youtube.com/watch?v=G4A5T3y5bJM)
- [Project13 Repository](https://github.com/matkatmusic/Project13_FreeCodeCamp)

