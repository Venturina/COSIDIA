#ifndef _BASE_OBJECT_HPP_
#define _BASE_OBJECT_HPP_

#include <boost/fiber/all.hpp>
#include "core/Action.hpp"
#include "objects/ObjectContext.hpp"
#include "set"

namespace paresis
{

typedef int ObjectId;

class Core;

class BaseObject
{
public:
    BaseObject(Core* c);
    virtual void execute(std::shared_ptr<Action>);
    virtual void startExecution(std::shared_ptr<Action>) = 0;
    virtual void endExecution(std::shared_ptr<Action>) = 0;
    virtual void initObject(std::shared_ptr<Action>) = 0;
    virtual ObjectContext copyContext() = 0;

    virtual void setObjectId(int id) { mObjectId = id; }
    virtual void setParent(ObjectId id) { mParent = id; }
    virtual void addChild(ObjectId id) { mChildIdList.emplace(id); }

    virtual ObjectId getObjectId() { return mObjectId; }

protected:
    ObjectId mObjectId = -1;
    ObjectId mParent = -1;
    std::set<ObjectId> mChildIdList;

    Core* mCore;

};

} // ns paresis

#endif /* _BASE_OBJECT_HPP_ */