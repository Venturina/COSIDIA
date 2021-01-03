#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include "core/Action.hpp"
#include "objects/ObjectActionManager.hpp"
#include "objects/ObjectContext.hpp"
#include "objects/ObjectId.hpp"
#include "objects/ObjectList.hpp"
#include <boost/fiber/all.hpp>
#include <set>

namespace paresis
{

class Core;

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    virtual ObjectId execute(std::shared_ptr<Action>);
    virtual void startExecution(std::shared_ptr<Action>) = 0;
    virtual void endExecution(std::shared_ptr<Action>) = 0;
    virtual void initObject(std::shared_ptr<Action>) = 0;

    virtual void setObjectId(ObjectId id) { mObjectId = id; }
    virtual void addParent(ObjectId id) { mParentList.emplace(id); }
    virtual void addChild(ObjectId id) { mChildIdList.emplace(id); }

    virtual std::set<ObjectId> getParents() const { return mParentList; }
    virtual std::set<ObjectId> getChildren() const { return mChildIdList; }

    virtual ObjectId getObjectId() const { return mObjectId; }
    virtual std::string getObjectName() const { return mObjectName; }

    virtual bool isInitialized();

protected:
    std::shared_ptr<Action> createSelfAction(SteadyClock::duration duration, SteadyClock::duration start);


    ObjectId mObjectId;
    std::set<ObjectId> mParentList;
    std::set<ObjectId> mChildIdList;

    std::unique_ptr<ObjectActionManager> mActionManager;

    std::string mObjectName = "";

    #ifdef PARESIS_SAFE
    int mCurrentAction = 0;
    #endif
};


std::weak_ptr<BaseObject> getSiblingByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects);
std::weak_ptr<BaseObject> getParentByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects);
std::weak_ptr<BaseObject> getChildByName(BaseObject* obj, std::string name, ConstObjectContainer_ptr objects);
int getRelatedObjectByName(std::shared_ptr<BaseObject> obj, std::string name, ConstObjectContainer_ptr objects);


} // ns paresis

#endif /* _BASE_OBJECT_HPP_ */