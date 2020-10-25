#include "Logger.h"
#include "ctime"
#include "cstring"

#define LOGGER_LOG(type, file, format) \
    startColoredText(type); \
    printTag(file, type); \
    va_list argptr; \
    va_start(argptr, format); \
    vfprintf(file, format, argptr); \
    va_end(argptr); \
    fprintf(file, "\n"); \
    endColoredText();

Logger Logger::logger;

Logger::Logger(int inLevel) {
    level = inLevel;
}

const char* Logger::getTypeString(Type type) {
    switch (type)
    {
    case Type::FATAL:
        return "Fatal";
    case Type::ERROR:
        return "Error";
    case Type::WARNING:
        return "Warning";
    case Type::INFO:
        return "Info";
    case Type::DEBUG:
        return "Debug";
    }
}

void Logger::startColoredText(Type type) {
    switch (type)
    {
    case Type::FATAL:
    case Type::ERROR:
        printf("\033[1;31m");
        break;
    case Type::WARNING:
        printf("\033[1;33m");
        break;
    case Type::INFO:
        printf("\033[1;32m");
        break;
    case Type::DEBUG:
        printf("\033[1;34m");
        break;
    }
}

void Logger::endColoredText() {
    printf("\033[0m");
}

void Logger::printTag(FILE* file, Type type) {
    time_t now = time(0);
    char* dt = ctime(&now);
    int len = strlen(dt);
    dt[len-1] = '\0';  // 去掉换行符
    printf("%s %s:: ", dt, getTypeString(type));
}

void Logger::fatal(const char* fmt, ...) {
    LOGGER_LOG(Type::FATAL, stdout, fmt);
}

void Logger::error(const char* fmt, ...) {
    LOGGER_LOG(Type::ERROR, stdout, fmt);
}

void Logger::warning(const char* fmt, ...) {
    LOGGER_LOG(Type::WARNING, stdout, fmt);
}

void Logger::info(const char* fmt, ...) {
    LOGGER_LOG(Type::INFO, stdout, fmt);
}

void Logger::debug(const char* fmt, ...) {
    LOGGER_LOG(Type::DEBUG, stdout, fmt);
}