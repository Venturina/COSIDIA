#include "networking/ParesisRouter.hpp"
#include "objects/BaseObject.hpp"
#include "objects/ObjectFactory.hpp"
#include "objects/VehicleObject.hpp"
#include "radio/Microchannel.hpp"
#include "utils/enforce.hpp"

#include "loguru/loguru.hpp"

namespace paresis
{

ObjectFactory& ObjectFactory::getInstance()
{
    static ObjectFactory instance; // Guaranteed to be destroyed.
    return instance;
}

void ObjectFactory::registerType()
{
    DLOG_F(ERROR, "Object registered");
}

TemporaryObjectList ObjectFactory::createObject(std::string identifier, ConstObjectContainer_ptr objectList, FactoryData* data)
{
    DLOG_F(ERROR, "Create something");
    if(!identifier.compare("vehicle")) {
        return createVehicleObject(objectList, data);
    } else {
        throw std::runtime_error("ObjectFactory: Unknown object requested");
    }
}

TemporaryObjectList ObjectFactory::createVehicleObject(ConstObjectContainer_ptr objectList, FactoryData* data)
{
    enforce(data != nullptr, "ObjectFactory: Need FactoryData to create Vehicle")
    auto router = std::make_shared<ParesisRouter>();
    auto microchannel = std::make_shared<Microchannel>();
    auto vehicle = std::make_shared<VehicleObject>();
    vehicle->setExternalId(data->get<std::string>("id"));

    TemporaryObject tRadio(0, objectList->getUnique("Radio"), true);
    TemporaryObject tChannel(1, microchannel);
    TemporaryObject tVehicle(2, vehicle);
    TemporaryObject tRouter(3, router);

    tRadio.setTempChild(tChannel);
    tChannel.setTempParent(tRadio);

    tChannel.setTempChild(tVehicle);
    tVehicle.setTempParent(tChannel);

    tChannel.setTempChild(tRouter);
    tRouter.setTempParent(tChannel);

    TemporaryObjectList l;
    l.addToList(std::make_shared<TemporaryObject>(tRadio));
    l.addToList(std::make_shared<TemporaryObject>(tChannel));
    l.addToList(std::make_shared<TemporaryObject>(tVehicle));
    l.addToList(std::make_shared<TemporaryObject>(tRouter));

    DLOG_F(ERROR, "Create vehicle");
    return l;
    //return list;
}

}