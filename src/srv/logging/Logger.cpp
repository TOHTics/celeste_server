#include <chrono>
#include <cstdio>
#include <cstdarg>

#include "Logger.hpp"

INITIALIZE_EASYLOGGINGPP


using namespace std;
using namespace std::chrono;

void CelesteLogger::start(const shared_ptr<const restbed::Settings>& settings)
{
    // Load configuration from file
    el::Configurations conf("logger-conf.conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Loggers::addFlag(el::LoggingFlag::ImmediateFlush);
}


void CelesteLogger::log(const restbed::Logger::Level level, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char* buf = new char[MAX_MESSAGE_SIZE];
    vsnprintf(buf, MAX_MESSAGE_SIZE, format, arguments);
    va_end(arguments);

    switch (level)
    {
        case Level::INFO:
        {
            LOG(INFO) << buf;
            break;
        }
        case Level::DEBUG:
        {
            LOG(DEBUG) << buf;
            break;    
        }
        case Level::ERROR:
        {
            LOG(ERROR) << buf;
            break;
        }
        case Level::WARNING:
        {
            LOG(WARNING) << buf;
            break;
        }
        case Level::FATAL:
        {
            LOG(FATAL) << buf;
            break;
        }
        case Level::SECURITY:
        {
            LOG(INFO) << buf;
            break;
        }
    }
    delete[] buf;
}

void CelesteLogger::log_if(bool expression, const restbed::Logger::Level level, const char *format, ...)
{
    if (expression)
    {
        va_list arguments;
        va_start(arguments, format);
        log(level, format, arguments);
        va_end(arguments);
    }
}

void CelesteLogger::stop(void)
{

}