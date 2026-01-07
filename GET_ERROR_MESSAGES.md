# Getting Error Messages from Xcode

To help debug the errors, please:

1. **Look at the Xcode Console/Output Panel:**
   - At the bottom of Xcode, make sure the **Console** or **All Output** tab is visible
   - Scroll through the messages
   - Look for lines in **red** or with "error", "EXC", "crash", "assertion", etc.

2. **Copy the Error Messages:**
   - Select the error text
   - Copy it (⌘+C)
   - Paste it here

3. **Common Error Types to Look For:**
   - Lines starting with "EXC_" (crash reports)
   - Lines with "assertion failed" or "jassert"
   - Lines with "error:" or "Error:"
   - Stack traces (lists of function names and file paths)

4. **The "Message Thread" Line:**
   - "JUCE v8.0.12: Message Thread (1) Queue : com.apple.main-thread (serial)"
   - This is usually **informational logging**, not an error
   - Look for actual error messages **after** this line

Please share the actual error messages so I can help fix them!


