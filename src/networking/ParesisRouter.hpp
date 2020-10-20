#pragma once

#include "networking/ParesisPositionProvider.hpp"
#include "objects/BaseObject.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/PureLocal.hpp"

namespace paresis
{

struct RouterUpdateData
{
    std::shared_ptr<Action> actionsToSchedule;
};

class ParesisRouter : public BaseObject
{
public:
    ParesisRouter();
    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);
    virtual void initObject(std::shared_ptr<Action>);

private:

    void scheduleNextUpdate(RouterUpdateData&, const Action*);
    RouterUpdateData executeUpdate(std::shared_ptr<Action> action, std::shared_ptr<const VehicleObjectContext> context);

    /* do not touch this variables from other thread than main */
    std::weak_ptr<VehicleObject> mVehicleObject;
    boost::fibers::future<RouterUpdateData> mFuture;

    /* pure locals, do not return */
    PureLocal<ParesisPositionProvider> mPositionProvider;
    //runtime
    //postion provider
    //router

};

} // namespace paresis