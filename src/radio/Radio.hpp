#ifndef _RADIO_HPP_NSDK
#define _RADIO_HPP_NSDK

#include "objects/BaseObject.hpp"
#include "objects/ObjectCache.hpp"

#include "radio/Transmission.hpp"
#include "utils/ContextContainer.hpp"

#include <boost/circular_buffer.hpp>

#include "radio/fwd.hpp"
#include "radio/RadioDecider.hpp"

//class RadioDecider;

namespace cosidia
{

// TODO: receive sending request from router / mac
// TODO: finish sending of a message

class Radio : public BaseObject
{
public:
    struct Result {
        std::list<ActionP> actionsToSchedule;
        std::shared_ptr<RadioContext> contextToSwap;
    };

    Radio();

    void startExecution(std::shared_ptr<Action>) override;
    void endExecution(std::shared_ptr<Action>) override;
    void initObject(std::shared_ptr<Action>) override;

private:

    Result startTransmission(ActionP,  std::shared_ptr<const RadioContext>);
    Result endTransmission(ActionP, std::shared_ptr<const RadioContext>);

    boost::fibers::future<Result> mFuture;
    std::unique_ptr<ObjectCache> mObjectCache;
    std::map<ObjectId, std::unique_ptr<CacheQuery>> queries;

    std::string mRequestObjectType = "none";
    ContextContainer<RadioContext> mRadioContext;

    std::unique_ptr<RadioDecider> mDecider;
};

}

#endif /* _RADIO_HPP_NSDK */