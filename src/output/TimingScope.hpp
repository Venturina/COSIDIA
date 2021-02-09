#pragma once

#include <chrono>

#if __cpp_lib_source_location >= 201907L
#include <source_location>
namespace cosidia { using source_location = std::source_location; }
#else
#include <experimental/source_location>
namespace cosidia { using source_location = std::experimental::source_location; }
#endif

#ifdef COSIDIA_MEASURE_TIMING
#define COSIDIA_TIMING(action) TimingScope timing##__FUNCTION__##__LINE__ { action->getStartTime() }
#else
#define COSIDIA_TIMING(atcion) {}
#endif

namespace cosidia
{

class TimingScope
{
public:
    TimingScope(SimClock::time_point, const source_location& location = source_location::current());
    ~TimingScope();

private:
    SimClock::time_point m_sim_start;
    std::chrono::high_resolution_clock::time_point m_start;
    source_location m_location;
};

} // namespace cosidia
