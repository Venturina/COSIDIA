#ifndef _MICROCHANNEL_HPP_OSNSAO
#define _MICROCHANNEL_HPP_OSNSAO

#include "objects/BaseObject.hpp"
#include "radio/MicrochannelContext.hpp"


namespace paresis
{
namespace radio
{

class Microchannel : public object::BaseObject
{
public:
    void startExecution(ObjectContext&);
    void endExecution(ObjectContext&);
private:
    ObjectContext copyContext();
};

}
}

#endif /* _MICROCHANNEL_HPP_OSNSAO */