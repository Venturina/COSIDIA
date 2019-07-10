#include "radio/MediumAccess.hpp"
#include "boost/fiber/future.hpp"
#include <sstream>
#include <functional>

namespace paresis
{

MediumAccess::MediumAccess()
{

}

void MediumAccess::startExecution(std::shared_ptr<Action>)
{
    boost::fibers::packaged_task<int()> pt(std::bind(&MediumAccess::executeLongOperation, this));
    mFuture = pt.get_future();
    boost::fibers::fiber(std::move(pt)).detach();
    mFuture.wait();
    //fi.wait();
    //assert(fi.get()==42);

}

int MediumAccess::executeLongOperation()
{
    auto my_thread = std::this_thread::get_id();
    std::stringstream st;

    st << "Medium Access launched on thread: " << my_thread;
    std::cout << st.str() << std::endl;
    int x;
    for (x = 0; x < 1000000000; x++)
    {
        x=x+x;
    }
    return 42;
}

void MediumAccess::endExecution(std::shared_ptr<Action>)
{
    std::cout << mFuture.get() << std::endl;
}

}