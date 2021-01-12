#ifndef CONFIG_HPP_DUDOEX3L
#define CONFIG_HPP_DUDOEX3L

#include <yaml-cpp/yaml.h>

namespace cosidia
{

void setGlobalConfig(const YAML::Node&);
const YAML::Node& getGlobalConfig();

} // namespace cosidia

#endif /* CONFIG_HPP_DUDOEX3L */

