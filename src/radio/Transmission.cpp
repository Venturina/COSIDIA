#include <radio/Transmission.hpp>
#include <utils/enforce.hpp>

namespace cosidia
{

void Transmission::setupReceiverContext()
{
    enforce(onCoreThread(), "Transmission: setupReceiverContext() on wrong thread");
    enforce(isSetup() == false, "Transmission: duplicate setup");

    for(auto& receiver : mPossibleReceivers) {
        receiver.second.receiverContext = receiver.second.mobility.lock()->getContext();
    }

    mIsSetupDone = true;
}

void Transmission::addPossibleReceiver(ObjectId id, std::shared_ptr<VehicleObject> vehicle)
{
    Receiver r;
    enforce(id.valid(), "Transmission: invalid Receiver Id")
    enforce(vehicle, "Transmission: vehicle Object not valid")
    enforce(!isSetup(), "Transmission: tried to add vehicle to finished transmission")
    r.lowestLayer = id;
    r.mobility = vehicle;
    mPossibleReceivers[id] = r;
}

bool Transmission::isSetup() const
{
    return mIsSetupDone && mEmitter != ObjectId::stub() && mEmitterContext;
};

void Transmission::addEmitter(ObjectId id, std::shared_ptr<const VehicleObjectContext> dynamics)
{
    enforce(id.valid(), "Transmission: invalid Receiver Id")
    enforce(!isSetup(), "Transmission: tried to add vehicle to finished transmission")
    mEmitter = id;
    mEmitterContext = dynamics;
}

} //ns cosidia