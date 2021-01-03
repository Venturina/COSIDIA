#pragma once

namespace paresis
{

/**
 * ObjectId is syntactic sugar improving code readability over plain ints
 */
class ObjectId
{
public:
    constexpr explicit ObjectId(int id) : mId(id)
    {
    }

    constexpr bool operator==(const ObjectId& other) const
    {
        return mId == other.mId;
    }

    constexpr bool operator!=(const ObjectId& other) const
    {
        return !(*this == other);
    }

    constexpr bool operator<(const ObjectId& other) const
    {
        return mId < other.mId;
    }

    constexpr bool valid() const
    {
        return mId > -1;
    }

    constexpr int raw() const
    {
        return mId;
    }

    static const ObjectId stub()
    {
        return ObjectId { -1 };
    }

    static const ObjectId root()
    {
        return ObjectId { 0 };
    }

private:
    int mId = -1;
};

} // namespace paresis
