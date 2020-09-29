#pragma once

#include "core/Core.hpp"
#include "utils/enforce.hpp"

namespace paresis
{

template <class T>
class ContextContainer {
public:

    std::shared_ptr<const T> getElement() {
        enforce(std::this_thread::get_id() == getCoreThreadId(), "Tried to fetch context from wrong thread");
        return mElement;
    }

    void swap(std::shared_ptr<T> newElement) {
        enforce(std::this_thread::get_id() == getCoreThreadId(), "Tried to swap context from wrong thread");
        mElement.swap(newElement);
    }

private:
    std::shared_ptr<T> mElement;
};


} // ns paresis

