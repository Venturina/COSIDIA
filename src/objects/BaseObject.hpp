#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include <boost/fiber/all.hpp>
#include "core/Action.hpp"
#include "objects/ObjectContext.hpp"

namespace paresis
{

class Core;

class BaseObject
{
public:
    BaseObject(Core* c) : mCore(c) {};
    virtual void execute(std::shared_ptr<Action>);
    virtual void startExecution(std::shared_ptr<Action>) = 0;
    virtual void endExecution(std::shared_ptr<Action>) = 0;
    virtual void initObject(std::shared_ptr<Action>) = 0;
    virtual ObjectContext copyContext() = 0;

protected:
    Core* mCore;
};

} // ns paresis

#endif /* _BASE_OBJECT_HPP_ */