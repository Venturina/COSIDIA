#include "mobility/SumoUpdater.hpp"
//#include "sumo/libsumo/TraCIConstants.h"

#include "loguru/loguru.hpp"

namespace paresis
{

SumoUpdater::SumoUpdater(traci::LiteAPI& a) : mLiteApi(a)
{
    initSubscriptions();
}

void SumoUpdater::initSubscriptions()
{
    using namespace libsumo;
    static const std::set<int> vars {
        VAR_DEPARTED_VEHICLES_IDS,
        VAR_ARRIVED_VEHICLES_IDS,
        VAR_DELTA_T,
        VAR_TELEPORT_STARTING_VEHICLES_IDS,
        VAR_TIME
    };

    mVehicleVars.push_back(VAR_SPEED);
    mVehicleVars.push_back(VAR_POSITION);

    subscribeSimulationVariables(vars);

    // subscribe already running vehicles
    for (const std::string& id : mLiteApi.vehicle().getIDList()) {
        subscribeVehicle(id);
    }
}

void SumoUpdater::updateSubscriptions()
{
    
}

SumoUpdater::Results SumoUpdater::step(std::chrono::milliseconds currentSimTime)
{
    const auto& simvars = mLiteApi.simulation().getSubscriptionResults("");
    //DLOG_F(ERROR, "Got %d simvars", simvars.size());
    Results res;

    auto departed = simvars.find(libsumo::VAR_DEPARTED_VEHICLES_IDS);
    if(departed != simvars.end()) {
        const libsumo::TraCIResult& result = *departed->second;
        auto list = dynamic_cast<const libsumo::TraCIStringList&>(result);
        for(auto& name : list.value) {
            DLOG_F(INFO, "vehicle with ID %s inserted", name.c_str());
            subscribeVehicle(name);
            res.departedVehicles.push_back(name);
        }
    }
    auto currentSumoTime = simvars.find(libsumo::VAR_TIME);
    auto currentSumoTimeDouble = dynamic_cast<const libsumo::TraCIDouble&>(*currentSumoTime->second);
    assert(double(currentSimTime.count())/1000 == currentSumoTimeDouble.value);
    //DLOG_F(INFO, "current sumo time: %g, current sim time: %g", currentSumoTimeDouble.value, double(currentSimTime.count())/1000);

    auto arrived = simvars.find(libsumo::VAR_ARRIVED_VEHICLES_IDS);
    if(arrived != simvars.end()) {
        const libsumo::TraCIResult& result = *arrived->second;
        auto list = dynamic_cast<const libsumo::TraCIStringList&>(result);
        for(auto&name : list.value) {
            DLOG_F(INFO, "vehicle with ID %s removed", name.c_str());
            res.arrivedVehicles.push_back(name);
            mSubscribedVehicles.erase(name);
        }
    }

    std::shared_ptr<VehicleUpdateActionData> actionData(new VehicleUpdateActionData());
    for(auto& vehicle : mSubscribedVehicles) {
        const auto vehicleVars = mLiteApi.vehicle().getSubscriptionResults(vehicle);
        VehicleUpdate update;
        update.mVehicle = vehicle;
        res.updateVehicles.push_back(vehicle);
        getVehicleUpdate(vehicleVars, update);
        actionData->addData(vehicle, update);
    }
    res.updateData = std::move(actionData);

    return res;
}

void SumoUpdater::getVehicleUpdate(const libsumo::TraCIResults& vehicleVars, VehicleUpdate& update)
{
    for(auto& vehicleVar : vehicleVars) {
        switch (vehicleVar.first) {
            case libsumo::VAR_SPEED: {
                auto i = dynamic_cast<const libsumo::TraCIDouble*>(vehicleVar.second.get());
                enforce(i, "SumoUpdater: Could not cast TraCI Variable");
                update.mSpeed = i->value;
                break;
            }
            case libsumo::VAR_POSITION: {
                auto pos = dynamic_cast<const libsumo::TraCIPosition*>(vehicleVar.second.get());
                enforce(pos, "SumoUpdater: Could not cast TraCIPosition");
                auto geoPos = mLiteApi.convertGeo(*pos);

                update.mLatitude = geoPos.latitude;
                update.mLongitude = geoPos.longitude;

                break;
            }
            default:
                enforce(false, "SumoUpdater: Traci Subscription not available in VehicleUpdate");
                break;
        }
    }
}

void SumoUpdater::subscribeSimulationVariables(const std::set<int>& add_vars)
{
    std::vector<int> tmp_vars;
    std::set_union(mSimVars.begin(), mSimVars.end(), add_vars.begin(), add_vars.end(), std::back_inserter(tmp_vars));
    std::swap(mSimVars, tmp_vars);
    assert(mSimVars.size() >= tmp_vars.size());

    if (mSimVars.size() != tmp_vars.size()) {
        mLiteApi.simulation().subscribe("", mSimVars, libsumo::INVALID_DOUBLE_VALUE, libsumo::INVALID_DOUBLE_VALUE);
    }

}

void SumoUpdater::subscribeVehicle(const std::string id)
{
    if (!mVehicleVars.empty()) {
        updateVehicleSubscription(id, mVehicleVars);
    }
    mSubscribedVehicles.insert(id);
}

void SumoUpdater::updateVehicleSubscription(const std::string& id, const std::vector<int>& vars)
{
    mLiteApi.vehicle().subscribe(id, vars, libsumo::INVALID_DOUBLE_VALUE, libsumo::INVALID_DOUBLE_VALUE);
}


}
