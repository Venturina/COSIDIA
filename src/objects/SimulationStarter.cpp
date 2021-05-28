#include "objects/SimulationStarter.hpp"
#include "mobility/SumoMobilityManager.hpp"
#include "radio/Radio.hpp"
#include "core/Config.hpp"

namespace cosidia
{

std::list<std::shared_ptr<BaseObject>> SimulationStarter::getSimulationStarterObjects(std::string)
{
    const auto& simulationConfig = getGlobalConfig()["simulation"];
    const auto& model = simulationConfig["model"].as<std::string>("v2x");


    if(model == "v2x") {
        return getV2x();
    } else if (model == "phold") {
        return getPhold();
    } else {
        throw std::runtime_error("SimulationStarter: model specified in config is unknown");
    }

}

std::list<std::shared_ptr<BaseObject>> SimulationStarter::getPhold()
{
    std::list<std::shared_ptr<BaseObject>> phold;

    return phold;
}

std::list<std::shared_ptr<BaseObject>> SimulationStarter::getV2x()
{
    std::list<std::shared_ptr<BaseObject>> v2x;
    v2x.push_back(std::make_shared<SumoMobilityManager>());
    v2x.push_back(std::make_shared<Radio>());

    return v2x;
}


} // namespace cosidia

