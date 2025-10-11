#include "LoggingSystem.h"
#include "../imgui/imgui.h"
#include <cstdio>
#include <cstring>


//to get level naem
static const char* LevelName(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
    }
    return "?";
}

//singleton instance
LoggingSystem& LoggingSystem::I() {
    static LoggingSystem inst;
    return inst;
}

LoggingSystem::LoggingSystem() {
    t0_ = std::chrono::steady_clock::now();
}


//give the elapsed time in ms since start
uint64_t LoggingSystem::NowMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - t0_
           ).count();
}

void LoggingSystem::Log(LogLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    LogV(level, fmt, args);
    va_end(args);
}

void LoggingSystem::LogV(LogLevel level, const char* fmt, va_list args) {
    char tmp[2048];
    vsnprintf(tmp, sizeof(tmp), fmt, args);

    LogEntry e{};
    e.time_ms = NowMs();
    e.level   = level;

    std::snprintf(e.text, LogEntry::kMax, "%s", tmp);

    buf_.push_back(e);
}

void LoggingSystem::Clear() {
    buf_.clear();
}


void LoggingSystem::SaveAllToFile_(const char* path) {
    FILE* f = std::fopen(path, "w");
    if (!f) return;

    for (const auto& e : buf_) {
        // for calculating time stamp
        uint64_t ms = e.time_ms % 1000;
        uint64_t s  = (e.time_ms / 1000) % 60;
        uint64_t m  = (e.time_ms / (1000*60)) % 60;
        uint64_t h  = (e.time_ms / (1000*60*60));
        char stamp[32];
        std::snprintf(stamp, sizeof(stamp), "[%02llu:%02llu:%02llu.%03llu]",
                      (unsigned long long)h, (unsigned long long)m,
                      (unsigned long long)s, (unsigned long long)ms);

        std::fprintf(f, "[%s] [%s] %s\n", stamp, LevelName(e.level), e.text);
    }

    std::fclose(f);
}

static ImVec4 ColorFor(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::Info: return ImGui::GetStyleColorVec4(ImGuiCol_Text);
        case LogLevel::Warning: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        case LogLevel::Error: return ImVec4(1.0f, 0.35f, 0.35f, 1.0f);
    }
    return ImGui::GetStyleColorVec4(ImGuiCol_Text);
}

void LoggingSystem::DrawWindow() {
    if (!ImGui::Begin("Logging System")) { ImGui::End(); return; }

    ImGui::Checkbox("Info", &show_info_); 
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &show_warning_);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &show_error_);
    ImGui::SameLine();
    ImGui::Checkbox("Auto Scroll", &auto_scroll_);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    if (ImGui::Button("Save")) SaveAllToFile_(save_path_);
    ImGui::SameLine();
    ImGui::InputText("##savepath", save_path_, IM_ARRAYSIZE(save_path_));

    ImGui::Separator();

    ImGui::BeginChild("log_scroller", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()),
                      false, ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& e : buf_) {
        // filter by level
        if ((e.level == LogLevel::Info && !show_info_)||
            (e.level == LogLevel::Warning && !show_warning_)||
            (e.level == LogLevel::Error && !show_error_)) {
            continue;
        }

        // for calculating time stamp
        uint64_t ms = e.time_ms % 1000;
        uint64_t s = (e.time_ms / 1000) % 60;
        uint64_t m = (e.time_ms / (1000*60)) % 60;
        uint64_t h = (e.time_ms / (1000*60*60));
        char stamp[32];
        std::snprintf(stamp, sizeof(stamp), "[%02llu:%02llu:%02llu.%03llu]",
                      (unsigned long long)h, (unsigned long long)m,
                      (unsigned long long)s, (unsigned long long)ms);

        ImGui::PushStyleColor(ImGuiCol_Text, ColorFor(e.level));
        ImGui::Text("[%s] [%s] %s", stamp, LevelName(e.level), e.text);
        ImGui::PopStyleColor();
    }


    //for auto scrolling, chat helped lol idk how this works
    if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}