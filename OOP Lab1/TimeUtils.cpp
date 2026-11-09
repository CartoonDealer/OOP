#include "TimeUtils.h"
#include <ctime>

Date TimeUtils::get_today() {
    std::time_t t = std::time(nullptr);
    std::tm now_tm;

#ifdef _WIN32
    // Безопасная версия для Windows (MSVC)
    localtime_s(&now_tm, &t);
#else
    // Потокобезопасная версия для Linux/macOS (POSIX)
    localtime_r(&t, &now_tm);
#endif

    return Date(now_tm.tm_mday, now_tm.tm_mon + 1, now_tm.tm_year + 1900);
}