#ifndef _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO
#define _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO

#include "actionData/ActionData.hpp"
#include "objects/ObjectId.hpp"

#include <map>
#include <string>
#include <tuple>


namespace cosidia
{


class VehicleUpdate
{
public:
    std::tuple<double, double, double> mPosition;
    double mLongitude;
    double mLatitude;
    double mHeading;
    double mSpeed;

    std::string mVehicle;
    ObjectId mObjectId = ObjectId::stub();
};

class VehicleUpdateActionData : public ActionData
{
public:
    VehicleUpdateActionData() : ActionData("") {}
    //TODO check if key exists
    const VehicleUpdate& getUpdateForVehicle(std::string veh) const { return mUpdates.find(veh)->second; }
    VehicleUpdate& setUpdateForVehicle(std::string veh) { return mUpdates[veh]; }
    void addData(std::string veh, VehicleUpdate u) { mUpdates[veh] = u; }

private:
    std::map<std::string, VehicleUpdate> mUpdates;
};


} // namespace cosidia

#endif /* _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO */
