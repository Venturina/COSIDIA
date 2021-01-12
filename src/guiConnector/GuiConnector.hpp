#ifndef _GUI_CONNECTOR_HPP_SDFM
#define _GUI_CONNECTOR_HPP_SDFM

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include "objects/VehicleObject.hpp"

namespace cosidia
{

class GuiConnector
{
public:
    public:
    static GuiConnector& getInstance();

    void setup(boost::asio::io_service& service);

    GuiConnector(GuiConnector const&) = delete;
    void operator=(GuiConnector const&) = delete;

    void updateObject(int VehicleId, VehicleObjectContext context);

private:
    GuiConnector() {};

    bool mIsSetup = false;
    std::unique_ptr<boost::asio::ip::udp::socket> mUdpSocket;
    std::unique_ptr<boost::asio::ip::udp::endpoint> mUdpEndpoint;
};

} // namespace cosidia

#endif /* _GUI_CONNECTOR_HPP_SDFM */