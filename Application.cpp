#include "Application.h"
#include "Logger.h"
#include "imgui/imgui.h"
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

namespace ClassGame {
    
    static int gameActCounter = 0;
    static float floatVal = 0.0f;
    static ImVec4 clearColor = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

    static bool DemoWin = true;
    static bool AnotherWin = false;

    static float colorR = 115.0f / 255.0f;
    static float colorG = 140.0f / 255.0f;
    static float colorB = 153.0f / 255.0f;
    
    void GameStartUp() 
    {
        // Test Init Logs
        LOG_WARN("This is a test warning message");
        LOG_ERROR("This is a test error message");
        LOG_INFO("This is a test info message");
        LOG_INFO_TAG("Player made a move", "GAME");
        LOG_WARN_TAG("Invalid move attempted", "GAME");
        LOG_ERROR_TAG("Game state corrupted", "GAME");
        
        gameActCounter = 0;
        floatVal = 0.0f;
        DemoWin = true;
        AnotherWin = false;
    }

    void RenderGame() 
    {
        ImGui::DockSpaceOverViewport();
        if (DemoWin) {
            ImGui::ShowDemoWindow(&DemoWin);
        }

        // Window #1
        ImGui::Begin("ImGui Log Demo");
        ImGui::LogButtons();

        if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
        {
            ImGui::LogToClipboard();
            ImGui::LogText("Hello, world!");
            ImGui::LogFinish();
        }
        ImGui::End();

        // Window #2
        ImGui::Begin("Game Log");

        ImGui::Button("Options");

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

        const auto& entries = Logger::GetInstance().GetEntries();
        const auto& colors = Logger::GetInstance().GetColors();
        
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), true);
        for (size_t i = 0; i < entries.size(); i++) {
            ImGui::PushStyleColor(ImGuiCol_Text, colors[i]);
            ImGui::Text("%s", entries[i].c_str());
            ImGui::PopStyleColor();
        }
        
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        
        ImGui::EndChild();
        
        ImGui::End();

        // Window #3
        ImGui::Begin("Game Control");
        ImGui::Text("Main Game Control Panel");

        ImGui::Checkbox("##DemoCheck", &DemoWin);
        ImGui::SameLine();
        ImGui::Text("Demo Window");

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
            LOG_INFO_TAG("Game action performed, counter: ", "GAME");
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
    }

    void EndOfTurn() 
    {
        gameActCounter++;
        LOG_INFO_TAG("End of turn #" + std::to_string(gameActCounter), "GAME");
    }
}