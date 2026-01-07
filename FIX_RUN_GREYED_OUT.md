# Fix: Product → Run is Greyed Out

When "Run" is greyed out in Xcode, it usually means:

## 1. Check the Scheme Selection

At the top of Xcode, next to the play button, there's a dropdown that shows the scheme and destination.

- **Make sure "MyVSTPlugin - Standalone Plugin" is selected**
- The destination should show "My Mac" or similar

If it shows "Any Mac" or something else, click the scheme dropdown and select:
- **"MyVSTPlugin - Standalone Plugin"** scheme
- Destination: **"My Mac"**

## 2. Build First

Before you can run, you need to build successfully:

- Press **⌘+B** (or **Product → Build**)
- Wait for the build to complete
- Check for any build errors in the Issue Navigator (left sidebar, triangle icon)

If there are build errors, fix them first. The Run button will only be enabled after a successful build.

## 3. Check the Issue Navigator

- Click the **Issue Navigator** icon in the left sidebar (triangle with exclamation mark)
- Look for any red error messages
- If there are errors, we need to fix them before running

## 4. Try Building Again

Sometimes Xcode needs a clean build:

1. **Clean**: Press **Shift+⌘+K** (Product → Clean Build Folder)
2. **Build**: Press **⌘+B** (Product → Build)
3. **Wait** for "Build Succeeded" message
4. **Run**: Press **⌘+R** (should now be enabled)

## Common Issues:

- **No scheme selected**: Select the scheme from the dropdown
- **Build failed**: Check the Issue Navigator for errors
- **No destination**: Make sure "My Mac" is selected as the destination
- **Xcode needs restart**: Sometimes quitting and reopening Xcode helps

Let me know what you see in the Issue Navigator or if the build succeeds!


