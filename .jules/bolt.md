# Bolt's Journal - Critical Learnings

## 2025-05-18 - String reallocation overhead and fragmentation in embedded string routines
**Learning:** In microcontroller and embedded environments running Arduino core on ESP32, dynamic `String` concatenations inside loops without pre-allocating buffer capacity cause repeated `realloc` calls, leading to heap fragmentation and runtime overhead on every keystroke in interactive components like virtual keyboards (`mykeyboard.cpp`).
**Action:** When creating strings with known or easily calculable final lengths, always call `.reserve()` or construct with the known size to minimize heap allocations and fragmentation.
