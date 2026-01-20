# Game Logging System | CMPM123

C++ logger with ImGui. Outputs color-coded logs (INFO/WARN/ERROR) to both `game_log.txt` and ImGui console with timestamps and optional tags. Utilized Meyer's Singleton accessible via macros: `LOG_INFO()`, `LOG_WARN()`, `LOG_ERROR()`, etc.

---
## Citations & References

1. [ImGui](https://github.com/ocornut/imgui/tree/docking)
2. [Meyer's Singleton](https://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html)
3. [ImGui Console Usage](https://github.com/rmxbalanque/imgui-console/blob/master/example/src/example_main.cpp)

---
## Approach & Questions

### Approach

1. Surfed imgui.cpp and documentation.
2. Did the getting started with the DEMO IMGUI window code.
3. Went through dropdown and tables available.
4. Started creating other windows with button placeholders.
5. Created Logger class to link logic to the pre-made buttons & UI.
6. Look into the 'Singleton' referred in the class Discord - found Scott Meyer's example.

### Questions

1. For planning sake, how long should the assignment take and which files should be the main reference?
    - Should take an hour or two, planned to read documentation and go down the imgui rabbithole to find extent of capabilities. I also centered focus onto the MISC/HELPERS Utilities Section and LOGGING/CAPTURING in imgui.cpp
2. How to fix version issues in CMakeLists.txt?
    - Changed to most recent version
3. How to add a Logger class and change the CMakeLists.txt to build correctly?
    - Initialized class and added the files to add_executable in CMakeLists.txt
4. What is the functionality of certain UI and the command input of the Game Log window?
    - Learned of console commands for old games which configured variables for certain outputs. Reference link provided by Professor Devine -> [Commands](https://quake.fandom.com/wiki/Console_Commands_(Q3)).