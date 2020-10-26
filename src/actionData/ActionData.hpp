#ifndef _ACTION_DATA_HPP_
#define _ACTION_DATA_HPP_

#include <string>

namespace paresis
{

class ActionData
{
public:
    ActionData(std::string type) : mDataType(type) {};

    virtual std::string getType() const { return mDataType; }
private:
    std::string mDataType = "";
};

}

#endif /* _ACTION_DATA_HPP_ */