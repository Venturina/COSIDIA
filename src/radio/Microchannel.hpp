#ifndef _MICROCHANNEL_HPP_OSNSAO
#define _MICROCHANNEL_HPP_OSNSAO

#include "objects/BaseObject.hpp"
#include "radio/MicrochannelContext.hpp"


namespace paresis
{
namespace radio
{

class Microchannel : public BaseObject
{
public:
    void startExecution(ObjectContext&);
    void endExecution(ObjectContext&);
private:
    MicrochannelContext copyContext();
};

}
}

#endif /* _MICROCHANNEL_HPP_OSNSAO */