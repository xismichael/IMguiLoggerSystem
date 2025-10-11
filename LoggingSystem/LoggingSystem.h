#pragma once
#include <string>
#include <vector>
#include <cstdarg>
#include <chrono>

//Log Levels, could be extended later
enum class LogLevel { Info, Warning, Error };


struct LogEntry {
    //timer tracker that starts when the UI opens
    uint64_t time_ms;

    //Log Level
    LogLevel level;

    //cap size for message
    static constexpr size_t kMax = 512;
    
    //actual text of the log
    char text[kMax];
};

class LoggingSystem {
public:

    //singleton instance
    static LoggingSystem& I();

    //for logging
    void Log(LogLevel level, const char* message, ...);
    void LogV(LogLevel level, const char* message, va_list args);

    //for opening and clearing window
    void Clear();
    void DrawWindow();
        
private:
    LoggingSystem();

    //start the build in timer
    uint64_t NowMs() const;

    std::vector<LogEntry> buf_;
    bool auto_scroll_ = true;

    //for storing when the log started
    std::chrono::steady_clock::time_point t0_;

    bool show_info_ = true;
    bool show_warning_ = true;
    bool show_error_ = true;

    //for saving to the debug log

    void SaveAllToFile_(const char* path);
    char save_path_[260] = "gameLog.txt";


};
