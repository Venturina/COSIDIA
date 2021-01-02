#include "core/Symbol.hpp"
#include <tbb/concurrent_unordered_set.h>
#include <xxhash.h>
#include <string_view>
#include <variant>

namespace paresis
{

class SymbolData
{
public:
    SymbolData(const std::string_view& sv) : mData(sv)
    {
    }

    SymbolData(const std::string& s) : mData(s)
    {
    } 

    SymbolData(const SymbolData& d)
    {
        mData = d.string();
    } 

    SymbolData& operator=(const SymbolData& d)
    {
        mData = d.string();
        return *this;
    }

    std::string string() const
    {
        struct get_value
        {
            std::string operator()(const std::string_view& sv) const
            {
                return std::string { sv }; 
            }

            std::string operator()(const std::string& s) const
            {
                return s;
            } 
        };

        return std::visit(get_value(), mData);
    }

    std::string_view string_view() const
    {
        struct get_string_view
        {
            std::string_view operator()(const std::string_view& sv) const
            {
                return sv;
            } 
        
            std::string_view operator()(const std::string& s) const
            {
                return std::string_view { s };
            }
        };

        return std::visit(get_string_view(), mData);
    } 

    std::size_t hash() const
    {
        struct get_hash
        {
            std::size_t operator()(const std::string& s) const
            {
                return XXH3_64bits(s.data(), s.size());
            }

            std::size_t operator()(const std::string_view& sv) const
            {
                return XXH3_64bits(sv.data(), sv.size());
            } 
        };

        return std::visit(get_hash(), mData);
    } 

    bool operator==(const SymbolData& other) const
    {
        return string_view() == other.string_view();
    }

private:
    std::variant<std::string_view, std::string> mData;
};

struct SymbolHasher
{
    std::size_t operator()(const SymbolData& d) const
    {
        return d.hash();
    } 
};

namespace {

tbb::concurrent_unordered_set<SymbolData, SymbolHasher>& get_symbols()
{
    static tbb::concurrent_unordered_set<SymbolData, SymbolHasher> symbols;
    return symbols;
}

tbb::concurrent_unordered_set<SymbolData, SymbolHasher>::iterator
find_or_create_symbol(const std::string_view& sv)
{
    auto insertion = get_symbols().insert(SymbolData { sv });
    return insertion.first;
}

} // namespace

Symbol::Symbol(const char* data, std::size_t len) :
    mIterator(find_or_create_symbol(std::string_view(data, len)))
{
}

Symbol::Symbol(const std::string_view& sv) :
    mIterator(find_or_create_symbol(sv))
{
}

std::string_view Symbol::value() const
{
    return mIterator->string_view();
}

Symbol operator "" _sym(const char* c, std::size_t len)
{
    return Symbol(c, len);
}

} // namespace paresis
