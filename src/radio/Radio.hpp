#ifndef _RADIO_HPP_NSDK
#define _RADIO_HPP_NSDK

#include "objects/BaseObject.hpp"

namespace cosidia
{

class Radio : public BaseObject
{
public:
    Radio();

    void startExecution(std::shared_ptr<Action>) override;
    void endExecution(std::shared_ptr<Action>) override;
    void initObject(std::shared_ptr<Action>) override;

private:

};

}

#endif /* _RADIO_HPP_NSDK */