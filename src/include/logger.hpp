#pragma once

#include <iostream>

#define _LOG logger

#define DEBUG_MSG   "DEBUG: "
#define INFO_MSG    " INFO: "
#define WARNING_MSG " WARN: "
#define ERROR_MSG   "ERROR: "
#define BLANK_MSG   "       "

enum LoggingLevel_en {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger{
    private:
        LoggingLevel_en _logging_level;
        LoggingLevel_en _message_level;
        std::string get_level_msg();

    public:
        void set_logging_level(LoggingLevel_en logging_level);

        template <typename T>
        friend Logger& operator<<(Logger& logger, const T& s){
            if(logger._message_level >= logger._logging_level){
                std::cout << logger.get_level_msg();
                std::cout << s;
                std::cout << "\n";
            }
            return logger;
        }

        Logger& operator()(LoggingLevel_en logging_level);
};

extern Logger logger;
