#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <string>

class Logger {
public:
    static void init();
    static std::shared_ptr<spdlog::logger> getLogger();

    static void LogInfo(const std::string& message);
    static void LogWarn(const std::string& message);
    static void LogError(const std::string& message);
    static void LogDebug(const std::string& message);

    class FunctionLogger {
    public:
        explicit FunctionLogger(const char* functionName)
            : m_functionName(functionName) {
            LogInfo("Entering function ----> " + m_functionName);
        }

        ~FunctionLogger() {
            LogInfo("Exiting function <---- " + m_functionName);
        }

    private:
        std::string m_functionName;
    };

private:
    static std::shared_ptr<spdlog::logger> logger;
};

#define LOG_FUNCTION() Logger::FunctionLogger functionLogger(__FUNCTION__)

#endif // LOGGER_H
