#include "core/Core.hpp"
//#include "debug/DebugObject.hpp"
#include "objects/SimulationManager.hpp"
#include "mobility/SumoMobilityManager.hpp"
#include "radio/Radio.hpp"

#include <loguru/loguru.hpp>

namespace cosidia
{

SimulationManager::SimulationManager() : BaseObject()
{
    mObjectName = "SimulationManager";
    addParent(ObjectId::root());
    mObjectId = getCoreP()->getNextObjectId();
    DLOG_F(INFO, "Id of SimulationManager: %d", mObjectId);

    startAndScheduleObject(std::make_shared<SumoMobilityManager>());
    //startAndScheduleObject(std::make_shared<MobilityManager>());
    startAndScheduleObject(std::make_shared<Radio>());
    //startAndScheduleObject(std::make_shared<DebugObject>());
}

void SimulationManager::startAndScheduleObject(std::shared_ptr<BaseObject> obj)
{
    obj->addParent(mObjectId);
    obj->setObjectId(getCoreP()->getNextObjectId());
    getCoreP()->addUniqueObject(obj);

    auto action = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, obj->getObjectId(), mObjectId);
    getCoreP()->scheduleAction(std::move(action));
}

} // namespace cosidia