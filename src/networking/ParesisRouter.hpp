#ifndef PARESIS_ROUTER_HPP_SDOFN
#define PARESIS_ROUTER_HPP_SDOFN

#include "objects/BaseObject.hpp"

namespace paresis
{

class ParesisRouter : public BaseObject
{
public:
    virtual void startExecution(std::shared_ptr<Action>) {};
    virtual void endExecution(std::shared_ptr<Action>) {};
    virtual void initObject(std::shared_ptr<Action>);


private:

    //runtime
    //postion provider
    //router

};

} // namespace paresis



#endif /* PARESIS_ROUTER_HPP_SDOFN */