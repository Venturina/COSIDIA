#include "core/Core.hpp"
#include "networking/ParesisRouter.hpp"

namespace paresis
{

ParesisRouter::ParesisRouter() : BaseObject()
{
    mObjectName = "ParesisRouter";
}

void ParesisRouter::initObject(std::shared_ptr<Action> action)
{
    const auto objList = getCoreP()->getCurrentObjectList();
    auto mobility = getSiblingByName(this, "VehicleObject", objList);
    mVehicleObject = std::static_pointer_cast<VehicleObject>(mobility.lock());

}


} // ns paresis