#include "radio/Radio.hpp"

#include "loguru/loguru.hpp"

namespace paresis
{
Radio::Radio(Core* c) : BaseObject(c)
{
    mObjectName = "Radio";
}

void Radio::startExecution(std::shared_ptr<Action>)
{

}

void Radio::endExecution(std::shared_ptr<Action>)
{

}

void Radio::initObject(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "init Radio");
}

}