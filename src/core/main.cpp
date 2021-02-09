#include <iostream>
#include <optional>
#include <string>
#include <boost/fiber/all.hpp>

#include <CLI/CLI.hpp>
#include <loguru/loguru.hpp>
#include <yaml-cpp/yaml.h>

#include "core/Config.hpp"
#include "core/Core.hpp"
#include "core/DebugClock.hpp"
#include "core/SteadyClock.hpp"
#include "output/SqliteBackend.hpp"

int main(int argc, char* argv[])
{
    CLI::App app {"COSIDIA - concurrent simulation of discrete actions"};
    std::string config_file = "config.yaml";
    bool debug_mode = false;
    float sim_speed = 1.0f;
    std::optional<std::string> log_all_file, log_error_file;
    app.add_option("-c,--config", config_file, "configuration YAML file", true);
    app.add_option("--debug", debug_mode, "enable debug mode", true);
    app.add_option("--sim-speed", sim_speed, "simulation speed factor", true);
    app.add_option("--log-all-file", log_all_file, "log everything to this file", false);
    app.add_option("--log-error-file", log_error_file, "log errors to this file", false);

    loguru::init(argc, argv);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    try {
        YAML::Node config = YAML::LoadFile(config_file);
        cosidia::setGlobalConfig(config);
    } catch (const YAML::BadFile& e) {
        LOG_F(ERROR, "Could not read YAML config file");
        return 1;
    }

    if (log_all_file) {
        loguru::add_file(log_all_file->c_str(), loguru::FileMode::Truncate, loguru::Verbosity_MAX);
    }
    if (log_error_file) {
        loguru::add_file(log_error_file->c_str(), loguru::FileMode::Truncate, loguru::Verbosity_ERROR);
    }
    LOG_F(INFO, "Hello World");

    // initialize database now
    cosidia::SqliteBackend::get();

    std::shared_ptr<cosidia::SteadyClock> clock = debug_mode ?
        std::make_shared<cosidia::DebugClock>(sim_speed) :
        std::make_shared<cosidia::SteadyClock>(sim_speed);
    cosidia::Core core(clock);
    loguru::shutdown();
    return 0;
}
