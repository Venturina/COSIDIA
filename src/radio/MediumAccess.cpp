#include "radio/MediumAccess.hpp"
#include <functional>

namespace paresis
{
namespace radio
{

MediumAccess::MediumAccess()
{

}

void MediumAccess::startExecution(std::shared_ptr<core::Action>)
{
    boost::fibers::packaged_task<int()> pt(std::bind(&MediumAccess::executeLongOperation, this));
}

int MediumAccess::executeLongOperation()
{
    std::thread::id my_thread = std::this_thread::get_id();
    std::cout << "launched on thread: " << my_thread << std::endl;
}

}
}