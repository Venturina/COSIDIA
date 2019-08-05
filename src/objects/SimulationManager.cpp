#include "core/Core.hpp"
#include "objects/SimulationManager.hpp"

#include "mobility/MobilityManager.hpp"

namespace paresis
{

SimulationManager::SimulationManager(Core* c) : BaseObject(c)
{

    std::stringstream s;
    s << "id of Simulation Manager: " << mObjectId << std::endl;
    std::cout << s.str();

    auto mob = std::make_shared<MobilityManager>(mCore);
    mCore->addObject(mob);

    auto firstAction = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, mob);
    mCore->scheduleAction(firstAction);
}

} // namespace paresis