#include "include/logger.hpp"

Logger logger;

void Logger::set_logging_level(LoggingLevel_en logging_level){
    _logging_level = logging_level;
}

Logger& Logger::operator()(LoggingLevel_en logging_level){
    _message_level = logging_level;
    return *this;
}

std::string Logger::get_level_msg(){
    switch(_message_level){
        case DEBUG:
            return std::string(DEBUG_MSG);
        case INFO:
            return std::string(INFO_MSG);
        case WARNING:
            return std::string(WARNING_MSG);
        case ERROR:
            return std::string(ERROR_MSG);
    }
    return std::string(BLANK_MSG);
}
