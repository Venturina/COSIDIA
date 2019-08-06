#ifndef OBJECT_FACTORY_HPP_
#define OBJECT_FACTORY_HPP_

#include "objects/ObjectList.hpp"

class BaseObject;
class Core;

namespace paresis
{

class ObjectFactory
{
public:
    ObjectFactory(Core* core) : mCore(core)
    {}

    virtual std::list<std::shared_ptr<BaseObject>> createObject(std::string);


private:
    Core* mCore;

    std::list<std::shared_ptr<BaseObject>> createVehicleObject();

};

} // namespace paresis
#endif /* OBJECT_FACTORY_HPP_ */