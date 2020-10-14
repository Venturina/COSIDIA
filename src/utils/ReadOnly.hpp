#pragma once

namespace paresis
{

template <class T>
class ReadOnly {
public:

    void setElement(T& value) {
        enforce(mSet == false, "Tried to ser Read Only was twice");
        mElement = value;
        mSet = true;
    }

    const T& get() {
        enforce(mSet, "Read Only has no initial value");
        return mElement;
    }

private:
    T mElement;
    bool mSet = false;
};

} // namespace paresis
