#ifndef _OBJECT_REMOVER_HPP_
#define _OBJECT_REMOVER_HPP_

#include "objects/ObjectList.hpp"

#include <string>
#include <vector>

namespace paresis
{

class ObjectRemover
{
public:
    static ObjectRemover& getInstance();

    ObjectRemover(ObjectRemover const&) = delete;
    void operator=(ObjectRemover const&) = delete;

    virtual std::vector<ObjectId> getObjectsToDelete(const std::string&, ObjectId, ConstObjectContainer_ptr);

protected:
    std::vector<ObjectId> getVehicleRelatedObjects(ObjectId, ConstObjectContainer_ptr);

private:
    ObjectRemover() {};

};

}

#endif /* _OBJECT_REMOVER_HPP_ */