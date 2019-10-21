#ifndef OBJECT_FACTORY_HPP_
#define OBJECT_FACTORY_HPP_

#include "objects/ObjectList.hpp"
#include "objects/TemporaryObject.hpp"

class BaseObject;
class Core;

namespace paresis
{

class ObjectFactory
{
public:
    public:
    static ObjectFactory& getInstance();

    ObjectFactory(ObjectFactory const&) = delete;
    void operator=(ObjectFactory const&) = delete;

    void registerType();


    virtual TemporaryObjectList createObject(std::string, ObjectContainer_ptr objectList = nullptr);

private:
    ObjectFactory() {}

    TemporaryObjectList createVehicleObject(ObjectContainer_ptr objectList);

};

} // namespace paresis
#endif /* OBJECT_FACTORY_HPP_ */