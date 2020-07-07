#ifndef _SUMO_VEHICLE_UPDATER_HPP_SDFN
#define _SUMO_VEHICLE_UPDATER_HPP_SDFN

#include "actionData/VehicleUpdateActionData.hpp"
#include "mobility/BaseVehicleUpdater.hpp"
#include "traci/LiteAPI.h"

namespace paresis
{

class SumoUpdater
{
public:

    struct Results
    {
        std::list<std::string> departedVehicles;
        std::list<std::string> arrivedVehicles;

        std::list<std::string> updateVehicles;
        std::shared_ptr<VehicleUpdateActionData> updateData;
    };

    SumoUpdater(traci::LiteAPI&);

    void initSubscriptions();
    void updateSubscriptions();

    Results step(std::chrono::milliseconds);

private:
    void subscribeSimulationVariables(const std::set<int>&);
    void subscribeVehicle(const std::string);
    void updateVehicleSubscription(const std::string&, const std::vector<int>&);
    void getVehicleUpdate(const libsumo::TraCIResults&, VehicleUpdate&);

    traci::LiteAPI& mLiteApi;
    std::vector<int> mVehicleVars;
    std::vector<int> mSimVars;
    std::unordered_set<std::string> mSubscribedVehicles;
};

} // ns paresis



#endif /* _SUMO_VEHICLE_UPDATER_HPP_SDFN */