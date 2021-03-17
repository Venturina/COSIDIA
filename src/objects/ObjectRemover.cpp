#include "objects/BaseObject.hpp"
#include "objects/ObjectRemover.hpp"
#include "utils/enforce.hpp"

namespace cosidia
{

ObjectRemover& ObjectRemover::getInstance()
{
    static ObjectRemover instance;
    return instance;
}

std::vector<ObjectId> ObjectRemover::getObjectsToDelete(const std::string& objectIdentifier, ObjectId id, ConstObjectContainer_ptr objectList)
{
    if(!objectIdentifier.compare("vehicle")) {
        return getVehicleRelatedObjects(id, objectList);
    }
    return std::vector<ObjectId>();
}

std::vector<ObjectId> ObjectRemover::getVehicleRelatedObjects(ObjectId id, ConstObjectContainer_ptr objectList)
{
    std::vector<ObjectId> removeList;
    auto obj = objectList->getObject(id);
    enforce(obj->getObjectName() == "VehicleObject", "Object to remove must be VehicleObject");
    removeList.push_back(id);

    auto microchannel = getParentByName(obj.get(), "Microchannel", objectList);
    removeList.push_back(microchannel.lock()->getObjectId());

    auto router = getSiblingByName(obj.get(), "Router", objectList);
    removeList.push_back(router.lock()->getObjectId());

    auto caService = getChildByName(router.lock().get(), "CaService", objectList);
    removeList.push_back(caService.lock()->getObjectId());

    return removeList;
}


}