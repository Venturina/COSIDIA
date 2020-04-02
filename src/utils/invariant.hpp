#ifndef _INVARIANT_HPP_SNDVO
#define _INVARIANT_HPP_SNDVO

#ifdef PARESIS_SAFE
#define invariant_(condition, msg) \
    if (!condition) { throw std::runtime_error(msg); }
#else
#define invariant_(condition, msg)
#endif

#define invariant(condition, msg) invariant_((condition), msg)

#endif /*  _INVARIANT_HPP_SNDVO */