# How to Delete Old Plugin from Ableton

Ableton uses the system plugin folders. Here's how to delete the old "MyVSTPlugin":

## Method 1: Using Finder (Easiest)

1. **Open Finder**

2. **Press ⌘+Shift+G** (Go to Folder)

3. **Navigate to VST3 folder:**
   - Type: `~/Library/Audio/Plug-Ins/VST3`
   - Press Enter
   - Look for `MyVSTPlugin.vst3` folder
   - **Delete it** (drag to Trash or right-click → Move to Trash)

4. **Navigate to AU folder:**
   - Press ⌘+Shift+G again
   - Type: `~/Library/Audio/Plug-Ins/Components`
   - Press Enter
   - Look for `MyVSTPlugin.component` folder
   - **Delete it**

5. **Empty Trash** (optional, but recommended)

6. **Rescan plugins in Ableton:**
   - Open Ableton Live
   - Go to **Preferences → Plug-ins**
   - Click **"Rescan"** or **"Rescan Plug-ins"**

## Method 2: Using Terminal

```bash
# Delete VST3 version
rm -rf ~/Library/Audio/Plug-Ins/VST3/MyVSTPlugin.vst3

# Delete AU version
rm -rf ~/Library/Audio/Plug-Ins/Components/MyVSTPlugin.component
```

## Important Notes:

- **The `~` symbol** means your home folder (`/Users/christopherkalla`)
- **VST3 plugins** are folders (even though they end in .vst3)
- **AU plugins** are also folders (ending in .component)
- You need to delete the **entire folder**, not just files inside

## After Deleting:

1. Rebuild your plugin with the new name (NebulaEQ)
2. Copy the new plugin to the same folders
3. Rescan in Ableton
4. The plugin should now appear as "NebulaEQ" instead of "MyVSTPlugin"

