#ifndef CELESTE_LOGGER_HPP
#define CELESTE_LOGGER_HPP

#include <restbed>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include "easylogging++.h"

class CelesteLogger : public restbed::Logger
{
public:
    static constexpr int MAX_MESSAGE_SIZE = 2048;

    void
    start(const std::shared_ptr<const restbed::Settings>& settings)
    override;
    
    void
    stop(void)
    override;
    
    void
    log(const Level level, const char* format, ...)
    override;
    
    void
    log_if(bool expression, const Level level, const char* format, ...)
    override;
};

#endif