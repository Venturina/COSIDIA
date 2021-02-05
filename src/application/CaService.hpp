#pragma once

#include "objects/BaseObject.hpp"
#include "objects/ObjectId.hpp"
#include "objects/VehicleObject.hpp"
#include "utils/PureLocal.hpp"

#include <vanetza/asn1/cam.hpp>

namespace cosidia
{

struct CaServiceUpdateData
{
    std::list<std::shared_ptr<Action>> actionsToSchedule;
    std::shared_ptr<Action> actionToDelete;
};

class CaService : public  BaseObject
{
public:
    CaService();
    void startExecution(std::shared_ptr<Action>);
    void endExecution(std::shared_ptr<Action>);
    void initObject(std::shared_ptr<Action>);

private:
    CaServiceUpdateData executeUpdate(std::shared_ptr<Action>, std::shared_ptr<const VehicleObjectContext>);
    CaServiceUpdateData init(std::shared_ptr<Action>, ConstObjectContainer_ptr);

    void makeCam(vanetza::asn1::Cam&, Action*, const VehicleObjectContext*);

    std::weak_ptr<VehicleObject> mVehicleObject;

    boost::fibers::future<CaServiceUpdateData> mFuture;

    ObjectId mRouterId = ObjectId::stub();
};

} // namespace cosidia


