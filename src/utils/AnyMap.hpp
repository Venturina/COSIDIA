#ifndef _ANY_MAP_HPP_
#define _ANY_MAP_HPP_

#include "loguru/loguru.hpp"
#include "utils/enforce.hpp"

#include <map>
#include <boost/any.hpp>

namespace cosidia
{

class AnyMap
{
public:
    template<class T>
    void add(std::string key, T value)
    {
        enforce(data.find(key) == data.end(), "Element allready in AnyMap");
        data[key] = value;
    }

    template<class T>
    T get(std::string key)
    {
        enforce(data.find(key) != data.end(), "Element not in AnyMap");
        enforce(data[key].type() == typeid(T), "Wrong type in AnyMap");
        return boost::any_cast<T>(data[key]);
    }

private:
    std::map<std::string, boost::any> data;
};

}

#endif /* _ANY_MAP_HPP_ */