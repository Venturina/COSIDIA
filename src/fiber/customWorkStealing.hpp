#ifndef _CUSTOM_WORK_STEALING_SDOFN
#define _CUSTOM_WORK_STEALING_SDOFN

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>

#include <boost/config.hpp>
#include <boost/intrusive_ptr.hpp>

#include <boost/fiber/algo/algorithm.hpp>
#include <boost/fiber/context.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/context_spinlock_queue.hpp>
#include <boost/fiber/detail/context_spmc_queue.hpp>
#include <boost/fiber/scheduler.hpp>

namespace boost {
namespace fibers {
namespace algo {

class BOOST_FIBERS_DECL CustomWorkStealing : public algorithm
{
public:
    CustomWorkStealing(uint32_t thread_count, std::thread::id id, bool suspend = false);

    CustomWorkStealing( CustomWorkStealing const&) = delete;
    CustomWorkStealing( CustomWorkStealing &&) = delete;

    CustomWorkStealing & operator=( CustomWorkStealing const&) = delete;
    CustomWorkStealing & operator=( CustomWorkStealing &&) = delete;

    virtual void awakened( context *) noexcept;

    virtual context * pick_next() noexcept;

    virtual context * steal() noexcept {
        return rqueue_.steal();
    }

    virtual bool has_ready_fibers() const noexcept {
        return ! rqueue_.empty();
    }

    virtual void push(context* ctx) noexcept {
        rqueue_.push(ctx);
    }

    virtual void suspend_until( std::chrono::steady_clock::time_point const&) noexcept;

    virtual void notify() noexcept;

private:
    static std::atomic< std::uint32_t > counter_;
    static std::vector< intrusive_ptr<CustomWorkStealing>> schedulers_;

    std::uint32_t id_;
    std::uint32_t thread_count_;
#ifdef BOOST_FIBERS_USE_SPMC_QUEUE
    detail::context_spmc_queue queue_{};
#else
    detail::context_spinlock_queue rqueue_{};
#endif
    std::mutex mtx_{};
    std::condition_variable cnd_{};
    bool flag_{ false };
    bool suspend_;
    bool isMain_ = false;

    static void init_( std::uint32_t, std::vector< intrusive_ptr< CustomWorkStealing > > &);

};

}
}
}


#endif /* _CUSTOM_WORK_STEALING_SDOFN */