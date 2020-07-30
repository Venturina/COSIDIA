#ifndef _GUI_HPP_SDLF
#define _GUI_HPP_SDLF

#include "guiConnector/GuiConnector.hpp"
#include <boost/asio/io_service.hpp>

#ifdef WITH_GUI
#define setupGuiConnector(ioService) \
    GuiConnector::getInstance().setup(ioService);
#else
#define setupGuiConnector(ioService)
#endif

//#define setupConnector(condition) setupConnector_((condition))

#ifdef WITH_GUI
#define guiUpdateObject(objectId, update) \
    GuiConnector::getInstance().updateObject(objectId, update);
#else
#define GuiUpdateObject(objectId, update)
#endif


#endif /* _GUI_HPP_SDLF */