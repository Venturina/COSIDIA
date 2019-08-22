#include "objects/BaseObject.hpp"
#include "objects/ObjectFactory.hpp"

#include "objects/VehicleObject.hpp"
#include "radio/MediumAccess.hpp"

namespace paresis
{

std::list<std::shared_ptr<BaseObject>> ObjectFactory::createObject(std::string object)
{
    if(!object.compare("vehicle")) {
        return createVehicleObject();
    } else {
        throw std::runtime_error("Unknown object requested");
    }
}

std::list<std::shared_ptr<BaseObject>> ObjectFactory::createVehicleObject()
{
    auto mediumAccess = std::make_shared<MediumAccess>(mCore);
    auto vehicle = std::make_shared<VehicleObject>(mCore);
    mediumAccess->setParent(0);
    mediumAccess->addChild(vehicle->getObjectId());
    vehicle->setParent(mediumAccess->getObjectId());

    std::list<std::shared_ptr<BaseObject>> list;
    list.push_back(mediumAccess);
    list.push_back(vehicle);

    return list;
}

}