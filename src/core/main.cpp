#include <iostream>
#include <string>
#include <boost/fiber/all.hpp>

#include <CLI/CLI.hpp>
#include <loguru/loguru.hpp>
#include <yaml-cpp/yaml.h>

#include "core/Config.hpp"
#include "core/Core.hpp"
#include "core/DebugClock.hpp"

int main(int argc, char* argv[])
{
    CLI::App app {"paresis - yet another marvelous simulation environment"};
    std::string config_file = "config.yaml";
    app.add_option("-c", config_file, "configuration YAML file", true);

    loguru::init(argc, argv);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    try {
        YAML::Node config = YAML::LoadFile(config_file);
        paresis::setGlobalConfig(config);
    } catch (const YAML::BadFile& e) {
        LOG_F(ERROR, "Could not read YAML config file");
        return 1;
    }

    loguru::add_file("everything.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);
    loguru::add_file("error.log", loguru::FileMode::Truncate, loguru::Verbosity_ERROR);
    LOG_F(INFO, "Hello World");
    //paresis::Core test(std::make_shared<paresis::DebugClock>(1));
    paresis::Core test;
    loguru::shutdown();
    return 0;
}
