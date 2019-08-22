#ifndef _RADIO_HPP_NSDK
#define _RADIO_HPP_NSDK

#include "objects/BaseObject.hpp"

namespace paresis
{

class Radio : public BaseObject
{
public:
    Radio(Core* c);

    void startExecution(std::shared_ptr<Action>) override;
    void endExecution(std::shared_ptr<Action>) override;
    void initObject(std::shared_ptr<Action>) override;

private:

};

}

#endif /* _RADIO_HPP_NSDK */