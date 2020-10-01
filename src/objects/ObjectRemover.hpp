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

    virtual std::vector<int> getObjectsToDelete(std::string, int, ConstObjectContainer_ptr);

protected:
    std::vector<int> getVehicleRelatedObjects(int, ConstObjectContainer_ptr);

private:
    ObjectRemover() {};

};

}

#endif /* _OBJECT_REMOVER_HPP_ */