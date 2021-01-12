#ifndef _MICROCHANNEL_CONTEXT_HPP_
#define _MICROCHANNEL_CONTEXT_HPP_

#include "objects/ObjectContext.hpp"
#include "radio/Transmission.hpp"
#include <vector>

namespace cosidia
{

class MicrochannelContext : public ObjectContext
{
public:

private:
    std::vector<Transmission> activeTransmissions;
};

}

#endif /* MICROCHANNEL_CONTEXT_HPP_ */