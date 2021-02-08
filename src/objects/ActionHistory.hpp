#pragma once

#include "core/Action.hpp"

#include <boost/circular_buffer.hpp>

namespace cosidia
{

class ActionHistory
{
public:
    ActionHistory(int size);

    void addAction(std::shared_ptr<const Action>);
    void prettyPrint();
private:
    std::unique_ptr<boost::circular_buffer<std::shared_ptr<const Action>>> mBuffer;
};

} // namespace cosidia