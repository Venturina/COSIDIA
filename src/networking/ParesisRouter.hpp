#pragma once

#include "objects/BaseObject.hpp"
#include "objects/VehicleObject.hpp"

namespace paresis
{

class ParesisRouter : public BaseObject
{
public:
    ParesisRouter();
    virtual void startExecution(std::shared_ptr<Action>) {};
    virtual void endExecution(std::shared_ptr<Action>) {};
    virtual void initObject(std::shared_ptr<Action>);


private:

    /* do not touch this variables from other thread than main */
    std::weak_ptr<VehicleObject> mVehicleObject;

    //runtime
    //postion provider
    //router

};

} // namespace paresis