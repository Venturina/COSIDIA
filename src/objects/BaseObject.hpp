#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include <boost/fiber/all.hpp>
#include "core/Action.hpp"
#include "objects/ObjectContext.hpp"

namespace paresis
{
namespace object
{

class BaseObject
{
public:
    virtual void execute(std::shared_ptr<core::Action>);
    virtual void startExecution(std::shared_ptr<core::Action>) {};
    virtual void endExecution(std::shared_ptr<core::Action>) {};
    virtual void initObject(std::shared_ptr<core::Action>) {};
    virtual ObjectContext copyContext() {};

private:
    boost::fibers::fiber* mFiber;
};

} // ns object
} // ns paresis

#endif /* _BASE_OBJECT_HPP_ */