#include "Application.h"
#include "imgui/imgui.h"
#include "LoggingSystem/LoggingSystem.h"


//Added the class LoggingSystem, which contains all the code for the logging system
//added its window in the main render loop
//added a test window to generate some log entries


namespace ClassGame
{
    //
    // our global variables
    //

    //
    // game starting point
    // this is called by the main render loop in main.cpp
    //
    void GameStartUp()
    {
        // Initialize logging system
        //            Logger& logger = Logger::GetInstance();
        //            logger.LogInfo("Game started successfully");
        //            logger.LogGameEvent("Application initialized");
    }

    //
    // game render loop
    // this is called by the main render loop in main.cpp
    //
    void RenderGame()
    {
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();

        ImGui::Begin("ImGui Log Demo");
        ImGui::LogButtons();

        if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
        {
            ImGui::LogToClipboard();
            ImGui::LogText("Hello, world!");
            ImGui::LogFinish();
        }
        ImGui::End();

        //the logging system window
        LoggingSystem::I().DrawWindow();


        //a testing window

        static bool show = true;
        ImGui::Begin("Test Window", &show);

        static int InfoCount = 0;
        static int WarningCount = 0;
        static int ErrorCount = 0;
        if (ImGui::Button("Test Info"))
        {
            InfoCount++;
            LoggingSystem::I().Log(LogLevel::Info, "This is your %d time trying to test the info level", InfoCount);
        }
        ImGui::SameLine();
        if (ImGui::Button("Test Warning"))
        {
            WarningCount++;
            LoggingSystem::I().Log(LogLevel::Warning, "This is your %d time trying to test the warning level", WarningCount);
        }
        ImGui::SameLine();
        if (ImGui::Button("Test Error"))
        {
            ErrorCount++;
            LoggingSystem::I().Log(LogLevel::Error, "This is your %d time trying to test the error level", ErrorCount);
        }   
        ImGui::End();

    }

    //
    // end turn is called by the game code at the end of each turn
    // this is where we check for a winner
    //
    void EndOfTurn()
    {
    }
}
