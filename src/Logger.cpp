#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init() {
    if (!logger) {
        logger = spdlog::basic_logger_mt("file_logger", "logs/app.log");
        spdlog::set_level(spdlog::level::info);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    if (!logger) {
        init();
    }
    return logger;
}

// Wrapper methods
void Logger::LogInfo(const std::string& message) {
    getLogger()->info(message);
}

void Logger::LogWarn(const std::string& message) {
    getLogger()->warn(message);
}

void Logger::LogError(const std::string& message) {
    getLogger()->error(message);
}

void Logger::LogDebug(const std::string& message) {
    getLogger()->debug(message);
}
