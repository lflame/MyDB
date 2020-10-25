#ifndef _LOGGER_H_
#define _LOGGER_H_

class Logger {
  public:
    enum class Type {
        FATAL = 1,
        ERROR = 2,
        WARNING = 3,
        INFO = 4,
        DEBUG = 5
    };
    // 小于等于此 level 的类型都需要输出
    int level;

    Logger(int inLevel = 5);

    template <typename ...Args>
    void log(Type type, const char* fmt, const Args&... args);

    template <typename ...Args>
    void fatal(const char* fmt, const Args&... args);
    template <typename ...Args>
    void error(const char* fmt, const Args&... args);
    template <typename ...Args>
    void warning(const char* fmt, const Args&... args);
    template <typename ...Args>
    void info(const char* fmt, const Args&... args);
    template <typename ...Args>
    void debug(const char* fmt, const Args&... args);
};

#endif