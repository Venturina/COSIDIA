#include <iostream>
#include <boost/fiber/all.hpp>

#include <loguru/loguru.hpp>

#include "core/Core.hpp"

int main(int argc, char* argv[])
{
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);
    LOG_F(INFO, "Hello World");
    paresis::Core test;

    return 0;
}