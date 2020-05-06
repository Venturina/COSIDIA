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

std::vector<int> ObjectRemover::getObjectsToDelete(std::string objectIdentifier, int id, ObjectContainer_ptr objectList)
{
    if(!objectIdentifier.compare("vehicle")) {
        return getVehicleRelatedObjects(id, objectList);
    }
}

std::vector<int> ObjectRemover::getVehicleRelatedObjects(int id, ObjectContainer_ptr objectList)
{
    std::vector<int> removeList;
    auto obj = objectList->getObject(id);
    enforce(obj->getObjectName() == "VehicleObject", "Object to remove must be VehicleObject");
    removeList.push_back(id);
    auto parents = obj->getParents();
    for(auto p : parents) {
        enforce(objectList->getObject(p)->getObjectName() == "Microchannel", "Vehicle object must have Microchannel as parent");
        removeList.push_back(p);
    }

    return removeList;
}


}