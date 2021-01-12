#ifndef SIMULATION_MANAGER_HPP
#define SIMULATION_MANAGER_HPP

#include "BaseObject.hpp"

namespace cosidia
{

class Core;

class SimulationManager : public BaseObject
{
public:
    SimulationManager();
    virtual void startExecution(std::shared_ptr<Action>){};
    virtual void endExecution(std::shared_ptr<Action>){};
    virtual void initObject(std::shared_ptr<Action>) {};

private:
    void startAndScheduleObject(std::shared_ptr<BaseObject>);
};


} // namespace cosidia

#endif /* SIMULATION_MANAGER_HPP */