#include "core/Core.hpp"
#include "objects/SimulationManager.hpp"
#include "mobility/MobilityManager.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

SimulationManager::SimulationManager(Core* c) : BaseObject(c)
{

    DLOG_F(INFO, "Id of SimulationManager: %d", mObjectId);

    auto mob = std::make_shared<MobilityManager>(mCore);
    mCore->addObject(mob);

    auto firstAction = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, mob->getObjectId());
    mCore->scheduleAction(firstAction);
}

} // namespace paresis