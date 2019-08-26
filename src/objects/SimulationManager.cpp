#include "core/Core.hpp"
#include "objects/SimulationManager.hpp"
#include "mobility/MobilityManager.hpp"
#include "radio/Radio.hpp"

#include <loguru/loguru.hpp>

namespace paresis
{

SimulationManager::SimulationManager() : BaseObject()
{
    mObjectName = "SimulationManager";
    mParent = 0;
    mObjectId = getCoreP()->getNextObjectId();
    DLOG_F(INFO, "Id of SimulationManager: %d", mObjectId);

    startAndScheduleObject(std::make_shared<MobilityManager>());
    startAndScheduleObject(std::make_shared<Radio>());
}
void SimulationManager::startAndScheduleObject(std::shared_ptr<BaseObject> obj)
{
    obj->setParent(mObjectId);
    obj->setObjectId(getCoreP()->getNextObjectId());
    getCoreP()->addObject(obj);

    auto action = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, obj->getObjectId());
    getCoreP()->scheduleAction(std::move(action));
}

} // namespace paresis