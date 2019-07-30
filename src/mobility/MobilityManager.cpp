#include "mobility/MobilityManager.hpp"

namespace paresis
{

void MobilityManager::startExecution(std::shared_ptr<Action>)
{

}

void MobilityManager::endExecution(std::shared_ptr<Action>)
{

}

void MobilityManager::initObject(std::shared_ptr<Action>)
{
    std::stringstream s;
    s << "Id of MobilityManager: " << mObjectId << std::endl;
    std::cout << s.str() << std::endl;
}

} // namespace paresis
