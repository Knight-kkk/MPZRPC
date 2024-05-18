#pragma once
#include "logqueue.hpp"
#include <string>
#include <stdarg.h>

#define LOG_INFO(format, ...) \
    do{ \
        Logger& logger = Logger::instance(); \
        logger.setLogLevel(INFO); \
        char buffer[1024] = {0};    \
        snprintf(buffer, 1024, format, ##__VA_ARGS__); \
        logger.Log(buffer); \
    } while(0) \

#define LOG_ERROR(format, ...) \
    do{ \
        Logger& logger = Logger::instance(); \
        logger.setLogLevel(ERROR); \
        char buffer[1024] = {0};    \
        snprintf(buffer, 1024, format, ##__VA_ARGS__); \
        logger.Log(buffer); \
    } while(0) \

enum LogLevel{
    INFO, // 普通日志
    ERROR, // 错误日志
};

class Logger{
public:
    void setLogLevel(LogLevel level);
    void Log(const std::string& logMsg);
    static Logger& instance();

private:
    LogQueue<std::string> _logQueue;
    LogLevel _level;
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};


  