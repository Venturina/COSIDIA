#ifndef _GUI_CONNECTOR_HPP_SDFM
#define _GUI_CONNECTOR_HPP_SDFM

class GuiConnector
{
public:
    public:
    static GuiConnector& getInstance();

    GuiConnector(GuiConnector const&) = delete;
    void operator=(GuiConnector const&) = delete;

    void registerType();

private:
    GuiConnector() {}

};

#endif /* _GUI_CONNECTOR_HPP_SDFM */