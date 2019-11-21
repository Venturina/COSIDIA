#include <iostream>
#include <boost/fiber/all.hpp>

#include <loguru/loguru.hpp>

#include "core/Core.hpp"
#include "core/DebugClock.hpp"

int main(int argc, char* argv[])
{
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);
    loguru::add_file("error.log", loguru::FileMode::Truncate, loguru::Verbosity_ERROR);
    LOG_F(INFO, "Hello World");
    //paresis::Core test(std::make_shared<paresis::DebugClock>(1));
    paresis::Core test;
    loguru::shutdown();
    return 0;
}