#pragma once

#include "core/Action.hpp"
#include "core/SteadyClock.hpp"

namespace cosidia
{


class ActiveActionTracker
{
public:
    void setNext(const Action* action);
    bool isEnd(const Action* action);

    bool isStart(const Action* action) const;
    SimClock::time_point nextStart() const;
    const Action* getEnd() const;

private:
    const Action* mNextStart = nullptr;
    const Action* mNextEnd = nullptr;
};

} // namespace cosidia
