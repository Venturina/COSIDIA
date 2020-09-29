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
    TemporaryObject(int, std::shared_ptr<BaseObject>, bool initialized = false);
    void setTempParent(const TemporaryObject&);
    void setTempChild(const TemporaryObject&);
    void setTempMapping(int);

    void setRealParent(int);
    void setRealChildren(int);

    bool isInitialized() { return mIsInitialized; }
    int getRealId();
    int getTempId() const { return mTempId; }

    void addToCore();

    std::string getObjectName();
    std::set<int> getTemporaryChildren() { return mTempChildren; }
    std::set<int> getTemporaryParent() { return mTempParents; }

    void initialize(SteadyClock::duration);

private:
    int mTempId;
    bool mIsInitialized;
    std::set<int> mTempParents;
    std::set<int> mTempChildren;
    std::pair<int, int> mTempMapping = {-1,-1};

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
    std::map<int,std::shared_ptr<TemporaryObject>> mResolvedIds;
};

} // namespace paresis



#endif /* _TEMPORARY_OBJECT_HPP_ */