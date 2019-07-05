#ifndef _RADIO_HPP_NSDK
#define _RADIO_HPP_NSDK

#include "objects/BaseObject.hpp"

namespace paresis
{
namespace radio
{

class Radio : public object::BaseObject
{
public:
    void startExecution(std::shared_ptr<core::Action>) override;
    void endExecution(std::shared_ptr<core::Action>) override;

private:

};


}
}

#endif /* _RADIO_HPP_NSDK */