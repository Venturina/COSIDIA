#include "core/Core.hpp"
#include "core/InitAction.hpp"
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

    auto action = std::make_shared<InitAction>(SimClock::duration::zero(), SimClock::atSecond(0), obj->getObjectId(), mObjectId);
    action->scheduleStartHandler();

    std::cout << "use: " << action.use_count() << std::endl;
    std::cout << "use: " << action.use_count() << std::endl;
}

} // namespace cosidia