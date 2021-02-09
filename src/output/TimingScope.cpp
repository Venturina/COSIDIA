#include "core/SteadyClock.hpp"
#include "output/SqliteBackend.hpp"
#include "output/TimingScope.hpp"

namespace cosidia
{

TimingScope::TimingScope(SimClock::time_point start, const source_location& location) :
    m_sim_start(start),
    m_start(std::chrono::high_resolution_clock::now()),
    m_location(location)
{
}

TimingScope::~TimingScope()
{
    auto stop = std::chrono::high_resolution_clock::now();
    SqliteBackend::get().recordTiming(m_location.function_name(), m_sim_start, stop - m_start);
}

} // namespace cosidia
