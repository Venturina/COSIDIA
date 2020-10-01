#include "objects/BaseObject.hpp"
#include "objects/ObjectRemover.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

ObjectRemover& ObjectRemover::getInstance()
{
    static ObjectRemover instance;
    return instance;
}

std::vector<int> ObjectRemover::getObjectsToDelete(std::string objectIdentifier, int id, ConstObjectContainer_ptr objectList)
{
    if(!objectIdentifier.compare("vehicle")) {
        return getVehicleRelatedObjects(id, objectList);
    }
    return std::vector<int>();
}

std::vector<int> ObjectRemover::getVehicleRelatedObjects(int id, ConstObjectContainer_ptr objectList)
{
    std::vector<int> removeList;
    auto obj = objectList->getObject(id);
    enforce(obj->getObjectName() == "VehicleObject", "Object to remove must be VehicleObject");
    removeList.push_back(id);

    auto microchannel = getParentByName(obj.get(), "Microchannel", objectList);
    removeList.push_back(microchannel.lock()->getObjectId());

    auto router = getSiblingByName(obj.get(), "ParesisRouter", objectList);
    removeList.push_back(router.lock()->getObjectId());

    return removeList;
}


}