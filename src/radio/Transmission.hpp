#ifndef TRANSMISSION_HPP_LNSFO
#define TRANSMISSION_HPP_LNSFO

#include "actionData/ActionData.hpp"
#include "objects/VehicleObject.hpp"
#include "objects/ObjectId.hpp"

#include "vanetza/access/data_request.hpp"
#include "vanetza/net/chunk_packet.hpp"

#include <list>

namespace cosidia {
class Transmission : public ActionData {
    /**
     * A receiver consists of the lowest layer after the radio and a mobility module for fetching the receiver vehicle's dynamics;
     **/
    struct Receiver
    {
        Receiver() : lowestLayer(-1) {};

        ObjectId lowestLayer;  //TODO has to change to the current lower layer
        std::weak_ptr<VehicleObject> mobility;
        std::shared_ptr<const VehicleObjectContext> receiverContext;

        bool reachable = true;
    };
    
public:
    Transmission(const vanetza::access::DataRequest request, std::unique_ptr<vanetza::ChunkPacket> packet):
        mEmitter(-1), mRequest(request), mPacket(std::move(packet)), ActionData("transmission") {};

    void setupReceiverContext();
    void addPossibleReceiver(ObjectId, std::shared_ptr<VehicleObject>);

private:
    ObjectId mEmitter;
    std::weak_ptr<VehicleObject> mobility;
    std::shared_ptr<const VehicleObjectContext> mEmitterContext;

    //TODO ObjectId instead of int!
    std::map<int, Receiver> mPossibleReceivers;

    const vanetza::access::DataRequest& mRequest;
    std::shared_ptr<vanetza::ChunkPacket> mPacket;

    bool mIsSetupDone = false;
};
} // ns cosidia
#endif /* TRANSMISSION_HPP_LNSFO */