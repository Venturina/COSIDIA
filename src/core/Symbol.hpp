#pragma once

#include <tbb/concurrent_unordered_set.h>
#include <string_view>

namespace cosidia
{

class SymbolData;
    
class Symbol
{
public:
    Symbol(const char* data, std::size_t len);
    Symbol(const std::string_view&);

    inline bool operator==(const Symbol& other) const
    {
        return mIterator == other.mIterator;
    }

    inline bool operator!=(const Symbol& other) const
    {
        return mIterator != other.mIterator;
    }

    std::string_view value() const;

private:
    tbb::concurrent_unordered_set<SymbolData>::iterator mIterator;
};

Symbol operator "" _sym(const char*, std::size_t);

} // namespace cosidia
