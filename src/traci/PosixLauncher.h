#ifndef POSIXLAUNCHER_H_B3W5MDQW
#define POSIXLAUNCHER_H_B3W5MDQW

#include "traci/Launcher.hpp"
#include <string>
#include <unistd.h>
#include <yaml-cpp/yaml.h>

namespace traci
{

class PosixLauncher : public Launcher
{
public:
    PosixLauncher(const YAML::Node&);
    ~PosixLauncher();
    ServerEndpoint launch() override;


    void finish();

private:
    void kill();
    std::string command();
    int lookupPort();
    void initialize();

    std::string m_executable;
    std::string m_command;
    std::string m_sumocfg;
    int m_port;
    int m_seed;
    pid_t m_pid;
    YAML::Node m_config;
};

} // namespace traci

#endif /* POSIXLAUNCHER_H_B3W5MDQW */

