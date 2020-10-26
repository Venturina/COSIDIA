#pragma once

#include "actionData/ParesisAccessInterfaceActionData.hpp"

#include "vanetza/access/interface.hpp"

namespace paresis
{



class ParesisAccessInterface : public vanetza::access::Interface
{
public:


    virtual void request(const vanetza::access::DataRequest&, std::unique_ptr<vanetza::ChunkPacket>) override;

    bool hasTransmissionRequest() const { return hasTransmission; }

    std::shared_ptr<AccesssInterfaceActionData> getCurrentTransmission() { return std::move(mInterfaceActionData); }

    ~ParesisAccessInterface() {};

private:
    bool hasTransmission = false;
    std::shared_ptr<AccesssInterfaceActionData> mInterfaceActionData;
};





} // namespace paresis
