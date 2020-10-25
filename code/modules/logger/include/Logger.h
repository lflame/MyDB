#ifndef _LOGGER_H_
#define _LOGGER_H_
#include "cstdarg"
#include "cstdio"

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

    // 单例
    static Logger logger;

    Logger(int inLevel = 5);

    const char* getTypeString(Type type);

    void printTag(FILE* file, Type type);

    void startColoredText(Type type);
    void endColoredText();

    void fatal(const char* fmt, ...);
    void error(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void info(const char* fmt, ...);
    void debug(const char* fmt, ...);
};

#endif