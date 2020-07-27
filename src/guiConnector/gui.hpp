#ifndef _GUI_HPP_SDLF
#define _GUI_HPP_SDLF

#ifdef WITH_GUI
#define enforce_(condition, msg) \
    if (!condition) { throw std::runtime_error(msg); }
#else
#define enforce_(condition, msg)
#endif

#endif /* _GUI_HPP_SDLF */