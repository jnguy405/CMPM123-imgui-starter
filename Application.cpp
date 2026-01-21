/*
 * ASSIGNMENT: W26 CMPM123 - Build a Logging System & adjusted makefile for a Dear IMGUI project
 * 
 * CHANGES AND ADDITIONS:
 * 
 * Logger.h: Defines Logger singleton class with Meyer's pattern and provides Info(), Warning(), Error() methods with tags and macros.
 * 
 * Logger.cpp: Implements logging functionality with timestamp formatting, file output to game_log.txt, and color-coded entries for ImGui display.
 * 
 * Application.cpp Usage:
 * Added "Game Log" window to display Logger entries with color coding and auto-scroll.
 * Uses LOG_INFO, LOG_WARN, LOG_ERROR macros throughout for testing and game events.
 * Integrated Logger::GetInstance() calls for clearing logs and retrieving entries for display.
 * Command Line Interface: refer to imgui_demo.cpp for command input handling, history navigation, and command execution example.
 * 
 * CMakeLists.txt Changes:
 * Added Logger.h and Logger.cpp to source files list for compilation.
 */
#include "Application.h"
#include "Logger.h"
#include "Command.h"
#include "imgui/imgui.h"
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

namespace ClassGame {
    
    // "Game Control" Window defaults
    static int gameActCounter = 0;                              // Track player actions by count
    static float floatVal = 0.0f;                               // Displays designated float val on slider
    static ImVec4 clearColor = ImVec4(0.5f, 0.5f, 0.5f, 1.00f); // Hex picker default

    static bool DemoWin = true;
    static bool LogWin = true;
    static bool AnotherWin = false;

    // Default hex color (clear)
    static float colorR = 115.0f / 255.0f;
    static float colorG = 140.0f / 255.0f;
    static float colorB = 153.0f / 255.0f;
    
    // Command line input buffer and history
    static char InputBuf[256] = "";
    
    void ResetGameCounter() {
        gameActCounter = 0;
    }

    void GameStartUp() {

        // Initialize Logger
        Logger::GetInstance().Init();

        // Test log entry types/tags
        LOG_WARN("This is a test warning message");
        LOG_ERROR("This is a test error message");
        LOG_INFO("This is a test info message");
        LOG_INFO_TAG("Player made a move", "GAME");
        LOG_WARN_TAG("Invalid move attempted", "GAME");
        LOG_ERROR_TAG("Game state corrupted", "GAME");
        
        // Initialize control variables
        gameActCounter = 0;
        floatVal = 0.0f;
        DemoWin = true;
        LogWin = true;
        AnotherWin = false;
    }

    void RenderGame() {
        
        // Relies on DemoWin boolean - Needs checkbox to view
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();


        // Window #1 - ImGui Log Demo
        if (DemoWin) { 
            ImGui::Begin("ImGui Log Demo", &DemoWin);
            ImGui::LogButtons();

            if (ImGui::Button("Copy \"Hello, world!\" to clipboard")){
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }
            ImGui::End();
        }

        // Window #2 - Game Log with Command Line
        if (LogWin) {
            ImGui::Begin("Game Log", &LogWin);

            // Filter state variables
            static bool showInfo = true;
            static bool showWarning = true;
            static bool showError = true;

            // Options button and popup
            if (ImGui::Button("Options")) {
                ImGui::OpenPopup("OptionsPopup");
            }

            if (ImGui::BeginPopup("OptionsPopup")) {
                ImGui::Text("Filter Options");
                ImGui::Separator();
                
                ImGui::Checkbox("Show Info", &showInfo);
                ImGui::Checkbox("Show Warnings", &showWarning);
                ImGui::Checkbox("Show Errors", &showError);
                
                ImGui::Separator();
                
                if (ImGui::MenuItem("Clear Log")) {
                    Logger::GetInstance().Clear();
                }
                
                ImGui::EndPopup();
            }

            // Test buttons
            ImGui::SameLine();
            if (ImGui::Button("Clear")) {
                Logger::GetInstance().Clear();
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Info")) {
                LOG_INFO("This is a test info message");
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Warning")) {
                LOG_WARN("This is a test warning message");
            }
            ImGui::SameLine();
            if (ImGui::Button("Test Error")) {
                LOG_ERROR("This is a test error message");
            }
            ImGui::Separator();

            // Display log entries with filtering
            const auto& entries = Logger::GetInstance().GetEntries();
            const auto& colors = Logger::GetInstance().GetColors();
            
            const float footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footer_height), true);
            
            for (size_t i = 0; i < entries.size(); i++) {
                bool display = true;
                
                if (entries[i].find("[INFO]") != std::string::npos && !showInfo) { 
                    display = false; 
                }
                else if (entries[i].find("[WARN]") != std::string::npos && !showWarning) { 
                    display = false; 
                }
                else if (entries[i].find("[ERROR]") != std::string::npos && !showError) { 
                    display = false; 
                }
                
                if (display) {
                    ImGui::PushStyleColor(ImGuiCol_Text, colors[i]);
                    ImGui::Text("%s", entries[i].c_str());
                    ImGui::PopStyleColor();
                }
            }
            
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();

            // Command line input
            ImGui::Separator();
            bool reclaim_focus = false;

            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | 
                                                   ImGuiInputTextFlags_EscapeClearsAll | 
                                                   ImGuiInputTextFlags_CallbackHistory;
            
            if (ImGui::InputText("##CommandInput", InputBuf, IM_ARRAYSIZE(InputBuf), 
                                input_text_flags, &Command::TextEditCallbackStub)) {
                char* s = InputBuf;
                Command::Strtrim(s);
                if (s[0])
                    Command::ExecCommand(s);
                strcpy_s(s, IM_ARRAYSIZE(InputBuf), "");
                reclaim_focus = true;
            }
            
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus)
                ImGui::SetKeyboardFocusHere(-1);

            ImGui::SameLine();
            ImGui::Text("Command");

            ImGui::SameLine();
            if (ImGui::Button("Help")) {
                LOG_INFO_TAG("Available commands: CLEAR, HELP, INFO, WARN, ERROR, RESET", "CMD");
            }

            ImGui::End();
        }

        // Window #3 - Game Control
        ImGui::Begin("Game Control");
        ImGui::Text("Main Game Control Panel");

        ImGui::Checkbox("##DemoCheck", &DemoWin);
        ImGui::SameLine();
        ImGui::Text("Demo Window");

        ImGui::Checkbox("##LogCheck", &LogWin);
        ImGui::SameLine();
        ImGui::Text("Log Window");

        ImGui::Checkbox("##AnotherCheck", &AnotherWin);
        ImGui::SameLine();
        ImGui::Text("Another Window");

        ImGui::SliderFloat("##float", &floatVal, 0.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        ImGui::Text("float");

        // Color edit control
        float colorArray[3] = { colorR, colorG, colorB };
        if (ImGui::ColorEdit3("clear color", colorArray)) {
            colorR = colorArray[0];
            colorG = colorArray[1];
            colorB = colorArray[2];
            clearColor = ImVec4(colorR, colorG, colorB, 1.0f);
        }

        if (ImGui::Button("Game Action")) {
            gameActCounter++;
            std::string msg = "Game action performed, counter: " + std::to_string(gameActCounter);
            LOG_INFO_TAG(msg, "GAME");
        }
        ImGui::SameLine();
        ImGui::Text("counter: %d", gameActCounter);

        ImGui::Separator();
        ImGui::Text("Logging Test Buttons:");

        if (ImGui::Button("Log Game Event")) {
            LOG_INFO_TAG("Manual game event from control panel", "GAME");
        }
        ImGui::SameLine();
        if (ImGui::Button("Log Warnings")) {
            LOG_WARN("Manual warning from control panel");
        }
        ImGui::SameLine();
        if (ImGui::Button("Log Error")) {
            LOG_ERROR("Manual error from control panel");
        }

        ImGui::End();

        // Window #4 - Another Window
        if (AnotherWin) { 
            ImGui::Begin("Another Window", &AnotherWin);
            ImGui::Text("Hello from another window!");
            ImGui::End();
        }
    }

    void EndOfTurn() {
        gameActCounter++;
        LOG_INFO_TAG("End of turn #" + std::to_string(gameActCounter), "GAME");
    }
}