#include "core/DurationAction.hpp"
#include "radio/InterferenceDecider.hpp"
#include "radio/PhysicalDecider.hpp"
#include "radio/Radio.hpp"
#include "output/TimingScope.hpp"

#include "loguru/loguru.hpp"

namespace cosidia
{
Radio::Radio() : BaseObject()
{
    mObjectName = "Radio";
    mObjectCache.reset(new SimpleObjectCache());
    auto interference = std::make_unique<NoInterferenceDecider>(NoInterferenceDecider());
    auto physical = std::make_unique<MessagesForEveryonePhysicalDecider>(MessagesForEveryonePhysicalDecider());
    auto decider = std::make_unique<RadioDecider>(std::move(interference), std::move(physical));
    mDecider.swap(decider);
    mRadioContext.swap(std::make_shared<RadioContext>());
}

void Radio::startExecution(std::shared_ptr<Action> action)
{
    if(action->getType() == "transmissionStart"_sym) {  // new tranmission from a router

        boost::fibers::packaged_task<Radio::Result(ActionP, std::shared_ptr<const RadioContext>)>
            pt (std::bind(&Radio::startTransmission, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mRadioContext.getElement()).detach();
    } else if (action->getType() == "transmissionEnd"_sym) {  // transmission ends

        boost::fibers::packaged_task<Radio::Result(ActionP, std::shared_ptr<const RadioContext>)>
            pt (std::bind(&Radio::endTransmission, this, std::placeholders::_1, std::placeholders::_2));

        mFuture = pt.get_future();
        boost::fibers::fiber(std::move(pt), action, mRadioContext.getElement()).detach();
    } else {
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("Radio: wrong BeginAction received");
    }
}

Radio::Result Radio::startTransmission(ActionP action,  std::shared_ptr<const RadioContext> context)
{
    COSIDIA_TIMING(action);
    auto contextCopy = std::make_shared<RadioContext>(*context);
    auto transmission = std::static_pointer_cast<const Transmission>(action->getActionData());

    auto recipients = mDecider->decideOnPhysicalReception(transmission.get());
    contextCopy->insert(RadioContextPair(transmission, recipients));

    //TODO: decide how long the transmission takes and schedule Action accordingly
    auto newAction = createSelfAction(std::chrono::milliseconds(2), action->getStartTime() + action->getDuration() + std::chrono::milliseconds(2));


    newAction->setType("transmissionEnd"_sym);
    newAction->setActionData(transmission);

    Radio::Result result;
    result.actionsToSchedule.push_back(newAction);
    result.contextToSwap = contextCopy;

    return result;
}

Radio::Result Radio::endTransmission(ActionP action, std::shared_ptr<const RadioContext> context)
{
    COSIDIA_TIMING(action);
    auto contextCopy = std::make_shared<RadioContext>(*context);

    //TODO remove current transmission from context
    Radio::Result result;
    auto transmission = std::static_pointer_cast<const Transmission>(action->getActionData());

    auto receivers = mDecider->decideOnInterference(transmission, context.get());
    for(auto& receiver : receivers) {
        auto newAction = ActionFactory<DurationAction>::create(std::chrono::milliseconds(2), action->getStartTime() + action->getDuration() + std::chrono::milliseconds(1) ,receiver, mObjectId);
        newAction->setType("indication"_sym);
        newAction->setActionData(transmission);
        result.actionsToSchedule.push_back(newAction);
    };

    contextCopy->erase(transmission);
    result.contextToSwap = contextCopy;
    return result;
}

void Radio::endExecution(std::shared_ptr<Action> action)
{
    if(action->getType() == "transmissionStart"_sym || action->getType() == "transmissionEnd"_sym) {  // new tranmission from a router
        auto data = mFuture.get();
        if(data.contextToSwap) {
            mRadioContext.swap(data.contextToSwap);
        }
        if(data.actionsToSchedule.size() > 0) {
            for(auto& action : data.actionsToSchedule) {
                action->scheduleStartHandler();
            }
        }
    } else {
        std::cout << action->getType().value() << std::endl;
        throw std::runtime_error("Radio: wrong EndAction received");
    }
}

void Radio::initObject(std::shared_ptr<Action>)
{
    DLOG_F(INFO, "init Radio");
}

} // ns cosidia