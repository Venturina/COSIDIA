#pragma once

#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

template <class T>
class PureLocal {
public:

    PureLocal(void* creator) : mCreator(creator) {};

    template<typename ...Args>
    void constructElement(void* caller, Args&& ...args) {
        constructElement(caller, std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename S, typename ...Args>
    void constructElement(void* caller, Args&& ...args) {
        std::unique_ptr<T> ptr = std::make_unique<S>(std::forward<Args>(args)...);
        constructElement(caller, std::move(ptr));
    }

    void constructElement(void* caller, std::unique_ptr<T> ptr) {
        enforce(caller == mCreator, "PureLocal was used at a wrong place!");
        enforce(!mElement, "PureLocal's element is already constructed");
        mElement = std::move(ptr);
    }

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
