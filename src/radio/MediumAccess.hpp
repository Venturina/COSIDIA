#ifndef _MEDIUM_ACCESS_HPP_SDFN
#define _MEDIUM_ACCESS_HPP_SDFN

#include "objects/BaseObject.hpp"
#include "boost/fiber/fiber.hpp"

namespace paresis
{

class Core;

class MediumAccess : public BaseObject
{
public:
    MediumAccess();
    virtual void startExecution(std::shared_ptr<Action>) override;
    virtual void endExecution(std::shared_ptr<Action>) override;
    virtual void initObject(std::shared_ptr<Action>) {};

private:
    int executeLongOperation();
    boost::fibers::future<int> mFuture;

};


}
#endif /* _MEDIUM_ACCESS_HPP_SDFN */
