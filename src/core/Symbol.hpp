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

    #ifdef COSIDIA_SAFE
    std::string m_debug;
    #endif
};

Symbol operator "" _sym(const char*, std::size_t);

inline std::string symbol_as_string(Symbol sym) {
    std::string_view v = sym.value();
    return {v.data(), v.size()};
}

} // namespace cosidia
