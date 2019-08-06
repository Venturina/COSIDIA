#ifndef SIMULATION_MANAGER_HPP
#define SIMULATION_MANAGER_HPP

#include "BaseObject.hpp"


namespace paresis
{

class Core;

class SimulationManager : public BaseObject
{
public:
    SimulationManager(Core* c);
    virtual void startExecution(std::shared_ptr<Action>){};
    virtual void endExecution(std::shared_ptr<Action>){};
    virtual void initObject(std::shared_ptr<Action>) {};
    virtual ObjectContext copyContext() {};
private:

};


} // namespace paresis

#endif /* SIMULATION_MANAGER_HPP */