#include <radio/Transmission.hpp>
#include <utils/enforce.hpp>

namespace cosidia
{

void Transmission::setupReceiverContext()
{
    enforce(onCoreThread(), "Transmission: setupReceiverContext() on wrong thread");
    enforce(mIsSetupDone == false, "Tranmssion: duplicate setup");

    for(auto& receiver : mPossibleReceivers) {
        receiver.second.receiverContext = receiver.second.mobility.lock()->getContext();
    }

    mIsSetupDone == true;
}

void Transmission::addPossibleReceiver(ObjectId id, std::shared_ptr<VehicleObject> vehicle)
{
    Receiver r;
    enforce(id.valid(), "Transmission: invalid Receiver Id")
    enforce(vehicle, "Transmission: vehicle Object not valid")
    r.lowestLayer = id;
    r.mobility = vehicle;

    mPossibleReceivers[id.raw()] = r;
}

} //ns cosidia