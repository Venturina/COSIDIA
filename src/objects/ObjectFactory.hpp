#ifndef OBJECT_FACTORY_HPP_
#define OBJECT_FACTORY_HPP_

#include "objects/ObjectList.hpp"
#include "objects/TemporaryObject.hpp"
#include "utils/AnyMap.hpp"

class BaseObject;
class Core;

namespace paresis
{

using FactoryData = AnyMap;


class ObjectFactory
{
public:
    public:
    static ObjectFactory& getInstance();

    ObjectFactory(ObjectFactory const&) = delete;
    void operator=(ObjectFactory const&) = delete;

    void registerType();

    /**
     * Creates a TemporaryObjectList containing all information and objects for the core to start and resolve objects contained in this list.
     * This includes additional data as well as relations between objects.
     *
     * @param identifier Identifies the object type which should be created
     * @param objectList Current copy of the ObjectContainer, usually available in all executing actions
     * @param data FactoryData which contains additional information needed to generate an Object. See AnyMap for more information.
     *
     */
    virtual TemporaryObjectList createObject(std::string identifier, ConstObjectContainer_ptr objectList = nullptr, FactoryData* data = nullptr);

private:
    ObjectFactory() {}

    TemporaryObjectList createVehicleObject(ConstObjectContainer_ptr objectList, FactoryData*);

};

} // namespace paresis
#endif /* OBJECT_FACTORY_HPP_ */