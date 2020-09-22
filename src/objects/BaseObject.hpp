#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include "core/Action.hpp"
#include "objects/ObjectActionManager.hpp"
#include "objects/ObjectContext.hpp"
#include <boost/fiber/all.hpp>
#include <set>

namespace paresis
{

typedef int ObjectId;

class Core;

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    virtual int execute(std::shared_ptr<Action>);
    virtual void startExecution(std::shared_ptr<Action>) = 0;
    virtual void endExecution(std::shared_ptr<Action>) = 0;
    virtual void initObject(std::shared_ptr<Action>) = 0;

    virtual void setObjectId(int id) { mObjectId = id; }
    virtual void addParent(ObjectId id) { mParentList.emplace(id); }
    virtual void addChild(ObjectId id) { mChildIdList.emplace(id); }

    virtual std::set<ObjectId> getParents() const { return mParentList; }
    virtual std::set<ObjectId> getChildren() const { return mChildIdList; }

    virtual ObjectId getObjectId() const { return mObjectId; }
    virtual std::string getObjectName() const { return mObjectName; }

    virtual bool isInitialized();

protected:
    std::shared_ptr<Action> createSelfAction(SteadyClock::duration duration, SteadyClock::duration start);


    ObjectId mObjectId = -1;
    std::set<ObjectId> mParentList;
    std::set<ObjectId> mChildIdList;

    std::unique_ptr<ObjectActionManager> mActionManager;

    std::string mObjectName = "";
};


std::weak_ptr<BaseObject> getSiblingByName(std::shared_ptr<BaseObject> obj, std::string name, ObjectContainer_ptr objects);
int getParentByName(std::shared_ptr<BaseObject> obj, std::string name, ObjectContainer_ptr objects);
int getChildByName(std::shared_ptr<BaseObject> obj, std::string name, ObjectContainer_ptr objects);
int getRelatedObjectByName(std::shared_ptr<BaseObject> obj, std::string name, ObjectContainer_ptr objects);


} // ns paresis

#endif /* _BASE_OBJECT_HPP_ */