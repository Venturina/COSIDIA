#ifndef _SUMO_MOBILITY_MANAGER_HPP_NOSDNF
#define _SUMO_MOBILITY_MANAGER_HPP_NOSDNF

#include "mobility/MobilityManager.hpp"
#include "traci/API.h"
#include "traci/Launcher.hpp"
#include "traci/LiteAPI.h"

namespace paresis
{

class SumoMobilityManager : public MobilityManager
{
public:
    SumoMobilityManager() : MobilityManager() {};
    virtual void initObject(std::shared_ptr<Action>) override;

private:
    virtual std::shared_ptr<MobilityManagerData> doVehicleUpdate(std::shared_ptr<Action> action, ObjectContainer_ptr) override;
    std::chrono::milliseconds mUpdateInterval;

    // this has to go to the context if we are cross referencing
    std::unique_ptr<traci::Launcher> mLauncher;
    std::unique_ptr<traci::API> mTraci;
    std::unique_ptr<traci::LiteAPI> mLite;
};

} // ns paresis


#endif /* _SUMO_MOBILITY_MANAGER_HPP_NOSDNF */