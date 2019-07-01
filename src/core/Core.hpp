#ifndef _CORE_HPP_ONSO
#define _CORE_HPP_ONSO

#include <boost/asio.hpp>
#include "core/ActionList.hpp"

namespace paresis
{
namespace core
{


class Core
{
public:
    void initialize();
    void runSimulationLoop();


private:
    boost::asio::io_service mIoService;
    ActionList mActivities;

};

} // ns core
} // ns paresis


#endif /* _CORE_HPP_ONSO */