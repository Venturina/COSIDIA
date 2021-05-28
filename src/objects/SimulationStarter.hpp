#pragma once

#include <string>
#include "objects/BaseObject.hpp"

namespace cosidia
{

class SimulationStarter
{
public:
    std::list<std::shared_ptr<BaseObject>> getSimulationStarterObjects(std::string);

private:
    std::list<std::shared_ptr<BaseObject>> getPhold();
    std::list<std::shared_ptr<BaseObject>> getV2x();
};

} // namespace cosidia