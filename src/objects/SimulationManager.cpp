#include "core/Core.hpp"
#include "objects/SimulationManager.hpp"
#include "mobility/MobilityManager.hpp"
#include "radio/Radio.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

SimulationManager::SimulationManager(Core* c) : BaseObject(c)
{

    DLOG_F(INFO, "Id of SimulationManager: %d", mObjectId);

    startAndScheduleObject(std::make_shared<MobilityManager>(mCore));
    startAndScheduleObject(std::make_shared<Radio>(mCore));
}

void SimulationManager::startAndScheduleObject(std::shared_ptr<BaseObject> obj)
{
    mCore->addObject(obj);
    obj->setParent(mObjectId);
    auto action = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, obj->getObjectId());
    mCore->scheduleAction(std::move(action));
}

} // namespace paresis