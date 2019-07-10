#include "fiber/customWorkStealing.hpp"

#include <random>

#include <boost/assert.hpp>
#include <boost/context/detail/prefetch.hpp>

#include "boost/fiber/detail/thread_barrier.hpp"
#include "boost/fiber/type.hpp"

namespace boost{
namespace fibers{
namespace algo {


std::atomic<std::uint32_t> CustomWorkStealing::counter_{ 0 };
std::vector<intrusive_ptr<CustomWorkStealing>> CustomWorkStealing::schedulers_{};

void CustomWorkStealing::init_( std::uint32_t thread_count, std::vector< intrusive_ptr< CustomWorkStealing > > & schedulers) {
    // resize array of schedulers to thread_count, initilized with nullptr
    std::vector<intrusive_ptr<CustomWorkStealing>>{thread_count, nullptr }.swap(schedulers);
}

CustomWorkStealing::CustomWorkStealing( std::uint32_t thread_count, std::thread::id id, bool suspend) : thread_count_{ thread_count }, suspend_{ suspend }
{
    // if (id == std::this_thread::get_id()) {
    //     isMain_ = true;
    // }
    // if(!isMain_) {
        id_ = counter_++;
        std::cout << "scheduler started on thread" << std::endl;
        static boost::fibers::detail::thread_barrier b{ thread_count };
        // initialize the array of schedulers
        static std::once_flag flag;
        std::call_once( flag, & CustomWorkStealing::init_, thread_count_, std::ref( schedulers_) );
        // register pointer of this scheduler
        schedulers_[id_] = this;
        b.wait();
    // } else {
    //     std::cout << "main thread, scheduler not started" << std::endl;
    // }
}

void CustomWorkStealing::awakened( context * ctx) noexcept
{
    if ( ! ctx->is_context( type::pinned_context) ) {
        ctx->detach();
    }
    rqueue_.push(ctx);
    std::cout << "awakened called!!!!!!!!!!!!!!" << std::endl;
}

context* CustomWorkStealing::pick_next() noexcept
{
    context * victim = rqueue_.pop();
    if ( nullptr != victim) {
        boost::context::detail::prefetch_range( victim, sizeof( context) );
        if ( ! victim->is_context( type::pinned_context) ) {
            context::active()->attach( victim);
        }
    } else {
        std::uint32_t id = 0;
        std::size_t count = 0, size = schedulers_.size();
        static thread_local std::minstd_rand generator{ std::random_device{}() };
        std::uniform_int_distribution< std::uint32_t > distribution{0, static_cast< std::uint32_t >( thread_count_ - 1) };
        do {
            do {
                ++count;
                // random selection of one logical cpu
                // that belongs to the local NUMA node
                id = distribution( generator);
                // prevent stealing from own scheduler
            } while ( id == id_);
            // steal context from other scheduler
            victim = schedulers_[id]->steal();
        } while ( nullptr == victim && count < size);
        if ( nullptr != victim) {
            boost::context::detail::prefetch_range( victim, sizeof( context) );
            BOOST_ASSERT( ! victim->is_context( type::pinned_context) );
            context::active()->attach( victim);
        }
    }
    return victim;
}

void CustomWorkStealing::suspend_until( std::chrono::steady_clock::time_point const& time_point) noexcept
{
    if ( suspend_) {
        if ( (std::chrono::steady_clock::time_point::max)() == time_point) {
            std::unique_lock< std::mutex > lk{ mtx_ };
            cnd_.wait( lk, [this](){ return flag_; });
            flag_ = false;
        } else {
            std::unique_lock< std::mutex > lk{ mtx_ };
            cnd_.wait_until( lk, time_point, [this](){ return flag_; });
            flag_ = false;
        }
    }
}

void CustomWorkStealing::notify() noexcept
{
    if ( suspend_) {
        std::unique_lock< std::mutex > lk{ mtx_ };
        flag_ = true;
        lk.unlock();
        cnd_.notify_all();
    }
}


}
}
}