#ifndef _TEMPORARY_OBJECT_HPP_
#define _TEMPORARY_OBJECT_HPP_

#include "core/SteadyClock.hpp"

#include <set>
#include <tuple>

namespace paresis
{

class Action;
class BaseObject;

class TemporaryObject
{
public:
    TemporaryObject(ObjectId, std::shared_ptr<BaseObject>, bool initialized = false);
    void setTempParent(const TemporaryObject&);
    void setTempChild(const TemporaryObject&);
    void setTempMapping(ObjectId);

    void setRealParent(ObjectId);
    void setRealChildren(ObjectId);

    bool isInitialized() { return mIsInitialized; }
    ObjectId getRealId();
    ObjectId getTempId() const { return mTempId; }

    void addToCore();

    std::string getObjectName();
    std::set<ObjectId> getTemporaryChildren() { return mTempChildren; }
    std::set<ObjectId> getTemporaryParent() { return mTempParents; }

    void initialize(SteadyClock::duration);

private:
    ObjectId mTempId;
    bool mIsInitialized;
    std::set<ObjectId> mTempParents;
    std::set<ObjectId> mTempChildren;
    std::pair<ObjectId, ObjectId> mTempMapping = { ObjectId::stub(), ObjectId::stub() };

    std::shared_ptr<Action> mInitAction;
    std::shared_ptr<BaseObject> mBaseObject;
};


/**
 * A list of TemporaryObject with relation informaiton.
 *
 */
class TemporaryObjectList
{
public:
    TemporaryObjectList(std::list<std::shared_ptr<TemporaryObject>> l) : mTempList(l) {};
    TemporaryObjectList() = default;

    void addToList(std::shared_ptr<TemporaryObject> obj) { mTempList.push_back(obj); }

    /**
     * Resolves the relation information and adds new objects to Core
     *
     * @param time when init Actions should be scheduled
     */
    void resolveAndStart(SteadyClock::duration);

private:
    std::list<std::shared_ptr<TemporaryObject>> mTempList;
    std::map<ObjectId,std::shared_ptr<TemporaryObject>> mResolvedIds;
};

} // namespace paresis



#endif /* _TEMPORARY_OBJECT_HPP_ */