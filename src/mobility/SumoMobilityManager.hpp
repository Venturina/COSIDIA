#ifndef _SUMO_MOBILITY_MANAGER_HPP_NOSDNF
#define _SUMO_MOBILITY_MANAGER_HPP_NOSDNF

#include "mobility/MobilityManager.hpp"
#include "mobility/SumoUpdater.hpp"
#include "traci/API.h"
#include "traci/Launcher.hpp"
#include "traci/LiteAPI.h"
#include <memory>

namespace paresis
{

class SumoMobilityManager : public MobilityManager
{
public:
    SumoMobilityManager() : MobilityManager() {};
    virtual void initObject(std::shared_ptr<Action>) override;

private:
    /**
     * Called from MobilityManager, fiber entry Method
     */
    virtual std::shared_ptr<MobilityManagerTasks> doVehicleUpdate(std::shared_ptr<Action> action, ConstObjectContainer_ptr) override;

    std::shared_ptr<MobilityManagerTasks> executeUpdate(const SumoUpdater::Results&, ConstObjectContainer_ptr, std::shared_ptr<Action>);
    void addVehicle(const std::string&, MobilityManagerTasks*, ConstObjectContainer_ptr);

    std::chrono::milliseconds mUpdateInterval;

    // this has to go to the context if we are cross referencing
    std::unique_ptr<traci::Launcher> mLauncher;
    std::unique_ptr<traci::API> mTraci;
    std::unique_ptr<traci::LiteAPI> mLite;

    std::unique_ptr<SumoUpdater> mUpdater;
};

} // ns paresis


#endif /* _SUMO_MOBILITY_MANAGER_HPP_NOSDNF */