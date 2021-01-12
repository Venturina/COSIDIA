#ifndef _DEBUG_OBJECT_HPP_
#define _DEBUG_OBJECT_HPP_

#include "objects/BaseObject.hpp"

namespace cosidia
{

class DebugObject : public BaseObject
{
public:
    DebugObject();

    virtual void setName(std::string);

    virtual void initObject() {}
    virtual void initObject(std::shared_ptr<Action>) {}

    virtual void startExecution(std::shared_ptr<Action>);
    virtual void endExecution(std::shared_ptr<Action>);

    std::shared_ptr<Action> testCreateSelfAction(SteadyClock::duration duration, SteadyClock::duration start);
};

class DebugObjectParent : public DebugObject
{
public:
    DebugObjectParent();

    virtual void initObject();
    virtual void initObject(std::shared_ptr<Action>);

};

class DebugObjectChild : public DebugObject
{
public:
    DebugObjectChild();

    virtual void initObject();
    virtual void initObject(std::shared_ptr<Action>);

};

} // ns cosidia

#endif /* _DEBUG_OBJECT_HPP_ */