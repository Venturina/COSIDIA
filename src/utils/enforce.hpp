#ifndef _ENFORCE_HPP_ASNVO
#define _ENFORCE_HPP_ASNVO

#include <stdexcept>

#ifdef COSIDIA_SAFE
#define enforce_(condition, msg) \
    if (!condition) { throw std::runtime_error(msg); }
#else
#define enforce_(condition, msg)
#endif

#define enforce(condition, msg) enforce_((condition), msg)

#endif /*  _ENFORCE_HPP_ASNVO */