#ifndef _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO
#define _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO

#include "actionData/ActionData.hpp"

#include <map>
#include <string>


namespace paresis
{


class VehicleUpdate
{
public:
    void setSpeed(double speed) { mSpeed = speed; }
    double getSpeed() { return mSpeed; }

    void setVehicle(std::string vehicle) { mVehicle = vehicle; }
    std::string getVehicle() { return mVehicle; }
    void setObjectId(int id) { mObjectId = id; }
    int getObjectId() { return mObjectId; }
private:
    double mSpeed;

    std::string mVehicle;
    int mObjectId;
};

class VehicleUpdateActionData : public ActionData
{
public:
    //TODO check if key exists
    VehicleUpdate getUpdateForVehicle(std::string veh) { return mUpdates[veh]; }

    void addData(std::string veh, VehicleUpdate u) { mUpdates[veh] = u; }

private:
    std::map<std::string, VehicleUpdate> mUpdates;
};


} // namespace paresis

#endif /* _VEHICLE_UPDATE_ACTION_DATA_HPP_SNDFO */
