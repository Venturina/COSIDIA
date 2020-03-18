#ifndef CONFIG_HPP_DUDOEX3L
#define CONFIG_HPP_DUDOEX3L

#include <yaml-cpp/yaml.h>

namespace paresis
{

void setGlobalConfig(const YAML::Node&);
const YAML::Node& getGlobalConfig();

} // namespace paresis

#endif /* CONFIG_HPP_DUDOEX3L */

