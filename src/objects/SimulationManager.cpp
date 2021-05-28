#include "core/Core.hpp"
#include "core/InitAction.hpp"
#include "objects/SimulationManager.hpp"

#include <loguru/loguru.hpp>

namespace cosidia
{

SimulationManager::SimulationManager() : BaseObject()
{
    mObjectName = "SimulationManager";
    addParent(ObjectId::root());
    mObjectId = getCoreP()->getNextObjectId();
    DLOG_F(INFO, "Id of SimulationManager: %d", mObjectId);

    for(auto & obj : mStarter.getSimulationStarterObjects("test")) {
        startAndScheduleObject(obj);
    }
}

void SimulationManager::startAndScheduleObject(std::shared_ptr<BaseObject> obj)
{
    obj->addParent(mObjectId);
    obj->setObjectId(getCoreP()->getNextObjectId());
    getCoreP()->addUniqueObject(obj);

    auto action = ActionFactory<InitAction>::create(SimClock::duration::zero(), SimClock::atSecond(0), obj->getObjectId(), mObjectId);
    action->scheduleStartHandler();
}

} // namespace cosidia