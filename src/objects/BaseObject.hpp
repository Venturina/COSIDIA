#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include <boost/fiber/all.hpp>
#include "objects/ObjectContext.hpp"

class BaseObject
{
public:
    virtual void startExecution(ObjectContext) {};
    virtual void endExecution(ObjectContext) {};
    virtual ObjectContext copyContext() {};

private:
    boost::fibers::fiber* mFiber;
};

#endif /* _BASE_OBJECT_HPP_ */