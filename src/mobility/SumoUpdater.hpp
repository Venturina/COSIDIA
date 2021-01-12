#ifndef _SUMO_VEHICLE_UPDATER_HPP_SDFN
#define _SUMO_VEHICLE_UPDATER_HPP_SDFN

#include "actionData/VehicleUpdateActionData.hpp"
#include "mobility/BaseVehicleUpdater.hpp"
#include "traci/LiteAPI.h"

namespace cosidia
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

    /**
     * Performs a SUMO step.
     *
     * @param duration of SUMO step to be executed (usually 100ms)
     * @return Corresponding result of the requested step.
     */
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

} // ns cosidia



#endif /* _SUMO_VEHICLE_UPDATER_HPP_SDFN */