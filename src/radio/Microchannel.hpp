#ifndef _MICROCHANNEL_HPP_OSNSAO
#define _MICROCHANNEL_HPP_OSNSAO

#include "objects/BaseObject.hpp"


namespace paresis
{

class Microchannel : public BaseObject
{
public:
    void startExecution(std::shared_ptr<Action>) override;
    void endExecution(std::shared_ptr<Action>) override;
    void initObject(std::shared_ptr<Action>) override {};

private:
    ObjectContext copyContext();
};

}

#endif /* _MICROCHANNEL_HPP_OSNSAO */