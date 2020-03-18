#include "Config.hpp"
#include <mutex>

namespace paresis
{

static YAML::Node globalConfig;
static std::once_flag globalConfigOnce;

void setGlobalConfig(const YAML::Node& cfg)
{
    std::call_once(globalConfigOnce, [&cfg]() {
        globalConfig = cfg;
    });
}

const YAML::Node& getGlobalConfig()
{
    return globalConfig;
}

} // namespace paresis
