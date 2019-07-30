#include "core/Core.hpp"
#include "objects/SimulationManager.hpp"

#include "mobility/MobilityManager.hpp"

namespace paresis
{

SimulationManager::SimulationManager(Core* c) : BaseObject(c)
{

    std::stringstream s;
    s << "id of Simulation Manager: " << mObjectId << std::endl;
    std::cout << s.str();

    //auto mob = std::shared_ptr<MobilityManager>;
    //mob.reset(new MobilityManager(mCore));
    //MobilityManager mob(c);
    auto mob = std::make_shared<MobilityManager>(c);
    c->addObject(mob);
    auto firstAction = std::make_shared<Action>(SteadyClock::duration{0}, Action::Kind::INIT, SteadyClock::duration{0}, mob);
    c->scheduleAction(firstAction);
}

} // namespace paresis