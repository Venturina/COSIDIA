#include "guiConnector/GuiConnector.hpp"
#include "guiConnector/gen/guiMsg.pb.h"

#include <boost/asio/ip/address.hpp>

namespace paresis
{

GuiConnector& GuiConnector::getInstance()
{
    static GuiConnector instance;
    return instance;
}

void GuiConnector::setup(boost::asio::io_service& service)
{
    using namespace boost::asio::ip;
    assert(mIsSetup == false);
    mIsSetup = true;
    mUdpEndpoint.reset(new udp::endpoint(make_address("127.0.0.1"),15999));
    mUdpSocket.reset(new udp::socket(service, udp::endpoint(make_address("127.0.0.1"),16000)));
}

void GuiConnector::updateObject(int vehicleId, VehicleObjectContext context)
{
    GuiMsg msg;
    msg.set_emitterid(vehicleId);

    Pos* position = msg.mutable_dot();
    position->set_latitude(context.latitude);
    position->set_longitude(context.longitude);
    //std::cout << msg.SerializeAsString().size() << std::endl;

    mUdpSocket->async_send_to(
        boost::asio::buffer(msg.SerializeAsString()), *mUdpEndpoint,
        [](const boost::system::error_code& error, std::size_t bytes_transferred)
            { std::cout << error.message() << " | " << "Transferred vehicle position to GUI. Bytes: " << bytes_transferred << std::endl; }
        );
}

} // namespace paresis