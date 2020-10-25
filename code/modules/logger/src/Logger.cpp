#include "Logger.h"
#include "cstdio"

Logger::Logger(int inLevel) {
    level = inLevel;
}

template <typename ...Args>
void Logger::log(Type type, const char* fmt, const Args&... args) {
    printf("%d::", static_cast<int>(type));
    printf(fmt, args...);
    // TODO
}

template <typename ...Args>
void Logger::fatal(const char* fmt, const Args&... args) {
    log(Type::FATAL, fmt, args...);
}

template <typename ...Args>
void Logger::error(const char* fmt, const Args&... args) {
    log(Type::ERROR, fmt, args...);
}

template <typename ...Args>
void Logger::warning(const char* fmt, const Args&... args) {
    log(Type::WARNING, fmt, args...);
}

template <typename ...Args>
void Logger::info(const char* fmt, const Args&... args) {
    log(Type::INFO, fmt, args...);
}

template <typename ...Args>
void Logger::debug(const char* fmt, const Args&... args) {
    log(Type::DEBUG, fmt, args...);
}