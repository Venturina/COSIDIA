#pragma once

#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

template <class T>
class PureLocal {
public:

    PureLocal(void* creator) : mCreator(creator) {};

    std::shared_ptr<T>& getElement(void* caller) {
        enforce(caller == mCreator, "PureLocal was used at a wrong place!");
        return mElement;
    }

    T& operator()(void* caller) {
        enforce(caller == mCreator, "PureLocal was used at a wrong place!");
        return *mElement;
    }

private:
    std::shared_ptr<T> mElement;
    const void* mCreator;
};

}