#pragma once

#include <list>
#include <map>
#include <memory>

namespace cosidia
{

class ObjectId;
class Transmission;

using RadioContext = std::map<std::shared_ptr<const Transmission>, std::list<ObjectId>>;
using RadioContextPair = std::pair<std::shared_ptr<const Transmission>, std::list<ObjectId>>;

} // namespace cosidia
