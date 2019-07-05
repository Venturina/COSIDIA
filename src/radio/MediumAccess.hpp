#ifndef _MEDIUM_ACCESS_HPP_SDFN
#define _MEDIUM_ACCESS_HPP_SDFN

#include "objects/BaseObject.hpp"
#include "boost/fiber/fiber.hpp"

namespace paresis
{

class MediumAccess : public BaseObject
{
public:
    MediumAccess();
    void startExecution(std::shared_ptr<Action>) override;
    void endExecution(std::shared_ptr<Action>) override {};

private:
    int executeLongOperation();
    boost::fibers::future<int> mFuture;

};


}
#endif /* _MEDIUM_ACCESS_HPP_SDFN */
